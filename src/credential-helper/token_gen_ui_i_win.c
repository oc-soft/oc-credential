#include "token_gen_ui_i.h"
#include <process.h>
#include <wchar.h>
#include <ctype.h>
#include <windows.h>

#include "buffer/char_buffer.h"
#include "str_conv.h"
#include "fd_io.h"
#include "fd_io_win.h"
#include "logging.h"


/**
 *  block for reading file descriptor in thead
 */
typedef struct _token_gen_ui_read_fd_info token_gen_ui_read_fd_info;

/**
 *  block for reading file descriptor in thead
 */
struct _token_gen_ui_read_fd_info
{
    /**
     * file descriptor
     */
    int fd;
    /**
     * buffer to accumerate 
     */
    buffer_char_buffer* buffer;

    /**
     * flag whether do read data from file descriptor
     */
    int stop_reading;
};


/**
 * convert from utf8 string to utf16 string and fill argv
 */
int
token_gen_ui_i_fill_argv(
    int argc,
    const char* const * argv,
    wchar_t** wargv);

/** 
 * read fd into buffer
 */
int
token_gen_ui_i_read_fd_into_buffer(
    int fd,
    char* tmp_buffer,
    size_t tmp_buffer_size,
    buffer_char_buffer* buffer,
    const int* stop_reading);

/**
 * call back to read file descriptor.
 */
static void
token_gen_ui_i_read_fd_cb(
    token_gen_ui_read_fd_info* info);


/**
 * convert credential response string from buffer
 */
static char*
token_gen_ui_convert_to_credential_response(
    buffer_char_buffer* buffer);

/**
 * run token generator program
 */
int
token_gen_ui_i_run(
    const char* in_data,
    size_t in_data_size,
    const char* program,
    int argc,
    const char** argv,
    file_desc* std_in_fd,
    file_desc* std_out_fd,
    file_desc* std_err_fd,
    char** out_str,
    char** err_str)
{
    int result;
    intptr_t child_pid;
    wchar_t** argv_param;
    wchar_t* program_w;
    size_t idx;
    char* out_res;
    char* err_res;
    size_t rest_in_data;
    ssize_t written_size[] = { 0, 0 };
    uintptr_t thread_ids[] = { -1L, -1L };
    DWORD thread_status[] = { -1, -1 };
    token_gen_ui_read_fd_info read_fd_info[] = {
        {
            file_desc_get_desc(std_out_fd),
            NULL,
            0,
        },
        {
            file_desc_get_desc(std_err_fd),
            NULL,
            0 
        }
    };

    result = 0;
    child_pid = -1;
    out_res = NULL;
    err_res = NULL;
    program_w = NULL;
    argv_param = NULL;
    rest_in_data = in_data_size;
    logging_log(LOG_LEVEL_DEBUG, "token generator run %s", program);
    if (result == 0) {
        for (idx = 0;
            idx < sizeof(read_fd_info) / sizeof(read_fd_info[0]); idx++) {
            read_fd_info[idx].buffer = buffer_char_buffer_create_00(
                token_gen_ui_i_alloc,
                token_gen_ui_i_realloc,
                token_gen_ui_i_free,
                token_gen_ui_i_mem_copy,
                token_gen_ui_i_mem_move,
                100);
            result = read_fd_info[idx].buffer ? 0 : -1;
            if (result) {
                break;
            }
        }
    }
    if (result == 0) {
        program_w = (wchar_t*)str_conv_utf8_to_utf16(program,
            strlen(program),
            token_gen_ui_i_alloc, token_gen_ui_i_free); 
        result = program_w ? 0 : -1;
    }
    if (result == 0) {
        argv_param = (wchar_t**)token_gen_ui_i_alloc(
            (argc + 1) * sizeof(wchar_t*));
        result = argv_param ? 0 : -1;
    }
    if (result == 0) {
        token_gen_ui_i_fill_argv(argc, argv, argv_param);
        argv_param[argc] = NULL;
    }
    if (result == 0) {
        written_size[1] = fd_io_write(
            file_desc_get_desc(std_in_fd),
            &in_data[written_size[0]], rest_in_data);
        if (written_size[1] != -1) {
            written_size[0] = written_size[1];
            rest_in_data -= written_size[1];
        } else {
            result = -1;
        }
    }
    if (result == 0) {
        child_pid = _wspawnv(_P_NOWAIT, program_w,
            (const wchar_t* const*)argv_param);

        result = child_pid != -1 ? 0 : -1;
    }

    if (result == 0) {
        for (idx = 0;
            idx < sizeof(thread_ids) / sizeof(thread_ids[0]); idx++) {
            thread_ids[idx] = _beginthread(
                (void (__cdecl *)(void*))token_gen_ui_i_read_fd_cb,
                0, &read_fd_info[idx]);
            logging_log(LOG_LEVEL_DEBUG,
                "token generator run start thread %d", 
                thread_ids[idx]);
        }
    }


    if (result == 0) {
        DWORD proc_status;
        proc_status = 0;
        do {
            DWORD wstate;
            if (rest_in_data) {
                written_size[1] = fd_io_write(
                    file_desc_get_desc(std_in_fd),
                    &in_data[written_size[0]], rest_in_data);
                if (written_size[1] != -1) {
                    written_size[0] = written_size[1];
                    rest_in_data -= written_size[1];
                } else {
                    result = -1;
                }
            }
            logging_log(LOG_LEVEL_DEBUG, "token generator wait for %d", 
                child_pid);
            wstate = WaitForSingleObject((HANDLE)child_pid, 0); 
            logging_log(LOG_LEVEL_DEBUG, "token generator get res %d", 
                wstate);
            if (wstate == WAIT_OBJECT_0) {
                GetExitCodeProcess((HANDLE)child_pid, &proc_status);
                break;
            } else if (wstate == WAIT_FAILED) {
                proc_status = GetLastError();
                result = -1;
            } else if (wstate != WAIT_TIMEOUT) {
                proc_status = -1;
                result = -1;
            }
            if (result) {
                break;
            }
        } while (1);

        logging_log(LOG_LEVEL_DEBUG, "token generator run proc status %d", 
            proc_status);
        for (idx = 0;
            idx < sizeof(thread_ids) / sizeof(thread_ids[0]); idx++) {
            read_fd_info[idx].stop_reading = 1;
        }
 
        for (idx = 0;
            idx < sizeof(thread_ids) / sizeof(thread_ids[0]); idx++) {
            if (thread_ids[idx] != -1L) {
                DWORD wstate;
                wstate = WaitForSingleObject(
                    (HANDLE)thread_ids[idx], 
                    INFINITE);
                if (wstate == WAIT_OBJECT_0) {
                    GetExitCodeThread((HANDLE)thread_ids[idx],
                        &thread_status[idx]);
                } else if (wstate == WAIT_FAILED) {
                    thread_status[idx] = GetLastError();
                } 
            }
        }


        if (proc_status == 0) {
            out_res = token_gen_ui_convert_to_credential_response(
                read_fd_info[0].buffer);
            result = out_res ? 0 : -1;
        } else {
            err_res = (char*)token_gen_ui_i_alloc(
                buffer_char_buffer_get_size(
                    read_fd_info[1].buffer) + 1);
            result = err_res ? 0 : -1;
            if (result == 0) {
                buffer_char_buffer_copy_to(
                    read_fd_info[1].buffer, err_res);
                err_res[buffer_char_buffer_get_size(
                    read_fd_info[1].buffer)] = '\0'; 
            }
        } 
        if (proc_status == 0) {
            if (out_res) {
                *out_str = out_res;
            }
        } else {
            if (err_res) {
                *err_str = err_res;
            }
        }
    }
    
    if (child_pid != -1) {
        CloseHandle((HANDLE)child_pid);
    }
    if (argv_param) {
        for (idx = 0; idx < argc; idx++) {
            token_gen_ui_i_free(argv_param[idx]);
        }
        token_gen_ui_i_free(argv_param);
    }
    if (program_w) {
        token_gen_ui_i_free(program_w);
    }

    for (idx = 0; idx < sizeof(read_fd_info) / sizeof(read_fd_info[0]); idx++) {
        if (read_fd_info[idx].buffer) {
            buffer_char_buffer_release(read_fd_info[idx].buffer);
        }
    }
    return result;
}

/**
 * convert from utf8 string to utf16 string and fill argv
 */
int
token_gen_ui_i_fill_argv(
    int argc,
    const char* const * argv,
    wchar_t** wargv)
{
    int result = 0;
    int idx;
    memset(wargv, 0, argc * sizeof(wchar_t*));
    for (idx = 0; idx < argc; idx++) {
        wchar_t* warg;
        warg = (wchar_t*)str_conv_utf8_to_utf16(
            argv[idx], strlen(argv[idx]),
            token_gen_ui_i_alloc, token_gen_ui_i_free);
        result = warg ? 0 : -1;
        wargv[idx] = warg;
        if (result) {
            break;
        }
    }
    if (result) {
        int idx0;
        for (idx0 = 0; idx0 < idx; idx0++) {
            token_gen_ui_i_free(wargv[idx0]);
            wargv[idx0] = NULL;
        }
    }
    return result;
}




/**
 * call back to read file descriptor.
 */
static void 
token_gen_ui_i_read_fd_cb(
    token_gen_ui_read_fd_info* info)
{
    char* tmp_buffer;
    size_t tmp_buffer_size;
    int result;
    tmp_buffer = NULL;
    tmp_buffer_size = 256;
    result = 0;
    if (result == 0) {
        tmp_buffer = (char*)token_gen_ui_i_alloc(tmp_buffer_size); 
        result = tmp_buffer ? 0 : -1;
    }

    if (result == 0) {
        result = token_gen_ui_i_read_fd_into_buffer(info->fd,
            tmp_buffer, tmp_buffer_size, info->buffer,
            &info->stop_reading);
    }

    if (tmp_buffer) {
        token_gen_ui_i_free(tmp_buffer);
    }
    _endthreadex(result);
}

/** 
 * read fd into buffer
 */
int
token_gen_ui_i_read_fd_into_buffer(
    int fd,
    char* tmp_buffer,
    size_t tmp_buffer_size,
    buffer_char_buffer* buffer,
    const int* stop_reading) 
{
    int result;
    while (!*stop_reading) {
        int do_read;
        if (fd_io_win_is_pipe(fd)) {
            do_read = fd_io_win_pipe_has_data(fd);
        } else {
            do_read = 1;
        }
        if (do_read) {
            ssize_t read_size; 
            read_size = fd_io_read(fd, tmp_buffer, tmp_buffer_size);
            if (read_size > 0) {
                result = buffer_char_buffer_append(
                    buffer, tmp_buffer, read_size);
            } else if (read_size == 0) {
                result = 0;
                break;
            } else {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    result = 0;
                } else {
                    result = -1;
                }
                break;
            }
        }
        SwitchToThread();
    }
    return result;
}

/**
 * convert credential response string from buffer
 */
static char*
token_gen_ui_convert_to_credential_response(
    buffer_char_buffer* buffer)
{
    /* windows electron stdout always contains newline at first byte.*/
    /* I have to skip skip white spaces of begining of stdout stream */
    /* https://github.com/electron/electron/blob/b0590b6ee874fbeac49bb5615525d145835eb64f/shell/app/electron_main_delegate.cc#L250 */

    const char* raw_buffer_ref;
    char* result;
    size_t idx;
    result = NULL;
    raw_buffer_ref = buffer_char_buffer_get_data(buffer);

    for (idx = 0; idx < buffer_char_buffer_get_size(buffer); idx++) {
        if (!isspace(raw_buffer_ref[idx])) {
            break;
        }
    }
    
    if (idx <= buffer_char_buffer_get_size(buffer)) {
        size_t buffer_length;
        buffer_length = buffer_char_buffer_get_size(buffer) - idx;
        result = (char*)token_gen_ui_i_alloc(buffer_length + 1);
        if (result) {
            memcpy(result, &raw_buffer_ref[idx], buffer_length);
            result[buffer_length] = '\0';
        }

    }
    return result;
}

/* vi: se ts=4 sw=4 et: */

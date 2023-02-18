#include "token_gen_ui_i.h"
#include <process.h>
#include <wchar.h>
#include <windows.h>
#include "str_conv.h"
#include "buffer/char_buffer.h"

/**
 * convert from utf8 string to utf16 string and fill argv
 */
int
token_gen_ui_i_fill_argv(
    int argc,
    const char* const * argv,
    wchar_t** wargv);
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
    int std_in_fd,
    int std_out_fd,
    int std_err_fd,
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
    char* tmp_buffer;
    size_t tmp_buffer_size;
    size_t rest_in_data;
    ssize_t written_size[] = { 0, 0 };
    buffer_char_buffer* buff[] = { 
        NULL, NULL
    }; 
    int buffer_src[] = {
        std_out_fd,
        std_err_fd
    };
    result = 0;
    child_pid = -1;
    out_res = NULL;
    err_res = NULL;
    tmp_buffer = NULL;
    tmp_buffer_size = 256;
    program_w = NULL;
    argv_param = NULL;
    rest_in_data = in_data_size;
    if (result == 0) {
        for (idx = 0; idx < sizeof(buff) / sizeof(buff[0]); idx++) {
            buff[idx] = buffer_char_buffer_create_00(
                token_gen_ui_i_alloc,
                token_gen_ui_i_relloc,
                token_gen_ui_i_free,
                token_gen_ui_i_mem_copy,
                token_gen_ui_i_mem_move,
                100);
            result = buff[idx] ? 0 : -1;
            if (result) {
                break;
            }
        }
    }
    if (result == 0) {
        tmp_buffer = (char*)token_gen_ui_i_alloc(tmp_buffer_size); 
        result = tmp_buffer ? 0 : -1;
    }
    if (result == 0) {
        program_w = (wchar_t*)str_conv_utf8_to_utf16(program,
            strlen(program),
            token_gen_ui_alloc, token_gen_ui_free); 
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
            std_in_fd, &in_data[written_size[0]], rest_in_data);
        if (written_size[1] != -1) {
            written_size[0] = written_size[1];
            rest_in_data -= written_size[1];
        } else {
            result = -1;
        }
    }
    if (result == 0) {
        child_pid = _wspawnv(_P_NOWAIT, program_w, argv_param);
    }

    if (result == 0) {
        DWORD proc_status;
        proc_status = 0;
        do {
            DWORD wstate;
            if (rest_in_data) {
                written_size[1] = fd_io_write(
                    std_in_fd, &in_data[written_size[0]], rest_in_data);
                if (written_size[1] != -1) {
                    written_size[0] = written_size[1];
                    rest_in_data -= written_size[1];
                } else {
                    result = -1;
                }
            }
            wstate = WaitForSingleObject(child_pid, 0); 
            if (wstate == WAIT_OBJECT_0) {
                GetExitCodeProcess(child_pid, &proc_status);
                break;
            } else if (wstate == WAIT_FAILED) {
                proc_status = GetLastError();
                result = -1;
            } else if (wstate != WAIT_TIMEOUT) {
                proc_status = -1;
                result = -1;
            }
            if (result == 0) {
                for (idx = 0; idx < sizeof(buff) / sizeof(buff[0]); idx++) {
                    result = token_gen_ui_i_read_fd_into_buffer(
                        buffer_src[idx], tmp_buffer, tmp_buffer_size,
                        buff[idx]); 
                    if (result) {
                        break;
                    }
                }
            }
            if (result) {
                break;
            }
        } while (1);

        if (proc_status == 0) {
            result = token_gen_ui_i_read_fd_into_buffer(
                buffer_src[0], tmp_buffer, tmp_buffer_size, buff[0]); 
        } else {
            result = token_gen_ui_i_read_fd_into_buffer(
                buffer_src[1], tmp_buffer, tmp_buffer_size, buff[1]); 
        }
        if (proc_status == 0) {
            out_res = (char*)token_gen_ui_i_alloc(
                buffer_char_buffer_get_size(buff[0]) + 1);
            result = out_res ? 0 : -1;
            if (result == 0) {
                buffer_char_buffer_copy_to(buff[0], out_res);
                out_res[buffer_char_buffer_get_size(buff[0])] = '\0'; 
            }
        } else {
            err_res = (char*)token_gen_ui_i_alloc(
                buffer_char_buffer_get_size(buff[1]) + 1);
            result = err_res ? 0 : -1;
            if (result == 0) {
                buffer_char_buffer_copy_to(buff[1], err_res);
                err_res[buffer_char_buffer_get_size(buff[1])] = '\0'; 
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
        CloseHandle(child_pid);
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

    for (idx = 0; idx < sizeof(buff) / sizeof(buff[0]); idx++) {
        if (buff[idx]) {
            buffer_char_buffer_release(buff[idx]);
        }
    }
    if (tmp_buffer) {
        token_gen_ui_i_free(tmp_buffer);
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
            token_gen_ui_free(wargv[idx0]);
            wargv[idx0] = NULL;
        }
    }
    return result;
}


/* vi: se ts=4 sw=4 et: */

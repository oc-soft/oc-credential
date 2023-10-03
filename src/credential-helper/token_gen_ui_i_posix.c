#include "token_gen_ui_i.h"
#include <spawn.h>
#include <sys/wait.h>
#include "buffer/char_buffer.h"
#include "fd_io.h"
#include "logging.h"

extern char** environ;
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
    pid_t child_pid;
    char** argv_param;
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
        file_desc_get_file_desc(std_out_fd),
        file_desc_get_file_desc(std_err_fd)
    };
    result = 0;
    child_pid = 0;
    out_res = NULL;
    err_res = NULL;
    tmp_buffer = NULL;
    tmp_buffer_size = 256;
    argv_param = NULL;
    rest_in_data = in_data_size;
    if (result == 0) {
        for (idx = 0; idx < sizeof(buff) / sizeof(buff[0]); idx++) {
            buff[idx] = buffer_char_buffer_create_00(
                token_gen_ui_i_alloc,
                token_gen_ui_i_realloc,
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
        argv_param = (char**)token_gen_ui_i_alloc((argc + 1) * sizeof(char*));
        result = argv_param ? 0 : -1;
    }
    if (result == 0) {
        token_gen_ui_i_mem_copy(argv_param, argv, argc * sizeof(char*));
        argv_param[argc] = NULL;
    }
    if (result == 0) {
        written_size[1] = fd_io_write(
            file_desc_get_file_desc(std_in_fd),
            &in_data[written_size[0]], rest_in_data);
        if (written_size[1] != -1) {
            written_size[0] = written_size[1];
            rest_in_data -= written_size[1];
        } else {
            result = -1;
        }
    }
    if (result == 0) {
        result = posix_spawn(&child_pid, program, NULL, NULL,
            argv_param, environ);
    }

    if (result == 0) {
        int proc_status;
        int proc_sig;
        int do_flush;
        proc_status = 0;
        proc_sig = 0;
        do_flush = 1;
        do {
            pid_t state_change_pid;
            int wstate;
            wstate = 0;

            if (rest_in_data) {
                written_size[1] = fd_io_write(
                    file_desc_get_file_desc(std_in_fd),
                    &in_data[written_size[0]], rest_in_data);
                if (written_size[1] != -1) {
                    written_size[0] = written_size[1];
                    rest_in_data -= written_size[1];
                } else {
                    result = -1;
                }
            } 
            state_change_pid = waitpid(child_pid, &wstate, WNOHANG); 
            if (state_change_pid) {
                result = state_change_pid == -1 ? -1 : 0;
                if (result == 0) {
                    if (WIFEXITED(wstate)) {
                        proc_status = WEXITSTATUS(wstate);
                        break;
                    }
                    if (WIFSIGNALED(wstate)) {
                        proc_status = -1; 
                        proc_sig = WTERMSIG(wstate);
                        break;
                    }
                }
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
                out_res = NULL;
            }
        } else {
            if (err_res) {
                *err_str = err_res;
                err_res = NULL;
            }
            if (result == 0) {
                result = proc_sig;
            }
        }
    }
    
    if (argv_param) {
        token_gen_ui_i_free(argv_param);
    }
    if (out_res) {
        token_gen_ui_i_free(out_res);
    }
    if (err_res) {
        token_gen_ui_i_free(err_res);
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
 * read fd into buffer
 */
int
token_gen_ui_i_read_fd_into_buffer(
    int fd,
    char* tmp_buffer,
    size_t tmp_buffer_size,
    buffer_char_buffer* buffer) 
{
    int result;
    while (1) {
        ssize_t read_size; 
        read_size = fd_io_read(fd, tmp_buffer, tmp_buffer_size);
        if (read_size > 0) {
            result = buffer_char_buffer_append(buffer, tmp_buffer, read_size);
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
    return result;
}

/* vi: se ts=4 sw=4 et: */

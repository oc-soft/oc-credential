#include "token_gen_ui.h"
#include "token_gen_ui_i.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "token_gen_ui_exec.h"
#include "credential_desc.h"
#include "fd_io.h"
#include "logging.h"

/**
 * run token generator and get token
 */
int
token_gen_ui_run(
    const char* in_data,
    size_t in_data_size,
    char** token)
{
    size_t idx;
    char* exec_path;
    char* out_str;
    char* err_str;
    int result;
    int state;
    struct {
        int pipe_fd[2];
        int original_fd;
        int saved_fd;
        FILE* stream;
    } std_ioe_state[3] = {
        {
            { -1, -1 },
            fd_io_fileno(stdin),
            -1,
            stdin 
        },
        {
            { -1, -1 },
            fd_io_fileno(stdout),
            -1,
            stdout
        },
        {
            { -1, -1 },
            fd_io_fileno(stderr),
            -1,
            stderr
        },
    };
    result = 0;
    exec_path = NULL;
    out_str = NULL;
    err_str = NULL;
    state = 0;
    logging_log(LOG_LEVEL_DEBUG, "token generator run ui");
    for (idx = 0;
        idx < sizeof(std_ioe_state) / sizeof(std_ioe_state[0]);
        idx++) {
        result = fd_io_pipe(std_ioe_state[idx].pipe_fd);
        if (result) {
            break;
        }
    }
    if (result == 0) {
        for (idx = 0;
            idx < sizeof(std_ioe_state) / sizeof(std_ioe_state[0]); idx++) {
            int newfd;
            newfd = fd_io_dup(std_ioe_state[idx].original_fd);
            if (newfd == -1) {
                result = -1;
            } else {
                std_ioe_state[idx].saved_fd = newfd;
            }
             
            if (result) {
                break;
            }
        }
    }
    if (result == 0) {
        for (idx = 1; idx < 3; idx++) {
            result = fd_io_set_nonblock(std_ioe_state[idx].pipe_fd[0]);
            if (result) {
                break;
            }
        }
    }
    if (result == 0) {
        result = fd_io_dup2(
            std_ioe_state[0].pipe_fd[0],
            std_ioe_state[0].original_fd);
        if (result == 0) {
            for (idx = 1; idx < 3; idx++) {
                result = fd_io_dup2(
                    std_ioe_state[idx].pipe_fd[1],
                    std_ioe_state[idx].original_fd);

                if (result) {
                    break;
                }
            }
        }
        if (result) {
            logging_log(LOG_LEVEL_EMERG,
                "tonken generator run ui failed dup2: %d\n"
                "%s:%d", errno, __FILE__, __LINE__);
        }
    }

    if (result == 0) {
        exec_path = token_gen_ui_exec_get_path();
        result = exec_path ? 0 : -1;
    }
     
    if (result == 0) {
        const char* args[] = {
            exec_path
        };

        result = token_gen_ui_i_run(
            in_data, in_data_size,
            exec_path, 1, args,
            std_ioe_state[0].pipe_fd[1],
            std_ioe_state[1].pipe_fd[0],
            std_ioe_state[2].pipe_fd[0],
            &out_str,
            &err_str);
    }
    if (exec_path) {
        token_gen_ui_exec_free(exec_path);
    }
    for (idx = 0;
        idx < sizeof(std_ioe_state) / sizeof(std_ioe_state[0]);
        idx++) {
        if (std_ioe_state[idx].saved_fd != -1) {
            fd_io_dup2(std_ioe_state[idx].saved_fd, 
                std_ioe_state[idx].original_fd);
        }
    }
    if (std_ioe_state[0].pipe_fd[1] != -1) {
        int state_0;
        state_0 = fd_io_close(std_ioe_state[0].pipe_fd[1]);
        if (state == 0) {
            state = state_0;
        }
    }
    for (idx = 1; idx < 3; idx++) {
        if (std_ioe_state[idx].pipe_fd[0] != -1) {
            int state_0;
            state_0 = fd_io_close(std_ioe_state[idx].pipe_fd[0]);
            if (state == 0) {
                state = state_0;
            }
        }
    }
    if (result == 0) {
        if (out_str) {
            credential_desc* desc;
            desc = credential_desc_decode(out_str, strlen(out_str));
            if (desc) {
                if (desc->password) {
                    char* token_0;
                    size_t password_len;
                    password_len = strlen(desc->password);
                    token_0 = (char*)token_gen_ui_i_alloc(password_len + 1);
                    result = token_0 ? 0 : -1;
                    if (result == 0) {
                        memcpy(token_0, desc->password, password_len + 1);
                        *token = token_0;
                    }
                }
            }
            if (desc) {
                credential_desc_free(desc);
            }
        }
    } else {
        if (err_str) {
            fputs(err_str, stderr);
        }
    }
    if (out_str) {
        token_gen_ui_i_free(out_str);
    }
    if (err_str) {
        token_gen_ui_i_free(err_str);
    }
    if (result == 0) {
        result = state;
    }
    return result;
}

/**
 * free object from this interface
 */
void
token_gen_ui_free(
    void* obj)
{
    free(obj);
}


/* vi: se ts=4 sw=4 et: */

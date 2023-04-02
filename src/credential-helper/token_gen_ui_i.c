#include "token_gen_ui_i.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "fd_io.h"
#include "logging.h"

/**
 * allocate heap memory
 */
void*
token_gen_ui_i_alloc(
    size_t size)
{
    return malloc(size);
}

/**
 * free heap memory
 */
void
token_gen_ui_i_free(
    void* obj)
{
    free(obj);
}

/**
 * copy memory
 */
void*
token_gen_ui_i_mem_copy(
    void* dst,
    const void* src,
    size_t size)
{
    return memcpy(dst, src, size);
}

/**
 * move memory
 */
void*
token_gen_ui_i_mem_move(
    void* dst,
    const void* src,
    size_t size)
{
    return memmove(dst, src, size);
}


/**
 * reallocate memory
 */
void*
token_gen_ui_i_realloc(
    void* obj,
    size_t size)
{
    return realloc(obj, size);
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

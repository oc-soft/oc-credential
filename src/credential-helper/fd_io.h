#ifndef __FD_IO_H__
#define __FD_IO_H__

#include <unistd.h>
#include <stddef.h>
#include <stdio.h>

#ifdef __cplusplus
#define _FD_IO_ITFC_BEGIN extern "C" {
#define _FD_IO_ITFC_END }
#else
#define _FD_IO_ITFC_BEGIN 
#define _FD_IO_ITFC_END 
#endif

_FD_IO_ITFC_BEGIN 

/**
 * create pipe
 */
int
fd_io_pipe(
    int* pipe);

/**
 * duplicate file descriptor
 */
int
fd_io_dup(
    int fd);

/**
 * duplicate file descriptor
 */
int
fd_io_dup2(
    int oldfd,
    int newfd);


/**
 * reade data from fd
 */
ssize_t
fd_io_read(
    int fd,
    void* buff,
    size_t size);

/**
 * write data into fd
 */
ssize_t
fd_io_write(
    int fd,
    const void* buff,
    ssize_t size);


/**
 * set file descriptor non-blocking
 */
int
fd_io_set_nonblock(
    int fd);

/**
 * flush data in memory
 */
int
fd_io_fsync(
    int fd);
/**
 * get file descriptor from file stream
 */
int
fd_io_fileno(
    FILE* stream);

/**
 * close file descriptor
 */
int
fd_io_close(
    int fd);

_FD_IO_ITFC_END
/* vi: se ts=4 sw=4 et: */
#endif

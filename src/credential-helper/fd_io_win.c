#include "fd_io.h"
#include <io.h>

/**
 * create pipe
 */
int
fd_pipe(
    int* pipe_fd)
{
    return _pipe(pipe_fd, 0, 0);
}


/**
 * duplicate file descriptor
 */
int
fd_io_dup(
    int fd)
{
    return _dup(fd);
}

/**
 * duplicate file descriptor
 */
int
fd_io_dup2(
    int oldfd,
    int newfd)
{
    return _dup2(oldfd, newfd);
}

/**
 * get file descriptor from file stream
 */
int
fd_io_fileno(
    FILE* stream)
{
    return _fileno(stream);
}

/**
 * reade data from fd
 */
ssize_t
fd_io_read(
    int fd,
    void* buff,
    size_t size)
{
    return _read(fd, buff, size);
}

/**
 * write data into fd
 */
ssize_t
fd_io_write(
    int fd,
    const void* buff,
    ssize_t size)
{
    return _write(fd, buff, size);
}


/**
 * close file descriptor
 */
int
fd_io_close(
    int fd)
{
    return _close(fd);
}

/* vi: se ts=4 sw=4 et: */

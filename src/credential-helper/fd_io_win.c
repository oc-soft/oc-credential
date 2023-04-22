#include "fd_io.h"
#include <io.h>

/**
 * create pipe
 */
int
fd_io_pipe(
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
 * you get no zero if the file descriptor is a tty
 */
int
fd_io_isatty(
    int fd)
{
    return _isatty(fd);
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
 * set file descriptor non-blocking
 */
int
fd_io_set_nonblock(
    int fd)
{
    int result;
    result = 0;
    return result;
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

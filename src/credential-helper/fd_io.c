#include "fd_io.h"
#include <unistd.h>
#include <fcntl.h>
/**
 * create pipe
 */
int
fd_io_pipe(
    int* pipe_fd)
{
    return pipe(pipe_fd);
}

/**
 * duplicate file descriptor
 */
int
fd_io_dup(
    int fd)
{
    return dup(fd);
}

/**
 * duplicate file descriptor
 */
int
fd_io_dup2(
    int oldfd,
    int newfd)
{
    return dup2(oldfd, newfd);
}

/**
 * get file descriptor from file stream
 */
int
fd_io_fileno(
    FILE* stream)
{
    return fileno(stream);
}

/**
 * you get no zero if the file descriptor is a tty
 */
int
fd_io_isatty(
    int fd)
{
    return isatty(fd);
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
    return read(fd, buff, size);
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
    return write(fd, buff, size);
}

/**
 * set file descriptor non-block
 */
int
fd_io_set_nonblock(
    int fd)
{
    return fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

/**
 * flush data in memory
 */
int
fd_io_fsync(
    int fd)
{
    return fsync(fd); 
}

/**
 * close file descriptor
 */
int
fd_io_close(
    int fd)
{
    return close(fd);
}

/* vi: se ts=4 sw=4 et: */


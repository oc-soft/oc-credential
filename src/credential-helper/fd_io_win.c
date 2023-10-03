#include "fd_io.h"
#include "fd_io_win.h"
#include <windows.h>
#include <io.h>
#include <fcntl.h>

/**
 * create pipe
 */
int
fd_io_pipe(
    int* pipe_fd)
{
    return _pipe(pipe_fd, 0, _O_BINARY);
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
    int result;
    result = _dup2(oldfd, newfd);
    if (result == 0) {
        result = newfd;
    }
    return result;
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
 * you get non zero if the fd is pipe
 */
int
fd_io_win_is_pipe(
    int fd)
{
    int result;
    intptr_t hdl;
    result = 0;
    hdl = _get_osfhandle(fd);
    if (hdl != -1) {
        DWORD type;
        type = GetFileType((HANDLE)hdl); 
        result = type == FILE_TYPE_PIPE;

    }
    return result;
}


/**
 * if fd is pipe and contains data in itself.
 */
int
fd_io_win_pipe_has_data(
    int fd)
{
    int result;
    intptr_t hdl;
    result = 0;
    hdl = _get_osfhandle(fd);
    if (hdl != -1) {
        BOOL b_state;
        DWORD available_bytes;
        available_bytes = 0;
        b_state = PeekNamedPipe((HANDLE)hdl,
            NULL, 0, NULL, &available_bytes, NULL);

        if (b_state) {
            result = available_bytes > 0;
        }

    }
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

#ifndef __FD_IO_WIN_H__
#define __FD_IO_WIN_H__


#ifdef __cplusplus
#define _FD_IO_WIN_ITFC_BEGIN extern "C" {
#define _FD_IO_WIN_ITFC_END }
#else
#define _FD_IO_WIN_ITFC_BEGIN 
#define _FD_IO_WIN_ITFC_END 
#endif

_FD_IO_WIN_ITFC_BEGIN 

/**
 * you get non zero if the fd is pipe
 */
int
fd_io_win_is_pipe(
    int fd);


/**
 * if fd is pipe and contains data in itself.
 */
int
fd_io_win_pipe_has_data(
    int fd);

_FD_IO_WIN_ITFC_END
/* vi: se ts=4 sw=4 et: */
#endif

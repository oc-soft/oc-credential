#include "l10n_i.h"

/**
 * open file stream
 */
FILE*
l10n_i_open_fs(
    const char* file_path,
    const char* mode)
{
    return fopen(file_path, mode);
}

/**
 * close file stream
 */
int
l10n_i_close_fs(
    FILE* fs)
{
    return fclose(fs);
}

/**
 * get line from file stream
 */
char*
l10n_i_gets(
    char* buffer,
    int buffer_size,
    FILE* fs)
{
    return fgets(buffer, buffer_size, fs);
}
/* vi: se ts=4 sw=4 et: */

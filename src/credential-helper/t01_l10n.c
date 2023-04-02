
#include "l10n_i.h"
#include <string.h>
#include <stdio.h>


/**
 * test data
 */
const char*
get_test_data();


/**
 * write test data
 */
static int
write_test_data(
    const char* test_file,
    const char* predata,
    const char* test_data,
    const char* postdata);


int
main(
    int argc,
    char** argv)
{
    int result;
    const char* test_data;
    char* path;
    result = 0;
    test_data = get_test_data();

    path = NULL;
    result = argc > 1 ? 0 : -1;
    if (result == 0) {
        puts("start test data\n");
        result = write_test_data(
            argv[1],
            "# the comment line.\n",
            test_data, 
            "end of line not comment\n");
    }
    if (result == 0) {
        path = l10n_i_read_locpath_from_file(argv[1]);
    }
    if (result == 0) {
        size_t path_range[] = { 0, 0 }; 
        
        result = l10n_i_find_path_range(
            test_data, &path_range[0], &path_range[1]);
        if (result == 0) {
            result = memcmp(path, &test_data[path_range[0]],
                path_range[1] - path_range[0]);
        }
        if (result == 0) {
            puts("succeeded\n");
            printf("test data: %.*s\n"
                "read data: %s\n", (int)(path_range[1] - path_range[0]), 
                &test_data[path_range[0]],
                path);
        }
    }

    if (path) {
        l10n_i_free(path);
    }
    return result;
}

/**
 * write test data
 */
static int
write_test_data(
    const char* test_file,
    const char* predata,
    const char* test_data,
    const char* postdata)
{
    int result;
    FILE* st;
    result = 0;
    st = fopen(test_file, "w");
    result = st ? 0 : -1;
    if (result == 0) {
        const char* data[] = {
            predata,
            test_data,
            postdata
        };
        size_t idx;
        for (idx = 0; idx < sizeof(data) / sizeof(data[0]); idx++) {
            result = fputs(data[idx], st) >= 0 ? 0 : -1;
            if (result) {
                break;
            }
        }
    }
    if (st) {
        fclose(st);
    }
    return result;
}

/**
 * test data
 */
const char*
get_test_data()
{
    return "  /the/file path/for testing  \n";
}




/* vi: se ts=4 sw=4 et: */

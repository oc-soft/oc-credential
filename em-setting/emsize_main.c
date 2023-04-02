#include <stdio.h>
#include <stddef.h>

int
main(
    int argc,
    char** argv)
{
#define SIZE_ENTRY(t) #t, sizeof(t)
    int result;
    size_t i;
    struct {
        const char* type_name;
        size_t size;
    } type_size[] = {
        { SIZE_ENTRY(short) },
        { SIZE_ENTRY(int) },
        { SIZE_ENTRY(long) },
        { SIZE_ENTRY(float) },
        { SIZE_ENTRY(double) },
        { SIZE_ENTRY(size_t) },
        { SIZE_ENTRY(ptrdiff_t) },
        { SIZE_ENTRY(void*) },
    };
    result = 0;
    for (i = 0; i < sizeof(type_size) / sizeof(type_size[0]); i++) {
        printf("%s\t%zu\n", type_size[i].type_name, type_size[i].size);
    }
    return result;
} 
/* vi: se ts=4 sw=4 et: */

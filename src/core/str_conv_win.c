#include "str_conv.h"
#include <stdlib.h>
#include <Windows.h>

/**
 * allocate heap memory
 */
void*
str_conv_alloc(
    size_t size);


/**
 * convert utf8 to utf16 string
 */
void*
str_conv_utf8_to_utf16(
    const char* utf8_str,
    size_t size,
    void* (*alloc_mem)(size_t),
    void (*free_mem)(void*))
{
    int size_utf16;
    void* result;
    result = NULL;
    size_utf16 = MultiByteToWideChar(
        CP_UTF8, 0, utf8_str, size, NULL, 0);

    if (size_utf16) {
        void* buffer;
        void* (*alloc_mem_0)(size_t);
        void (*free_mem_0)(void*);
        if (alloc_mem) {
            alloc_mem_0 = alloc_mem;
        } else {
            alloc_mem_0 = str_conv_alloc;
        }
        if (free_mem) {
            free_mem_0 = free_mem;
        } else {
            free_mem_0 = str_conv_free;
        }
        buffer = alloc_mem_0(size_utf16 * sizeof(WCHAR));
        if (buffer) {
            int state;
            state = MultiByteToWideChar(
                CP_UTF8, 0, utf8_str, size, (LPWSTR)buffer, size_utf16);
            if (state) {
                result = buffer;
                buffer = NULL;
            }
        }
        
        if (buffer) {
            free_mem_0(buffer);
        } 
    }
    return result;
}

/**
 * convert utf16 to utf8 string
 */
char*
str_conv_utf16_to_utf8(
    const void* utf16_str,
    size_t size,
    void* (*alloc_mem)(size_t),
    void (*free_mem)(void*))
{
    int size_utf8;
    char* result;
    result = NULL;
    size_utf8 = WideCharToMultiByte(
        CP_UTF8, 0, (LPCWCH)utf16_str, size, NULL, 0, NULL, NULL);

    if (size_utf8) {
        char* buffer;
        void* (*alloc_mem_0)(size_t);
        void (*free_mem_0)(void*);
        if (alloc_mem) {
            alloc_mem_0 = alloc_mem;
        } else {
            alloc_mem_0 = str_conv_alloc;
        }
        if (free_mem) {
            free_mem_0 = free_mem;
        } else {
            free_mem_0 = str_conv_free;
        }
        buffer = (char*)alloc_mem_0(size_utf8);
        if (buffer) {
            int state;
            state = WideCharToMultiByte(
                CP_UTF8, 0, utf16_str, size, (LPSTR)buffer, size_utf8,
                NULL, NULL);
            if (state) {
                result = buffer;
                buffer = NULL;
            }
        }
        
        if (buffer) {
            free_mem_0(buffer);
        } 
    }
    return result;
}

/**
 * convert utf8 str to code page string 
 * windows only.
 */
char*
str_conv_utf8_to_cp_str(
    const char* utf8_str,
    size_t size,
    unsigned int code_page,
    void* (*alloc_mem)(size_t),
    void (*free_mem)(void*))
{
    char* result;
    result = NULL;
    if (utf8_str) {
        int size_utf16;
        void* (*alloc_mem_0)(size_t);
        void (*free_mem_0)(void*);
        if (alloc_mem) {
            alloc_mem_0 = alloc_mem;
        } else {
            alloc_mem_0 = str_conv_alloc;
        }
        if (free_mem) {
            free_mem_0 = free_mem;
        } else {
            free_mem_0 = str_conv_free;
        }
        size_utf16 = MultiByteToWideChar(
            CP_UTF8, 0, utf8_str, size, NULL, 0);
        if (size_utf16) {
            void* utf16_buffer;
            utf16_buffer = alloc_mem_0(size_utf16 * sizeof(WCHAR));
            if (utf16_buffer) {
                int state;
                state = MultiByteToWideChar(
                    CP_UTF8, 0, utf8_str, size,
                    (LPWSTR)utf16_buffer, size_utf16);
                if (state) {
                    int size_cp;
                    size_cp = WideCharToMultiByte(
                        code_page, 0,
                        (LPCWCH)utf16_buffer, size_utf16,
                        NULL, 0, NULL, NULL);
                    if (size_cp) {
                        char* cp_buffer;
                        cp_buffer = (char*)alloc_mem_0(size_cp);
                        if (cp_buffer) {
                            int state;
                            state = WideCharToMultiByte(
                                code_page, 0,
                                utf16_buffer, size_utf16,
                                (LPSTR)cp_buffer, size_cp,
                                NULL, NULL);
                            if (state) {
                                result = cp_buffer;
                                cp_buffer = NULL;
                            }
                        }
                        if (cp_buffer) {
                            free_mem_0(cp_buffer);
                        }
                    } 
                }
            } 
            if (utf16_buffer) {
                free_mem_0(utf16_buffer);
            } 
        }
    }
    return result;
}



/**
 * allocate heap memory
 */
void*
str_conv_alloc(
    size_t size)
{
    return malloc(size);
}


/**
 * free heap object
 */
void
str_conv_free(
    void* heap_obj)
{
    free(heap_obj);
}

/* vi: se ts=4 sw=4 et: */

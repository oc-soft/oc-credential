#include "lmd_ui_reader.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "buffer/char_buffer.h"

/**
 * read number from stdin
 */
int
lmd_ui_reader_get_number_from_ui(
    lmd_ui* ui,
    int* number)
{
    int result;
    buffer_char_buffer* buffer;
    buffer = buffer_char_buffer_create(10); 
     
    result = buffer ? 0 : -1;
    if (result == 0) {
        while (1) {
            int a_char;
            a_char = lmd_ui_get_char(ui);
            if (a_char == EOF) {
                result = -1;
            } else if (a_char == '\b') {
                if (buffer_char_buffer_get_size(buffer)) {
                    result = buffer_char_buffer_remove(buffer,
                        buffer_char_buffer_get_size(buffer) - 1,
                        buffer_char_buffer_get_size(buffer));
                }
            } else if (a_char == '\n' || a_char == '\r') {
                break;
            } else if (a_char == 0x1b) {
                /* esc */
                result = -1;
            } else {
                char chars[] = {
                    (char)a_char
                }; 
                result = buffer_char_buffer_append(buffer, chars, 1);
            }
            if (result) {
                break;
            }
        } 
    }
    if (result == 0) {
        if (buffer_char_buffer_get_size(buffer)) {
            char null_char = '\0';
            result = buffer_char_buffer_append(buffer, &null_char, 1); 
            if (result == 0) {
                char* data_ptr;
                long number_l;
                data_ptr = (char*)buffer_char_buffer_get_data(buffer);
                number_l = strtol(buffer_char_buffer_get_data(buffer),
                    &data_ptr, 0);
                if (data_ptr != buffer_char_buffer_get_data(buffer)) {
                    if (number) {
                        *number = number_l;
                    } 
                    result = 0;
                } else {
                    result = -2;
                }
            } 
        } else {
            result = -2;
        } 
    } 
    if (buffer) {
        buffer_char_buffer_release(buffer);
    }
    return result;
}


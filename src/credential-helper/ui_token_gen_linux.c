#include "ui_token_gen.h"
#include "ui_token_gen_i.h"
#include <spawn.h>


/**
 * create token generator user interface
 */
ui_token_gen*
ui_token_gen_create()
{
    ui_token_gen* result;
    result = ui_token_gen_i_alloc(sizeof(ui_token_gen));
    if (result) {
        result->ref_count = 1;
    }
    return result;
}


/**
 * create token
 */
int
ui_token_gen_create_token(
    ui_token_gen* obj,
    const char* descreptor,
    size_t descptor_length,
    char** response,
    size_t* descriptor_length)
{

}





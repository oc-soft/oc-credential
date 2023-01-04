#include "credential_storage.h"
#include <stdarg.h>
#include <string.h>
#include <stdarg.h>
#include <sqlite3.h>
#include <json-c/json_object.h>
#include <json-c/json_object_iterator.h>
#include <uuid/uuid.h>
#include "cstr.h"
#include "credential_i.h"
#include "app_config.h"
#include "user_resource.h"
#include <stdio.h>

/**
 * find password by id
 */
int
credential_storage_find_password_by_id(
    const char* protocol_id,
    const char* host_id,
    const char* path_id, 
    const char* name_id,
    char** password);

/**
 * insert or update password
 */
int
credential_storage_insert_or_update_password(
    const char* protocol_id,
    const char* host_id,
    const char* path_id,
    const char* name_id,
    const char* password);

/**
 * insert 
 */
int
credential_storage_insert_if_not_exists(
    json_object* insert_statement_obj,
    json_object* select_statements,
    const char* table_name,
    const char* key,
    char** id);

/**
 * get id from key id table
 */
static int
credential_storage_get_id(
    json_object* select_statements,
    const char* table,
    const char* key,
    char** id);

/**
 * insert id into key id table
 */
static int
credential_storage_insert_id(
    json_object* insert_statement_jobj,
    const char* key,
    const char* id);

/**
 * remove password by id parameters
 */
static int
credential_storage_remove_password_by_id(
    const char* protocol_id,
    const char* host_id,
    const char* path_id,
    const char* name_id);

/**
 * run gabage collection for database
 */
int
credential_storage_gc();

/**
 * do gabase collection.
 */
int
credetial_storage_do_gc();

/**
 * you get a flag to run gc or not.
 */
int
credential_storage_need_to_gc(
    int* need_to_gc);

/**
 * update remove log
 */
static int
credentail_storage_update_remove_log();

/**
 * get json object 
 */
static json_object*
credential_storage_get_json(
    json_object* root, ...);
/**
 * get statement string from json object
 */
static int
credential_storage_get_statement_str_from_json(
    json_object* statement_arry,
    char** statement,
    size_t* statement_length);
/**
 * initialize table
 */
static int
credential_storage_init_table_statement(
    json_object* statement_jobj);
/**
 * initialize db
 */
static int
credential_storage_init_db_statement(
    json_object* statement_arry);

/**
 * initialize db
 */
static int
credential_storage_init_db();

/**
 * init tables
 */
static int
credential_storage_init_tables();

/**
 * get statement string from json object
 */
static int
credential_storage_get_statement_str_from_json(
    json_object* statement_arry,
    char** statement,
    size_t* statement_length);

/**
 * generate uuid
 */
static int
credential_storage_generate_uuid(
    char** uuid);
/**
 * get database connection
 */
static sqlite3*
credential_get_connection();

/**
 * create buffer for internal use
 */
static cstr*
credential_storage_create_cstr();


/**
 * find password 
 */
int
credential_storage_find_password(
    const char* protocol,
    const char* host,
    const char* path, 
    const char* username,
    char** password)
{
    int result;
    json_object* app_config;
    json_object* statement_arry_jobj;
    sqlite3_stmt* statement;
    char* prepared_str;
    size_t prepared_str_len;
    int sql_state;
    statement_arry_jobj = NULL;
    prepared_str = NULL;
    prepared_str_len = 0;
    statement = NULL;
    sql_state = 0; 
    app_config = app_config_get(); 
    result = app_config ? 0 : -1;
    if (result == 0) {
        statement_arry_jobj = credential_storage_get_json(app_config,
            "db", "get", "statements", NULL);
        result = statement_arry_jobj ? 0 : -1;
    } 
    if (result == 0) {
        result = credential_storage_get_statement_str_from_json(
            statement_arry_jobj, &prepared_str, &prepared_str_len);
    }
    
    if (result == 0) {
        sql_state = sqlite3_prepare_v2(
            credential_get_connection(),
            prepared_str,
            prepared_str_len,
            &statement, NULL); 
        result = sql_state == SQLITE_OK ? 0 : -1;
    }
    if (result == 0) {
        const char* values[] = {
            protocol ? protocol : "",
            host ? host : "",
            path ? path : "", 
            username ? username : ""
        };
        size_t i;
        for (i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
            sql_state = sqlite3_bind_text(
                statement, i + 1,
                values[i], 
                strlen(values[i]),
                NULL);
            result = sql_state == SQLITE_OK ? 0 : -1;
            if (result) {
                break;
            }
        } 
    }
    if (result == 0) {
        sql_state = sqlite3_step(statement);
        result = sql_state == SQLITE_ROW ? 0 : -1;
    }
    if (result == 0) {
        const char* password_0;
        size_t password_length;
        char* password_1;
        password_0 = sqlite3_column_text(statement, 0);
        result = password_0 ? 0 : -1;
        if (result == 0) {
            password_length = strlen(password_0);
            password_1 = credential_i_str_dup_0(password_0, password_length);
            result = password_1 ? 0 : -1;
        }
        if (result == 0) {
            *password = password_1;
        }
    }
    if (statement) {
        sqlite3_finalize(statement);
    }
    if (prepared_str) {
        credential_i_free(prepared_str);
    }
    if (app_config) {
        json_object_put(app_config);
    } 
    return result;
}

/**
 * find password by id
 */
int
credential_storage_find_password_by_id(
    const char* protocol_id,
    const char* host_id,
    const char* path_id, 
    const char* name_id,
    char** password)
{
    int result;
    json_object* app_config;
    json_object* statement_arry_jobj;
    sqlite3_stmt* statement;
    char* prepared_str;
    size_t prepared_str_len;
    int sql_state;
    statement_arry_jobj = NULL;
    prepared_str = NULL;
    prepared_str_len = 0;
    statement = NULL;
    sql_state = 0; 
    app_config = app_config_get(); 
    result = app_config ? 0 : -1;
    if (result == 0) {
        statement_arry_jobj = credential_storage_get_json(app_config,
            "db", "select", "statements", "password", NULL);
        result = statement_arry_jobj ? 0 : -1;
    } 

    if (result == 0) {
        result = credential_storage_get_statement_str_from_json(
            statement_arry_jobj, &prepared_str, &prepared_str_len);
    }
    
    if (result == 0) {
        sql_state = sqlite3_prepare_v2(
            credential_get_connection(),
            prepared_str,
            prepared_str_len,
            &statement, NULL); 
        result = sql_state == SQLITE_OK ? 0 : -1;
    }
    if (result == 0) {
        const char* values[] = {
            protocol_id ? protocol_id : "",
            host_id ? host_id : "",
            path_id ? path_id : "", 
            name_id ? name_id : ""
        };
        size_t i;
        for (i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
            sql_state = sqlite3_bind_text(
                statement, i + 1, 
                values[i], 
                strlen(values[i]),
                NULL);
            result = sql_state == SQLITE_OK ? 0 : -1;
            if (result) {
                break;
            }
        } 
    }
    if (result == 0) {
        sql_state = sqlite3_step(statement);
        result = sql_state == SQLITE_ROW ? 0 : 1;
    }
    if (result == 0 && password) {
        const char* password_0;
        size_t password_length;
        char* password_1;
        password_0 = sqlite3_column_text(statement, 0);
        result = password_0 ? 0 : -1;
        if (result == 0) {
            password_length = strlen(password_0);
            password_1 = credential_i_str_dup_0(password_0, password_length);
            result = password_1 ? 0 : -1;
        }
        if (result == 0) {
            *password = password_1;
        }
    }
    if (statement) {
        sqlite3_finalize(statement);
    }
    if (prepared_str) {
        credential_i_free(prepared_str);
    }
    if (app_config) {
        json_object_put(app_config);
    } 
    return result;
}


/**
 * insert or update password
 */
int
credential_storage_insert_or_update_password(
    const char* protocol_id,
    const char* host_id,
    const char* path_id,
    const char* name_id,
    const char* password)
{
    int result;
    json_object* app_config;
    json_object* insert_or_update_jobj;
    const char* insert_or_update_str;
    sqlite3_stmt* statement;
    char* prepared_str;
    int sql_state;
    size_t prepared_str_len;
    
    insert_or_update_str = NULL;
    insert_or_update_jobj = NULL;
    prepared_str = NULL;
    prepared_str_len = 0;
    statement = NULL;
    sql_state = 0;
    app_config = app_config_get(); 
    result = app_config ? 0 : -1;
    if (result == 0) {
        result = credential_storage_find_password_by_id(
            protocol_id, host_id, path_id, name_id, NULL);
    }
    if (result == 0) {
        insert_or_update_str = "update";
    } else if (result == 1) {
        insert_or_update_str = "insert";
        result = 0;
    }
    if (result == 0) {
        insert_or_update_jobj = credential_storage_get_json(app_config,
            "db", "store", "statements", "password", insert_or_update_str, NULL);
        result = insert_or_update_jobj ? 0 : -1;
    } 
    if (result == 0) {
        result = credential_storage_get_statement_str_from_json(
            insert_or_update_jobj, &prepared_str, &prepared_str_len);
    }
    if (result == 0) {
        sql_state = sqlite3_prepare_v2(
            credential_get_connection(),
            prepared_str,
            prepared_str_len,
            &statement, NULL); 
        result = sql_state == SQLITE_OK ? 0 : -1;
    }
    if (result == 0) {
        const char* values[] = {
            password ? password : "",
            protocol_id ? protocol_id : "",
            host_id ? host_id : "",
            path_id ? path_id : "", 
            name_id ? name_id : ""
        };
        size_t i;
        for (i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
            sql_state = sqlite3_bind_text(
                statement, i + 1,
                values[i], 
                strlen(values[i]),
                NULL);
            result = sql_state == SQLITE_OK ? 0 : -1;
            if (result) {
                break;
            }
        } 
    }
    if (result == 0) {
        sql_state = sqlite3_step(statement);
        result = sql_state == SQLITE_DONE? 0 : -1;
    }
    if (statement) {
        sqlite3_finalize(statement);
    }
    if (prepared_str) {
        credential_i_free(prepared_str);
    }
    if (app_config) {
        json_object_put(app_config);
    } 
    return result;
}


/**
 * store password 
 */
int
credential_storage_store_password(
    const char* protocol,
    const char* host,
    const char* path,
    const char* username,
    const char* password)
{
    int result;
    json_object* app_config;
    json_object* store_stmts_jobj;
    json_object* select_stmts_jobj;

    struct {
        const char* name;
        const char* key;
        char* id;
    } key_id_table[] = {
        { "protocol", protocol, NULL  },
        { "host", host, NULL  },
        { "path", path, NULL  },
        { "user", username, NULL  }
    };

    store_stmts_jobj = NULL;
    select_stmts_jobj = NULL;
    app_config = app_config_get(); 
    result = app_config ? 0 : -1;

    if (result == 0) {
        store_stmts_jobj = credential_storage_get_json(app_config,
            "db", "store", "statements", NULL);
        result = store_stmts_jobj ? 0 : -1;
    }
    if (result == 0) {
        select_stmts_jobj = credential_storage_get_json(app_config,
            "db", "select", "statements", NULL);
        result = select_stmts_jobj ? 0 : -1;
    }
    if (result == 0) {
        size_t idx;

        for (idx = 0;
            idx < sizeof(key_id_table) / sizeof(key_id_table[0]); idx++) {
            json_object* store_stmt_jobj;
            store_stmt_jobj = json_object_object_get(
                store_stmts_jobj, key_id_table[idx].name);
            result = store_stmt_jobj ? 0 : -1;
            if (result == 0) {
                result = credential_storage_insert_if_not_exists(
                    store_stmts_jobj, select_stmts_jobj,
                    key_id_table[idx].name,
                    key_id_table[idx].key,
                    &key_id_table[idx].id);
            }
            if (result) {
                break;
            }
        }
    }
    if (result == 0) {
        result = credential_storage_insert_or_update_password(
            key_id_table[0].id,
            key_id_table[1].id,
            key_id_table[2].id,
            key_id_table[3].id,
            password);
    }
    {
        size_t i;
        for (i = 0; i < sizeof(key_id_table) / sizeof(key_id_table[0]); i++) {
            if (key_id_table[i].id) {
                credential_i_free(key_id_table[i].id);
            }
        }
    }
    if (app_config) {
        json_object_put(app_config);
    } 
    return result;
}

/**
 * insert 
 */
int
credential_storage_insert_if_not_exists(
    json_object* insert_statement_obj,
    json_object* select_statements,
    const char* table_name,
    const char* key,
    char** id)
{
    int result;
    char* id0;
    json_object* insert_statement_arry;
    json_object* table_name_jobj; 
    insert_statement_arry = NULL;
    id0 = NULL;
    result = -1;
    insert_statement_arry = credential_storage_get_json(
        insert_statement_obj, table_name, "insert", NULL);
    result = insert_statement_arry ? 0 : -1;

    if (result == 0) {
        result = credential_storage_get_id(
            select_statements, 
            table_name,
            key,
            &id0);

        if (result == 1) {
            result = credential_storage_generate_uuid(&id0); 
            if (result == 0) {
                result = credential_storage_insert_id(
                    insert_statement_arry,
                    key, id0); 
            } 
        }
        if (result == 0) {
            *id = id0;
            id0 = NULL;
        }

    }
    if (id0) {
        credential_i_free(id0);
    }
    return result; 
}

/**
 * get id from key id table
 */
static int
credential_storage_get_id(
    json_object* select_statements,
    const char* table,
    const char* key,
    char** id)
{
    int result;
    int sql_state;
    json_object* stmt_jobj;
    char* prepared_str;
    size_t prepared_str_len;
    sqlite3_stmt* statement;
    statement = NULL;
    prepared_str = NULL;
    prepared_str_len = 0;


    stmt_jobj = json_object_object_get(select_statements, table);
    result = stmt_jobj ? 0 : -1;
    if (result == 0) {
        result = credential_storage_get_statement_str_from_json(stmt_jobj,
            &prepared_str, &prepared_str_len);
    } else {
        result = -1;
    }
    if (result == 0) {
        sql_state = sqlite3_prepare_v2(
            credential_get_connection(),
            prepared_str,
            prepared_str_len,
            &statement, NULL); 
        result = sql_state == SQLITE_OK ? 0 : -1;
    }
    if (result == 0) {
        const char* key_0;
        key_0 = key ? key : "";
        sql_state = sqlite3_bind_text(
            statement, 1, 
            key_0, 
            strlen(key_0), NULL);
        result = sql_state == SQLITE_OK ? 0 : -1;
    }
    if (result == 0) {
        sql_state = sqlite3_step(statement);
        result = sql_state == SQLITE_ROW ? 0 : 1;
    }
    if (result == 0) {
        const char* id_0;
        size_t id_length;
        char* id_1;
        id_0 = sqlite3_column_text(statement, 0);
        result = id_0 ? 0 : -1;
        if (result == 0) {
            id_length = strlen(id_0);
            id_1 = credential_i_str_dup_0(id_0, id_length);
            result = id_1 ? 0 : -1;
        }
        if (result == 0) {
            *id = id_1;
        }
    }
    if (statement) {
        sqlite3_finalize(statement);
    }
 
    if (prepared_str) {
        credential_i_free(prepared_str);
    }
    return result;
}

/**
 * insert id into key id table
 */
static int
credential_storage_insert_id(
    json_object* insert_statement_jobj,
    const char* key,
    const char* id)
{
    int result;
    int sql_state;
    char* prepared_str;
    size_t prepared_str_len;
    sqlite3_stmt* statement;
    prepared_str = NULL;
    prepared_str_len = 0;
    statement = NULL;
    result = credential_storage_get_statement_str_from_json(
        insert_statement_jobj,
        &prepared_str, &prepared_str_len);
    if (result == 0) {
        sql_state = sqlite3_prepare_v2(
            credential_get_connection(),
            prepared_str,
            prepared_str_len,
            &statement, NULL); 
        result = sql_state == SQLITE_OK ? 0 : -1;
    }
    if (result == 0) {
        const char* str_values[] = {
            key ? key : "",
            id ? id : ""
        };
        size_t idx;
        for (idx = 0; idx < sizeof(str_values) / sizeof(str_values[0]); idx++) {
            sql_state = sqlite3_bind_text(
                statement, idx + 1,
                str_values[idx], 
                strlen(str_values[idx]),
                NULL);

            result = sql_state == SQLITE_OK ? 0 : -1;
            if (result) {
                break;
            }
        }
    }

    if (result == 0) {
        sql_state = sqlite3_step(statement);
        result = sql_state == SQLITE_DONE ? 0 : -1;
    }
    if (statement) {
        sqlite3_finalize(statement);
    }
 
    if (prepared_str) {
        credential_i_free(prepared_str);
    }
    return result;
}



/**
 * remove password 
 */
int
credential_storage_remove_password(
    const char* protocol,
    const char* host,
    const char* path,
    const char* name)
{
    int result;
    json_object* app_config;
    json_object* select_stmts_jobj;
    struct {
        const char* table;
        const char* key;
        char* id;
    } table_key_id[] = {
        { 
            "protocol", protocol, NULL 
        },
        {
            "host", host, NULL
        },
        {
            "path", path, NULL
        },
        {
            "user", name, NULL
        }
    };
    result = 0;
    app_config = app_config_get(); 
    result = app_config ? 0 : -1;
    if (result == 0) { 
        select_stmts_jobj = credential_storage_get_json(app_config,
            "db", "select", "statements", NULL);
        result = select_stmts_jobj ? 0 : -1;
    }
    if (result == 0) {
        size_t i;
        for (i = 0; i < sizeof(table_key_id) / sizeof(table_key_id[0]); i++) {
            result = credential_storage_get_id(
                select_stmts_jobj, table_key_id[i].table, table_key_id[i].key,
                &table_key_id[i].id);
            if (result) {
                break;
            }
        }
    }
    if (result == 0) {
        result = credential_storage_remove_password_by_id(
            table_key_id[0].id, table_key_id[1].id,
            table_key_id[2].id, table_key_id[3].id);
    }
    if (result == 0) {
        credentail_storage_update_remove_log();
        credential_storage_gc();
    }
    {
        size_t i;
        for (i = 0; i < sizeof(table_key_id) / sizeof(table_key_id[0]); i++) {
            if (table_key_id[i].id) {
                credential_i_free(table_key_id[i].id);
            }
        }
    }
    if (app_config) {
        json_object_put(app_config);
    } 
    if (result == 1) {
        result = 0;
    }
    return result;
}

/**
 * remove password by id parameters
 */
static int
credential_storage_remove_password_by_id(
    const char* protocol_id,
    const char* host_id,
    const char* path_id,
    const char* name_id)
{
    int result;
    json_object* app_config;
    json_object* remove_stmts_jobj;
    sqlite3_stmt* statement;
    char* prepared_str;
    size_t prepared_str_len;
    int sql_state;
    result = 0;
    statement = NULL;
    prepared_str = NULL;
    prepared_str_len = 0;
    app_config = app_config_get(); 
    result = app_config ? 0 : -1;
    if (result == 0) { 
        remove_stmts_jobj = credential_storage_get_json(app_config,
            "db", "remove", "password", NULL);
        result = remove_stmts_jobj ? 0 : -1;
    }
    if (result == 0) {
        result = credential_storage_get_statement_str_from_json(
            remove_stmts_jobj, &prepared_str, &prepared_str_len);
    }
    if (result == 0) {
        sql_state = sqlite3_prepare_v2(
            credential_get_connection(),
            prepared_str,
            prepared_str_len,
            &statement, NULL); 
        result = sql_state == SQLITE_OK ? 0 : -1;
    }
    if (result == 0) {
        const char* values[] = {
            protocol_id ? protocol_id : "",
            host_id ? host_id : "",
            path_id ? path_id : "", 
            name_id ? name_id : ""
        };
        size_t i;
        for (i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
            sql_state = sqlite3_bind_text(
                statement, i + 1, 
                values[i], 
                strlen(values[i]),
                NULL);
            result = sql_state == SQLITE_OK ? 0 : -1;
            if (result) {
                break;
            }
        } 
    }
    if (result == 0) {
        sql_state = sqlite3_step(statement);
        result = sql_state == SQLITE_DONE ? 0 : -1;
    }
    if (statement) {
        sqlite3_finalize(statement);
    }
    if (prepared_str) {
        credential_i_free(prepared_str); 
    }
    if (app_config) {
        json_object_put(app_config);
    } 
    return result;     
}




/**
 * run gabage collection for database
 */
int
credential_storage_gc()
{
    int result;
    int need_to_gc;
    result = 0;
    need_to_gc = 0;
    result = credential_storage_need_to_gc(&need_to_gc);
    if (result == 0) {
        if (need_to_gc) {
            result = credetial_storage_do_gc();
        }
    }
    return result;
}


/**
 * do gabase collection.
 */
int
credetial_storage_do_gc()
{
    int result;
    json_object* app_config;
    json_object* gc_stmts_jobj;
    result = 0;
    app_config = app_config_get(); 
    result = app_config ? 0 : -1;
    if (result == 0) { 
        gc_stmts_jobj = credential_storage_get_json(app_config,
            "db", "gc", NULL);
        result = gc_stmts_jobj ? 0 : -1;
    }
    if (result == 0) {
        struct json_object_iterator iter;
        struct json_object_iterator end_iter;
        iter = json_object_iter_init_default();
        end_iter = json_object_iter_end(gc_stmts_jobj);
        for (iter = json_object_iter_begin(gc_stmts_jobj);
            !json_object_iter_equal(&iter, &end_iter);
            json_object_iter_next(&iter)) {
            json_object* stmt_jobj;
            stmt_jobj = json_object_iter_peek_value(&iter);
            if (stmt_jobj) {
                char* prepared_str;
                size_t prepared_str_len;
                sqlite3_stmt* statement;
                int sql_state;
                statement = NULL;
                prepared_str = NULL;
                result = credential_storage_get_statement_str_from_json(
                    stmt_jobj, &prepared_str, &prepared_str_len);
                if (result == 0) {
                    sql_state = sqlite3_prepare_v2(
                    credential_get_connection(),
                    prepared_str,
                    prepared_str_len,
                    &statement, NULL); 
                    result = sql_state == SQLITE_OK ? 0 : -1;
                }
                if (result == 0) {
                    sql_state = sqlite3_step(statement);
                    result = sql_state == SQLITE_DONE ? 0 : -1;
                } 
                if (statement) {
                    sqlite3_finalize(statement);
                }
                if (prepared_str) {
                    credential_i_free(prepared_str);
                }
                if (result) {
                    break;
                }
            }
        }
    }
    if (app_config) {
        json_object_put(app_config);
    } 
    return result;     
}

/**
 * you get a flag to run gc or not.
 */
int
credential_storage_need_to_gc(
    int* need_to_gc)
{
    int result;
    json_object* app_config;
    json_object* gc_stmts_jobj;
    sqlite3_stmt* statement;
    char* prepared_str;
    size_t prepared_str_len;
    int sql_state;
    result = 0;
    statement = NULL;
    prepared_str = NULL;
    prepared_str_len = 0;
    app_config = app_config_get(); 
    result = app_config ? 0 : -1;
    if (result == 0) { 
        gc_stmts_jobj = credential_storage_get_json(app_config,
            "db", "remove", "gc-condition", NULL);
        result = gc_stmts_jobj ? 0 : -1;
    }
    if (result == 0) {
        result = credential_storage_get_statement_str_from_json(
            gc_stmts_jobj, &prepared_str, &prepared_str_len);
    }
    if (result == 0) {
        sql_state = sqlite3_prepare_v2(
            credential_get_connection(),
            prepared_str,
            prepared_str_len,
            &statement, NULL); 
        result = sql_state == SQLITE_OK ? 0 : -1;
    }
    if (result == 0) {
        sql_state = sqlite3_step(statement);
        result = sql_state == SQLITE_ROW ? 0 : -1;
    }
    if (result == 0) {
        int count;
        count = sqlite3_column_int(statement, 0);
        *need_to_gc = count > 0;
    }
    if (statement) {
        sqlite3_finalize(statement);
    }
 
    if (prepared_str) {
        credential_i_free(prepared_str); 
    }
 
    if (app_config) {
        json_object_put(app_config);
    } 
    return result;
}

/**
 * update remove log
 */
static int
credentail_storage_update_remove_log()
{
    int result;
    json_object* app_config;
    json_object* statement_jobj;
    char* statement_str;
    size_t statement_str_len;
    int sql_state;
    sqlite3_stmt* statement;
     
    statement_jobj = NULL;
    statement_str = NULL;
    statement_str_len = 0;
    sql_state = 0;
    result = 0;
    statement = NULL;
    if (result == 0) {
        app_config = app_config_get(); 
        result = app_config ? 0 : -1;
    }
    if (result == 0) {
        statement_jobj = credential_storage_get_json(app_config,
            "db", "remove", "log", NULL);
        result = statement_jobj ? 0 : -1;
    } 
    if (result == 0) {
        result = credential_storage_get_statement_str_from_json(statement_jobj,
            &statement_str, &statement_str_len);
    }
    if (result == 0) {
        sql_state = sqlite3_prepare_v2(
            credential_get_connection(),
            statement_str,
            statement_str_len,
            &statement, NULL); 
        result = sql_state == SQLITE_OK ? 0 : -1;
    }
    if (result == 0) {
        sql_state = sqlite3_step(statement);
        result = sql_state == SQLITE_OK ? 0 : -1;
    }
    if (statement) {
        sqlite3_finalize(statement);
    }
    if (statement_str) {
        credential_i_free(statement_str);
    }
    if (app_config) {
        json_object_put(app_config);
    } 
}


/**
 * free heap object from this module
 */
void
credential_storage_free_object(
    void* obj)
{
    credential_i_free(obj);
}

/**
 * get json object 
 */
static json_object*
credential_storage_get_json(
    json_object* root, ...)
{
    va_list argp;
    json_object* result;
    json_object* parent_obj;
    result = NULL;
    parent_obj = root;
    va_start(argp, root); 
    
    while (1) {
        const char* jsname;
        jsname = va_arg(argp, const char*);
        if (jsname) {
            json_object* child_obj;   
            child_obj = json_object_object_get(parent_obj, jsname);
            if (child_obj) {
                parent_obj = child_obj;
            } else {
                break;
            }
        } else {
            result = parent_obj; 
            break;
        } 
    }
    va_end(argp);
    return result;
}

/**
 * get statement string from json object
 */
static int
credential_storage_get_statement_str_from_json(
    json_object* statement_arry,
    char** statement,
    size_t* statement_length)
{
    int result;
    cstr* statement_buffer;
    statement_buffer = credential_storage_create_cstr();
    result = statement ? 0 : -1;
    if (result == 0) {
        size_t idx;
        for (idx = 0;
            idx < json_object_array_length(statement_arry); idx++) {
            json_object* line;
            const char* line_0;
            line = json_object_array_get_idx(statement_arry, idx);
            line_0 = json_object_get_string(line);
            result = cstr_append(statement_buffer, line_0); 
            if (result) {
                break;
            }
        }
    }
    if (result == 0) {
        char* statement_str;
        statement_str = cstr_to_flat_str(statement_buffer);
        if (statement_str) {
            *statement = statement_str;
            *statement_length = cstr_length(statement_buffer); 
        } else {
            result = -1;
        }
    }
    if (statement_buffer) {
        cstr_release(statement_buffer);
    }
    return result;
} 

/**
 * initialize table
 */
static int
credential_storage_init_table_statement(
    json_object* statement_jobj)
{
    int result;
    int sql_state;
    json_object* name_value_array_jobj;
    json_object* condition_jobj;
    json_object* statement_run_jobj;
    sqlite3_stmt* condition_stmt;
    sqlite3_stmt* statement_run_stmt;
    char* condition_str;
    size_t condition_str_len;
    char* statement_run_str;
    size_t statement_run_str_len;

    condition_stmt = NULL;
    statement_run_stmt = NULL;
    condition_str = NULL;
    statement_run_str = NULL;
     
    name_value_array_jobj = json_object_object_get(
        statement_jobj, "name-value"); 
    result = name_value_array_jobj ? 0 : -1;
    if (result == 0) {
        condition_jobj = json_object_object_get(statement_jobj, "condition"); 
        result = condition_jobj ? 0 : -1;
    }
    if (result == 0) {
        result = credential_storage_get_statement_str_from_json(
            condition_jobj, &condition_str, &condition_str_len);
    }
    if (result == 0) {
        sql_state = sqlite3_prepare_v2(
            credential_get_connection(),
            condition_str,
            condition_str_len,
            &condition_stmt, NULL); 

        
        result = sql_state == SQLITE_OK ? 0 : -1;
    }
 
    if (result == 0) {
        statement_run_jobj = json_object_object_get(
            statement_jobj, "statement"); 
        result = statement_run_jobj ? 0 : -1;
    }
    if (result == 0) {
        result = credential_storage_get_statement_str_from_json(
            statement_run_jobj, &statement_run_str, &statement_run_str_len);
    }
    if (result == 0) {
        sql_state = sqlite3_prepare_v2(
            credential_get_connection(),
            statement_run_str,
            statement_run_str_len,
            &statement_run_stmt, NULL);
        result = sql_state == SQLITE_OK ? 0 : -1;
    }
     
    if (result == 0) { 
        size_t idx;
        for (idx = 0;
            idx < json_object_array_length(name_value_array_jobj); idx++) {
            json_object* name_value_jobj;
            json_object* name_jobj;
            json_object* value_jobj;
            name_value_jobj = json_object_array_get_idx(
                name_value_array_jobj, idx);
            result = name_value_jobj ? 0 : -1;
            
            if (result == 0) {
                name_jobj = json_object_array_get_idx(
                    name_value_jobj, 0);
                result = name_jobj ? 0 : -1;
            }
            if (result == 0) {
                value_jobj = json_object_array_get_idx(
                    name_value_jobj, 1);

                result = value_jobj ? 0 : -1;
            }
            if (result == 0) {
                const char* name;
                name = json_object_get_string(name_jobj);
                result = name ? 0 : -1;
                if (result == 0) {
                    sql_state = sqlite3_bind_text(
                        condition_stmt, 1, name, 
                        json_object_get_string_len(name_jobj), NULL);
                    result = sql_state == SQLITE_OK ? 0 : -1;
                }
                
                if (result == 0) {
                    sql_state = sqlite3_step(condition_stmt);
                    result = sql_state == SQLITE_ROW ? 0 : -1;
                }
                if (result == 0) {
                    int cnd;
                    cnd = sqlite3_column_int(condition_stmt, 0);
                    if (!cnd) {
                        sql_state = sqlite3_bind_text(
                            statement_run_stmt, 1, name, 
                            json_object_get_string_len(name_jobj), NULL);
                        result = sql_state == SQLITE_OK ? 0 : -1;
                        if (result == 0) {
                            sql_state = sqlite3_bind_int(
                                statement_run_stmt, 2, 
                                json_object_get_int(value_jobj));
                            result = sql_state == SQLITE_OK ? 0 : -1;
                        }
                        if (result == 0) {
                            sql_state = sqlite3_step(statement_run_stmt);
                            result = sql_state == SQLITE_DONE ? 0 : -1;
                        }
                        sqlite3_reset(statement_run_stmt);
                    }
                }
                sqlite3_reset(condition_stmt);
            }
            if (result) {
                break;
            }
        }
    }
    if (statement_run_stmt) {
        sqlite3_finalize(statement_run_stmt);
    }
    if (condition_stmt) {
        sqlite3_finalize(condition_stmt);
    }
    if (statement_run_str) {
        credential_i_free(statement_run_str);
    }
    if (condition_str) {
        credential_i_free(condition_str);
    }
      
    return result;
}

/**
 * initialize db
 */
static int
credential_storage_init_db_statement(
    json_object* statement_arry)
{
    int result;
    int sql_state;
    char* statement_str;
    size_t statement_str_len;
    statement_str = NULL;
    statement_str_len = 0;
    result = credential_storage_get_statement_str_from_json(
        statement_arry, &statement_str, &statement_str_len);
    if (result == 0) {
        sql_state = sqlite3_exec(
            credential_get_connection(),
            statement_str, NULL, NULL, NULL);
        result = sql_state == SQLITE_OK ? 0 : -1;
    }
    if (statement_str) {
        credential_i_free(statement_str);
    }
    return result;
}
/**
 * initialize db
 */
static int
credential_storage_init_db()
{
    int result;
    json_object* app_config;
    json_object* statements_arry_jobj;
    statements_arry_jobj = NULL;
    result = 0;
    app_config = app_config_get(); 
    result = app_config ? 0 : -1;
    if (result == 0) {
        statements_arry_jobj = credential_storage_get_json(app_config,
            "db", "init", "statements", NULL);
        result = statements_arry_jobj ? 0 : -1;
    } 
    if (result == 0) {
        size_t idx;
        for (idx = 0;
            idx < json_object_array_length(statements_arry_jobj); idx++) {
            json_object* statements;
            statements = json_object_array_get_idx(
                statements_arry_jobj, idx);
            result = credential_storage_init_db_statement(statements);
            if (result) {
                break;
            }
        }
    }
    if (app_config) {
        json_object_put(app_config);
    } 
    return result;
}

/**
 * init tables
 */
static int
credential_storage_init_tables()
{
    int result;
    json_object* app_config;
    json_object* statements_arry_jobj;
    statements_arry_jobj = NULL;
    app_config = app_config_get(); 
    result = app_config ? 0 : -1;
    if (result == 0) {
        statements_arry_jobj = credential_storage_get_json(app_config,
            "db", "init-tables", "statements", NULL);
        result = statements_arry_jobj ? 0 : -1;
    } 
    if (result == 0) {
        size_t idx;
        for (idx = 0;
            idx < json_object_array_length(statements_arry_jobj); idx++) {
            json_object* statements;
            statements = json_object_array_get_idx(statements_arry_jobj, idx);
            result = credential_storage_init_table_statement(statements);
            if (result) {
                break;
            }
        }
    }
    if (app_config) {
        json_object_put(app_config);
    } 
    return result;
}

#ifndef UUID_STR_LEN
#define UUID_STR_LEN 37
#endif

/**
 * generate uuid
 */
static int
credential_storage_generate_uuid(
    char** uuid)
{
    uuid_t uuid_0;
    char* uuid_str;
    int result;
    result = 0;
    uuid_str = (char*)credential_i_alloc(UUID_STR_LEN);
    result = uuid_str ? 0 : -1;
    if (result == 0) {
        uuid_generate(uuid_0);
        uuid_unparse(uuid_0, uuid_str);
        *uuid = uuid_str;
    }
    return result; 
}

/**
 * database connection
 */
static sqlite3* db_connection;

/**
 * get database connection
 */
sqlite3*
credential_get_connection()
{
    return db_connection;
}

/**
 * initialize this module 
 */
int
credential_storage_start()
{
    int result;
    int sql_state;
    char* cred_data_path;
    result = 0;
    cred_data_path = user_resource_get_credential_data_path();
    result = cred_data_path ? 0 : -1;
    if (result == 0) {
        sql_state = sqlite3_initialize();
        result = sql_state == SQLITE_OK ? 0 : -1;
        if (result == 0) {
            result = user_resource_create_credential_data_directory_if_not();
        }
        if (result == 0) { 
            sql_state = sqlite3_open(cred_data_path, &db_connection);
            result = sql_state == SQLITE_OK ? 0 : -1;
        }

        if (result == 0) {
            result = credential_storage_init_db();
        }
        if (result == 0) {
            result = credential_storage_init_tables();
        }
        if (result) {
            sqlite3_shutdown();
        }
    }
    
        
    if (cred_data_path) {
        user_resource_free(cred_data_path);
    }
    return result;
}

/**
 * tear down this module 
 */
int
credential_storage_stop()
{
    int result;
    result = 0;

    if (db_connection) {
        sqlite3_close(db_connection);
        db_connection = NULL;
    }
    result = sqlite3_shutdown();
    return result;
}

/**
 * create buffer for internal use
 */
static cstr*
credential_storage_create_cstr()
{
    cstr* result;
    result = cstr_create_01(
        credential_i_alloc_1,
        credential_i_free);
    return result;
}


/* vi: se ts=4 sw=4 et: */

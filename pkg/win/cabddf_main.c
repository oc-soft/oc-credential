#include <windows.h>
#include <wchar.h>
#include <stdio.h>
#include <string.h>
#include <msiquery.h>
#include <pathcch.h>
#include "col/array_list.h"

/**
 * cab ddf file generator option
 */
typedef struct _cabddf_option cabddf_option;

/**
 * cab ddf file entry
 */
typedef struct _cabddf_file_entry cabddf_file_entry;


/**
 * cab ddf file generator option
 */
struct _cabddf_option {
    /**
     * show help message
     */
    int help;

    /**
     * msi file path
     */
    wchar_t* msi_path;
};

/**
 * cab ddf file entry
 */ 
struct _cabddf_file_entry {
    /**
     * reference count
     */
    unsigned int ref_count; 

    /**
     * file path
     */
    LPWSTR file_path;

    /**
     * file name
     */
    LPWSTR file_name_id;

    /**
     * sequential number
     */
    int seq;
};

/**
 * create file entry
 */
cabddf_file_entry*
cabddf_file_entry_create();

/**
 * create file entry
 */
cabddf_file_entry*
cabddf_file_entry_create_1(
    LPCWSTR file_name_id,
    LPCWSTR file_path,
    int seq);

/**
 * increment reference count
 */
unsigned int
cabddf_file_entry_retain(
    cabddf_file_entry* obj);

/**
 * release file entry
 */
unsigned int
cabddf_file_entry_release(
    cabddf_file_entry* obj);

/**
 * release file entry
 */
void
cabddf_file_entry_release_1(
    cabddf_file_entry* obj);

/**
 * get hash
 */
int
cabddf_file_entry_get_hash(
    cabddf_file_entry* obj);

/**
 * copy reference
 */
int
cabddf_file_entry_copy_reference(
    cabddf_file_entry* src,
    cabddf_file_entry** dst);

/**
 * get file name id
 */
LPCWSTR
cabddf_file_entry_get_file_name_id_ref(
    cabddf_file_entry* obj);


/**
 * get file name id
 */
LPWSTR
cabddf_file_entry_get_file_name_id(
    cabddf_file_entry* obj);

/**
 * set file name id 
 */
int
cabddf_file_entry_set_file_name_id(
    cabddf_file_entry* obj,
    LPCWSTR file_name_id);


/**
 * get file path
 */
LPCWSTR
cabddf_file_entry_get_file_path_ref(
    cabddf_file_entry* obj);


/**
 * get file path
 */
LPWSTR
cabddf_file_entry_get_file_path(
    cabddf_file_entry* obj);

/**
 * set file path
 */
int
cabddf_file_entry_set_file_path(
    cabddf_file_entry* obj,
    LPCWSTR file_name_id);

/**
 * get file name id
 */
LPWSTR
cabddf_file_entry_get_file_name_id(
    cabddf_file_entry* obj);

/**
 * set file name id 
 */
int
cabddf_file_entry_set_file_name_id(
    cabddf_file_entry* obj,
    LPCWSTR file_name_id);

/**
 * get sequence
 */
int
cabddf_file_entry_get_sequence(
    cabddf_file_entry* obj);

/**
 * set sequence
 */
int
cabddf_file_entry_set_sequence(
    cabddf_file_entry* obj,
    int seq);



/**
 * parse option
 */
static int
parse_option(
    int argc,
    wchar_t** argv,
    cabddf_option* option);

/**
 * show help message
 */
static int
show_help();

/**
 * read string from record
 */
static LPWSTR
read_str_from_record(
    MSIHANDLE rec_hdl,
    UINT field);

/**
 * allocate memory
 */
static void*
cabddf_alloc_mem(
    size_t size);

/**
 * free memory
 */
static void
cabddf_free_mem(
    void* obj);

/**
 * duplicate string
 */
static LPWSTR
cabddf_strdupw(
    LPCWSTR srcstr);

/**
 * join path
 */
static LPWSTR
join_path(
    LPCWSTR path1,
    LPCWSTR path2);

/**
 * get source directory
 */
static LPCWSTR
get_source_dir_from_default_dir(
    LPCWSTR default_dir);

/**
 * run main program
 */
static int
run(
    const cabddf_option* option);

/**
 * load file entries
 */
static int
load_file_entries(
    MSIHANDLE db_hdl,
    col_list* file_entries);

/**
 * output file entries
 */
static int
output_file_entries(
    MSIHANDLE db_hdl,
    col_list* file_entries);

/**
 * output file entries
 */
static int
output_file_entries_with_range(
    col_list* file_entries,
    int start_idx, int end_idx);

/**
 * output file entry
 */
static int
output_file_entry(
    cabddf_file_entry* entry);

/**
 * read source directory
 */
static LPWSTR
read_src_dir(
    MSIHANDLE db_hdl,
    LPCWSTR start_dir_key);

/**
 * entry point
 */
int
wmain(
    int argc,
    wchar_t** argv)
{
    cabddf_option option;
    int result;
    memset(&option, 0, sizeof(option));
    result = 0;

    result = parse_option(argc, argv, &option);

    if (!option.help) {
        result = run(&option);
    } else {
        show_help();
    }

    return result;
}


/**
 * run main program
 */
int
run(
    const cabddf_option* option)
{
    int result;
    UINT u_state;
    MSIHANDLE db_hdl;
    col_list* file_entries;
    result = 0;
    db_hdl = 0;
    u_state = ERROR_SUCCESS;
    file_entries = NULL;
    if (option->msi_path) {
        u_state = MsiOpenDatabaseW(
            option->msi_path, (LPCWSTR)MSIDBOPEN_TRANSACT, &db_hdl);
        result = u_state == ERROR_SUCCESS ? 0 : -1;
    }
    if (result == 0) {
        file_entries = col_array_list_create(10, 10,
            (int (*)(void*))
                cabddf_file_entry_get_hash,
            (int (*)(const void*, void **))
                cabddf_file_entry_copy_reference,
            (void (*)(void*)) 
                cabddf_file_entry_release_1);
        result = file_entries ? 0 : -1;
    }
    if (result == 0) {
        result = load_file_entries(db_hdl, file_entries);
    }
    if (result == 0) {
        result = output_file_entries(db_hdl, file_entries);
    }
 
    if (file_entries) {
        col_list_free(file_entries);
    }

    if (db_hdl) {
        MsiCloseHandle(db_hdl);
    }
    return result;
}


/**
 * load file entries
 */
int
load_file_entries(
    MSIHANDLE db_hdl,
    col_list* file_entries)
{
    int result;
    MSIHANDLE view_hdl;
    col_list* file_names;
    UINT u_state;
    result = 0;
    view_hdl = 0;

    if (result == 0) {
        u_state = MsiDatabaseOpenViewW(db_hdl,
            L"SELECT `File`.`File`, `File`.`FileName`, "
            L" `Directory`.`Directory`, `File`.`Sequence` "
            L"FROM `File`, `Component`, `Directory` "
            L"WHERE `File`.`Component_`=`Component`.`Component` "
            L" AND `Component`.`Directory_`=`Directory`.`Directory` "
            L"ORDER BY `File`.`Sequence`",
            &view_hdl);
        result = u_state == ERROR_SUCCESS ? 0 : -1;
    }
    if (result == 0) {
        int i_status;
        u_state = MsiViewExecute(view_hdl, 0);

        i_status = u_state == ERROR_SUCCESS ? 0 : -1;
        result = i_status;

        while (i_status == 0) {
            MSIHANDLE rec_hdl;
            LPWSTR file_name;
            LPWSTR dir_key;
            LPWSTR file_key;
            LPWSTR src_dir_path;
            LPWSTR src_path;
            cabddf_file_entry* entry;
            int seq_num;
            rec_hdl = 0;
            file_name = NULL;
            dir_key = NULL;
            file_key = NULL;
            src_dir_path = NULL;
            src_path = NULL;
            entry = NULL;
            seq_num = MSI_NULL_INTEGER;

            if (i_status == 0) {
                u_state = MsiViewFetch(view_hdl, &rec_hdl);

                if (u_state == ERROR_SUCCESS) {
                    i_status = 0;
                    result = 0;
                } else if (u_state == ERROR_NO_MORE_ITEMS) {
                    i_status = -1;
                    result = 0;
                } else {
                    i_status = -1;
                    result = -1;
                }
            } 
            if (i_status == 0) {
                file_key = read_str_from_record(rec_hdl, 1);
                i_status = file_key ? 0 : -1;
                result = i_status;
            }
            if (i_status == 0) {
                file_name = read_str_from_record(rec_hdl, 2);
                i_status = file_name ? 0 : -1;
                result = i_status;
            }
            if (i_status == 0) {
                dir_key = read_str_from_record(rec_hdl, 3);
                i_status = dir_key ? 0 : -1;
                result = i_status;
            }
            if (i_status == 0) {
                seq_num = MsiRecordGetInteger(rec_hdl, 4);
                i_status = seq_num != MSI_NULL_INTEGER ? 0 : -1;
                result = i_status;
            }
            if (i_status == 0) {
                src_dir_path = read_src_dir(db_hdl, dir_key);
                i_status = src_dir_path ? 0 : -1;
                result = i_status;
            }
            if (i_status == 0) {
                src_path = join_path(src_dir_path, file_name);
                i_status = src_path ? 0 : -1;
                result = i_status;
            }

            if (i_status == 0) {
                entry = cabddf_file_entry_create_1(
                    file_key, src_path, seq_num);
                i_status = entry ? 0 : -1;
                result = i_status;
            }
            if (i_status == 0) {
                i_status = col_list_append(file_entries, entry);
                result = i_status;
            }
            if (entry) {
                cabddf_file_entry_release(entry);
            }

            if (src_path) {
                cabddf_free_mem(src_path);
            }

            if (src_dir_path) {
                cabddf_free_mem(src_dir_path);
            }
            
            if (file_name) {
                cabddf_free_mem(file_name);
            }

            if (dir_key) {
                cabddf_free_mem(dir_key);
            }
            if (file_key) {
                cabddf_free_mem(file_key);
            }
 
            if (rec_hdl) {
                MsiCloseHandle(rec_hdl);
            }
        }
    }

    if (view_hdl) {
        MsiViewClose(view_hdl);
    }

    return result;
}

/**
 * read source directory
 */
static LPWSTR
read_src_dir(
    MSIHANDLE db_hdl,
    LPCWSTR start_dir_key)
{
    UINT u_status;
    MSIHANDLE view_hdl;
    MSIHANDLE param_rec_hdl;
    int status;
    LPWSTR result;
    u_status = ERROR_SUCCESS;
    param_rec_hdl = 0;
    view_hdl = 0;
    status = 0;
    result = NULL;
    param_rec_hdl = MsiCreateRecord(1);
    status = param_rec_hdl ? 0 : -1;
    if (status == 0) {
        u_status = MsiDatabaseOpenViewW(db_hdl,
            L"SELECT " 
            L"`Directory`.`DefaultDir`, "
            L" `Directory`.`Directory_Parent` "
            L"FROM `Directory` "
            L"WHERE `Directory`.`Directory`=?",
            &view_hdl);
        status = view_hdl ? 0 : -1;
    }
    if (status == 0) {
        LPWSTR dir_path;
        LPWSTR dir_key;
        dir_path = NULL;
        dir_key = cabddf_strdupw(start_dir_key); 
        status = dir_key ? 0 : -1;
        if (status == 0) {
            do {
                MSIHANDLE res_rec_hdl;
                LPWSTR default_dir;
                LPWSTR parent_key;
                parent_key = NULL;
                default_dir = NULL;
                res_rec_hdl = 0;
                u_status = MsiRecordSetStringW(param_rec_hdl, 1, dir_key); 
               
                status = u_status == ERROR_SUCCESS ? 0 : -1;

                if (status == 0) {
                    u_status = MsiViewExecute(view_hdl, param_rec_hdl);
                    status = u_status == ERROR_SUCCESS ? 0 : -1;
                }
                if (status == 0) {
                    u_status = MsiViewFetch(view_hdl, &res_rec_hdl);

                    status = u_status == ERROR_SUCCESS ? 0 : -1;
                }
                if (status == 0) {
                    default_dir = read_str_from_record(res_rec_hdl, 1);

                    if (default_dir) {
                        LPCWSTR parent_dir_path;
                        parent_dir_path = get_source_dir_from_default_dir(
                                default_dir);
                        if (wcscmp(L"SourceDir", parent_dir_path) == 0) {
                            parent_dir_path = L".";
                        }
                        if (dir_path) {
                            LPWSTR tmp_dir_path;
                            tmp_dir_path = join_path(parent_dir_path, dir_path);
                            cabddf_free_mem(dir_path);
                            dir_path = tmp_dir_path;
                        } else {
                            dir_path = cabddf_strdupw(parent_dir_path);
                        }
                    }
                }
                if (status == 0) {
                    parent_key = read_str_from_record(res_rec_hdl, 2);
                    if (parent_key) {
                        cabddf_free_mem(dir_key);
                        dir_key = parent_key;
                        parent_key = NULL;
                    } else {
                        dir_key = NULL;
                    }
                }

                
                if (default_dir) {
                    cabddf_free_mem(default_dir);
                }
                if (parent_key) {
                    cabddf_free_mem(parent_key);
                }

                if (res_rec_hdl) {
                    MsiCloseHandle(res_rec_hdl);
                }
                if (status) {
                    break;
                }
            } while (dir_key);
        }
        result = dir_path;
    }
    if (view_hdl) {
        MsiViewClose(view_hdl);
    }
    if (param_rec_hdl) {
        MsiCloseHandle(param_rec_hdl);
    }
    return result;
}

/**
 * output file entries
 */
static int
output_file_entries(
    MSIHANDLE db_hdl,
    col_list* file_entries)
{
    UINT u_status;
    MSIHANDLE view_hdl;
    int result;
    u_status = ERROR_SUCCESS;
    view_hdl = 0;
    result = 0;
    u_status = MsiDatabaseOpenViewW(db_hdl,
        L"SELECT " 
        L"`Media`.`LastSequence` "
        L"FROM `Media` "
        L"ORDER BY `LastSequence`",
        &view_hdl);
    result = view_hdl ? 0 : -1;
    if (result == 0) {
        u_status = MsiViewExecute(view_hdl, 0);
        result = u_status == ERROR_SUCCESS ? 0 : -1;
    }
    if (result == 0) {
        int last_list_idx;
        int i_status;
        last_list_idx = -1;
        i_status = 0;
        while (i_status == 0) {
            MSIHANDLE res_rec_hdl;
            int end_idx;
            end_idx = -1;
            res_rec_hdl = 0;
            if (i_status == 0) {
                u_status = MsiViewFetch(view_hdl, &res_rec_hdl);
                i_status = u_status == ERROR_SUCCESS ? 0 : -1;
                if (u_status == ERROR_SUCCESS) {
                    i_status = 0;
                    result = 0;
                } else if (u_status == ERROR_NO_MORE_ITEMS) {
                    i_status = -1;
                    result = 0;
                } else {
                    i_status = -1;
                    result = -1;
                }
            }
            if (i_status == 0) {
                end_idx = MsiRecordGetInteger(res_rec_hdl, 1);
                i_status = end_idx != MSI_NULL_INTEGER ? 0 : -1;
                result = i_status;
            }
            if (i_status == 0) {
                i_status = output_file_entries_with_range(
                    file_entries,
                    last_list_idx + 1,
                    end_idx);
                result = i_status;
                last_list_idx = end_idx;
            }

            if (res_rec_hdl) {
                MsiCloseHandle(res_rec_hdl);
            }
        }
    }
    if (view_hdl) {
        MsiCloseHandle(view_hdl);
    }
    return result;
}


/**
 * output file entries
 */
static int
output_file_entries_with_range(
    col_list* file_entries,
    int start_idx, int end_idx)
{
    int idx;
    int result = 0;
    for (idx = start_idx; idx <= end_idx; idx++) {
        cabddf_file_entry* entry;
        entry = NULL;
        result = col_list_get(file_entries, idx, (void**)&entry);
        if (entry) {
            result = output_file_entry(entry);
        }
        if (entry) {
            cabddf_file_entry_release(entry);
        }
        if (result) {
            break;
        }
    }
    return result;
}

/**
 * output file entry
 */
static int
output_file_entry(
    cabddf_file_entry* entry)
{
    LPCWSTR src;
    LPCWSTR dst;
    int result;
    result = 0;
    src = cabddf_file_entry_get_file_path_ref(entry);
    result = src ? 0 : -1;
    if (result == 0) {
        dst = cabddf_file_entry_get_file_name_id_ref(entry);
        result = dst ? 0 : -1;
    }    
    if (result == 0) {
        wprintf(L"\"%ls\" \"%ls\"\n", src, dst);
    }
    return result;
}

/**
 * get source directory
 */
static LPCWSTR
get_source_dir_from_default_dir(
    LPCWSTR default_dir)
{
    LPCWSTR result;
    result = wcsstr(default_dir, L":");
    if (result) {
        result++;
    } else {
        result = default_dir;
    }
    return result;
}



/**
 * join path
 */
static LPWSTR
join_path(
    LPCWSTR path1,
    LPCWSTR path2)
{
    LPWSTR result;
    LPWSTR joined_path;
    HRESULT h_res;
    int status;
    status = 0;
    result = NULL;
    joined_path = NULL;
    h_res = PathAllocCombine(path1, path2, 0, &joined_path);

    status = SUCCEEDED(h_res) ? 0 : -1;
    if (status == 0) {
        size_t len;
        len = wcslen(joined_path);
        result = (LPWSTR)cabddf_alloc_mem((len + 1) * sizeof(WCHAR));
        if (result) {
            memcpy(result, joined_path, (len + 1) * sizeof(WCHAR));
        }
    }

    if (joined_path) {
        LocalFree(joined_path);
    }
    return result;
}

/**
 * read string from record
 */
static LPWSTR
read_str_from_record(
    MSIHANDLE rec_hdl,
    UINT field)
{
    WCHAR empty_buf[] = { L'\0' };
    DWORD buf_count;
    UINT state;
    LPWSTR result;
    buf_count = 0;
    result = NULL;
    if (!MsiRecordIsNull(rec_hdl, field)) {
        state = MsiRecordGetStringW(rec_hdl, field, empty_buf, &buf_count);

        if (state == ERROR_MORE_DATA) {
            result = (LPWSTR)cabddf_alloc_mem((buf_count + 1) * sizeof(WCHAR));
            if (result) {
                buf_count += 1;
                state = MsiRecordGetStringW(rec_hdl, field, result, &buf_count);
                if (state != ERROR_SUCCESS) {
                    cabddf_free_mem(result);
                    result = NULL;
                }
            }
        }
    }
    return result;
}

/**
 * duplicate string
 */
static LPWSTR
cabddf_strdupw(
    LPCWSTR srcstr)
{
    size_t len;
    LPWSTR result;
    result = NULL;
    if (srcstr) {
        len = wcslen(srcstr);
        result = (LPWSTR)cabddf_alloc_mem((len + 1) * sizeof(WCHAR));
        if (result) {
            memcpy(result, srcstr, (len + 1) * sizeof(WCHAR));
        }
    }
    return result;
}

/**
 * allocate memory
 */
static void*
cabddf_alloc_mem(
    size_t size)
{
    return malloc(size);
}

/**
 * free memory
 */
static void
cabddf_free_mem(
    void* obj)
{
    free(obj);
}

/**
 * show help message
 */
static int
show_help()
{
    int result;
    result = 0;
    wprintf(
L"cabddf [OPTIONS]\n"
L"-m            specify msi file\n"
L"-h            show this message.\n");
    return result;
}


/**
 * parse option
 */
static int
parse_option(
    int argc,
    wchar_t** argv,
    cabddf_option* option)
{
    int result;
    int idx;
    result = 0;
    for (idx = 0; idx < argc; idx++) {
        wchar_t* opt;
        wchar_t* next_opt;
        next_opt = NULL;
        opt = argv[idx];
        if (idx < argc - 1) {
            next_opt = argv[idx + 1];
        }
        if (wcscmp(L"-h", opt) == 0) {
            option->help = 1;
        } else if (wcscmp(L"-m", opt) == 0) {
            if (next_opt) {
                option->msi_path = next_opt;
                idx++;
            }
        }
    }
    return result; 
}


/**
 * create file entry
 */
cabddf_file_entry*
cabddf_file_entry_create()
{
    cabddf_file_entry* result;
    result = (cabddf_file_entry*)cabddf_alloc_mem(sizeof(cabddf_file_entry));
    if (result) {
        result->ref_count = 1;
        result->file_name_id = NULL;
        result->file_path = NULL;
        result->seq = 0;
    }
    return result;
}

/**
 * create file entry
 */
cabddf_file_entry*
cabddf_file_entry_create_1(
    LPCWSTR file_name_id,
    LPCWSTR file_path,
    int seq)
{
    cabddf_file_entry* result;
    result = cabddf_file_entry_create();
    if (result) {
        int state;
        state = cabddf_file_entry_set_file_name_id(result, file_name_id);
        if (state == 0) {
            cabddf_file_entry_set_file_path(result, file_path);
        }
        if (state == 0) {
            cabddf_file_entry_set_sequence(result, seq);
        }
        if (state) {
            cabddf_file_entry_release(result);
            result = NULL;
        }
    }
    return result;
}


/**
 * increment reference count
 */
unsigned int
cabddf_file_entry_retain(
    cabddf_file_entry* obj)
{
    unsigned int result;
    result = 0;
    if (obj) {
        result = ++obj->ref_count;
    }
    return result;
}

/**
 * release file entry
 */
unsigned int
cabddf_file_entry_release(
    cabddf_file_entry* obj)
{
    unsigned int result;
    result = 0;
    if (obj) {
        result = --obj->ref_count;
        if (result == 0) {
            cabddf_file_entry_set_file_name_id(obj, NULL);
            cabddf_file_entry_set_file_path(obj, NULL);
            cabddf_free_mem(obj);
        }
    }
    return result;
}

/**
 * release file entry
 */
void
cabddf_file_entry_release_1(
    cabddf_file_entry* obj)
{
    cabddf_file_entry_release(obj);
}

/**
 * get hash
 */
int
cabddf_file_entry_get_hash(
    cabddf_file_entry* obj)
{
    union {
        cabddf_file_entry* obj;
        int hash;
    } pointer_int;
    memset(&pointer_int, 0, sizeof(pointer_int));
    pointer_int.obj = obj;
    return pointer_int.hash;
}

/**
 * copy reference
 */
int
cabddf_file_entry_copy_reference(
    cabddf_file_entry* src,
    cabddf_file_entry** dst)
{
    int result;
    result = 0;
    if (src && dst) {
        cabddf_file_entry_retain(src);
    }
    if (dst) {
        *dst = src;
    }
    return result;
}

/**
 * get file name id
 */
LPCWSTR
cabddf_file_entry_get_file_name_id_ref(
    cabddf_file_entry* obj)
{
    LPCWSTR result;
    result = NULL;
    if (obj) {
        result = obj->file_name_id;
    }
    return result;
}



/**
 * get file name id
 */
LPWSTR
cabddf_file_entry_get_file_name_id(
    cabddf_file_entry* obj)
{
    LPWSTR result;
    result = NULL;
    if (obj) {
        result = cabddf_strdupw(obj->file_name_id);
    }
    return result;
}

/**
 * set file name id 
 */
int
cabddf_file_entry_set_file_name_id(
    cabddf_file_entry* obj,
    LPCWSTR file_name_id)
{
    unsigned int result;
    result = 0;
    if (obj) {
        if (obj->file_name_id != file_name_id) {
            if (obj->file_name_id) {
                cabddf_free_mem(obj->file_name_id);
            }
            obj->file_name_id = cabddf_strdupw(file_name_id);
        }
    }
    return result;
}


/**
 * get file path
 */
LPCWSTR
cabddf_file_entry_get_file_path_ref(
    cabddf_file_entry* obj)
{
    LPCWSTR result;
    result = NULL;
    if (obj) {
        result = obj->file_path;
    }
    return result;
}



/**
 * get file path
 */
LPWSTR
cabddf_file_entry_get_file_path(
    cabddf_file_entry* obj)
{
    LPWSTR result;
    result = NULL;
    if (obj) {
        result = cabddf_strdupw(obj->file_path);
    }
    return result;
}

/**
 * set file path 
 */
int
cabddf_file_entry_set_file_path(
    cabddf_file_entry* obj,
    LPCWSTR file_path)
{
    unsigned int result;
    result = 0;
    if (obj) {
        if (obj->file_path != file_path) {
            if (obj->file_path) {
                cabddf_free_mem(obj->file_path);
            }
            obj->file_path = cabddf_strdupw(file_path);
        }
    }
    return result;
}

/**
 * get sequence
 */
int
cabddf_file_entry_get_sequence(
    cabddf_file_entry* obj)
{
    int result;
    result = -1;
    if (obj) {
        result = obj->seq;
    }
    return result;
}

/**
 * set sequence
 */
int
cabddf_file_entry_set_sequence(
    cabddf_file_entry* obj,
    int seq)
{
    int result;
    if (obj) {
        result = 0;
        obj->seq = seq;
    } else {
        result = -1;
    }
    return result;
}



/* vi: se ts=4 sw=4 et: */

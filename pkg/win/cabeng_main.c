#include <windows.h>
#include <wchar.h>
#include <stdio.h>
#include <string.h>
#include <msiquery.h>
#include <pathcch.h>
#include <errno.h>
#include "col/array_list.h"
#include "col/rb_map.h"
#include "col/map.h"


/**
 * cab entry generator
 */
typedef struct _cabeng cabeng;

/**
 * cab ddf file generator option
 */
typedef struct _cabeng_option cabeng_option;

/**
 * cab ddf file entry
 */
typedef struct _cabeng_file_entry cabeng_file_entry;


/**
 * number object
 */
typedef struct _cabeng_number cabeng_number;

/**
 * cab entry generator
 */
struct _cabeng 
{
    /**
     * reference count
     */
    unsigned int ref_count;

    /**
     * entry list
     */
    col_list* entry_list;

    /**
     * sequence number entry map
     */
    col_map* seq_entry_map;


    /**
     * option
     */
    const cabeng_option* option;

};




/**
 * cab entry generator option
 */
struct _cabeng_option {
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
 * cab file entry
 */ 
struct _cabeng_file_entry {
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
 * number object
 */
struct _cabeng_number {

    /**
     * reference count
     */
    unsigned int ref_count;

    /**
     * number
     */
    int number;
};

/**
 * create entry generator
 */
cabeng*
cabeng_create();


/**
 * increment reference count
 */
unsigned int
cabeng_retain(
    cabeng* obj);

/**
 * decrement reference count
 */
unsigned int
cabeng_release(
    cabeng* obj);

/**
 * put file entry
 */
int
cabeng_put_entry(
    cabeng* obj,
    cabeng_file_entry* entry);

/**
 * get file entry
 */
int
cabeng_get_entry(
    cabeng* obj,
    int seq_num,
    cabeng_file_entry** entry);

/**
 * create file entry
 */
cabeng_file_entry*
cabeng_file_entry_create();

/**
 * create file entry
 */
cabeng_file_entry*
cabeng_file_entry_create_1(
    LPCWSTR file_name_id,
    LPCWSTR file_path,
    int seq);

/**
 * increment reference count
 */
unsigned int
cabeng_file_entry_retain(
    cabeng_file_entry* obj);

/**
 * release file entry
 */
unsigned int
cabeng_file_entry_release(
    cabeng_file_entry* obj);

/**
 * release file entry
 */
void
cabeng_file_entry_release_1(
    cabeng_file_entry* obj);

/**
 * get hash
 */
int
cabeng_file_entry_get_hash(
    cabeng_file_entry* obj);

/**
 * copy reference
 */
int
cabeng_file_entry_copy_reference(
    cabeng_file_entry* src,
    cabeng_file_entry** dst);

/**
 * get file name id
 */
LPCWSTR
cabeng_file_entry_get_file_name_id_ref(
    cabeng_file_entry* obj);


/**
 * get file name id
 */
LPWSTR
cabeng_file_entry_get_file_name_id(
    cabeng_file_entry* obj);

/**
 * set file name id 
 */
int
cabeng_file_entry_set_file_name_id(
    cabeng_file_entry* obj,
    LPCWSTR file_name_id);


/**
 * get file path
 */
LPCWSTR
cabeng_file_entry_get_file_path_ref(
    cabeng_file_entry* obj);


/**
 * get file path
 */
LPWSTR
cabeng_file_entry_get_file_path(
    cabeng_file_entry* obj);

/**
 * set file path
 */
int
cabeng_file_entry_set_file_path(
    cabeng_file_entry* obj,
    LPCWSTR file_name_id);

/**
 * get file name id
 */
LPWSTR
cabeng_file_entry_get_file_name_id(
    cabeng_file_entry* obj);

/**
 * set file name id 
 */
int
cabeng_file_entry_set_file_name_id(
    cabeng_file_entry* obj,
    LPCWSTR file_name_id);

/**
 * get sequence
 */
int
cabeng_file_entry_get_sequence(
    cabeng_file_entry* obj);

/**
 * set sequence
 */
int
cabeng_file_entry_set_sequence(
    cabeng_file_entry* obj,
    int seq);


/**
 * create number object
 */
cabeng_number*
cabeng_number_create(
    int number); 

/**
 * increment reference count
 */
unsigned int
cabeng_number_retain(
    cabeng_number* obj); 

/**
 * copy object
 */
int
cabeng_number_copy_ref(
    cabeng_number* src,
    cabeng_number** dst);

/**
 * decrement reference count
 */
unsigned int
cabeng_number_release(
    cabeng_number* obj); 

/**
 * decrement reference count
 */
void
cabeng_number_release_1(
    cabeng_number* obj); 

/**
 * get number
 */
int
cabeng_number_get_number(
    cabeng_number* obj);

/**
 * compare number
 */
int
cabeng_number_compare(
    cabeng_number* obj_1,
    cabeng_number* obj_2);

/**
 * parse option
 */
static int
parse_option(
    int argc,
    wchar_t** argv,
    cabeng_option* option);

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
cabeng_alloc_mem(
    size_t size);

/**
 * get long name from short|long name string
 */
static LPCWSTR
get_long_name(
    LPCWSTR short_long_name); 


/**
 * free memory
 */
static void
cabeng_free_mem(
    void* obj);

/**
 * duplicate string
 */
static LPWSTR
cabeng_strdupw(
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
    const cabeng_option* option);

/**
 * load file entries
 */
static int
load_file_entries(
    MSIHANDLE db_hdl,
    cabeng* entry_gen);

/**
 * output file entries
 */
static int
output_file_entries(
    MSIHANDLE db_hdl,
    cabeng* entry_gen);

/**
 * output file entries
 */
static int
output_file_entries_with_range(
    cabeng* entry_gen,
    int start_idx, int end_idx);

/**
 * output file entry
 */
static int
output_file_entry(
    cabeng_file_entry* entry,
    int flush_cabinet);

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
    cabeng_option option;
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
    const cabeng_option* option)
{
    int result;
    UINT u_state;
    MSIHANDLE db_hdl;
    cabeng* entry_gen;
    result = 0;
    db_hdl = 0;
    u_state = ERROR_SUCCESS;
    entry_gen = NULL;
    if (option->msi_path) {
        u_state = MsiOpenDatabaseW(
            option->msi_path, (LPCWSTR)MSIDBOPEN_TRANSACT, &db_hdl);
        result = u_state == ERROR_SUCCESS ? 0 : -1;
    }
    if (result == 0) {
        entry_gen = cabeng_create(option);
        result = entry_gen ? 0 : -1;
    }
    if (result == 0) {
        result = load_file_entries(db_hdl, entry_gen);
    }
    if (result == 0) {
        result = output_file_entries(db_hdl, entry_gen);
    }
 
    if (entry_gen) {
        cabeng_release(entry_gen);
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
    cabeng* entry_gen)
{
    int result;
    MSIHANDLE view_hdl;
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
            LPWSTR ds_file_name;
            LPCWSTR file_name;
            LPWSTR dir_key;
            LPWSTR file_key;
            LPWSTR src_dir_path;
            LPWSTR src_path;
            cabeng_file_entry* entry;
            int seq_num;
            rec_hdl = 0;
            ds_file_name = NULL;
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
                ds_file_name = read_str_from_record(rec_hdl, 2);
                i_status = ds_file_name ? 0 : -1;
                result = i_status;
            }
            if (i_status == 0) {
                file_name = get_long_name(ds_file_name);
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
                entry = cabeng_file_entry_create_1(
                    file_key, src_path, seq_num);
                i_status = entry ? 0 : -1;
                result = i_status;
            }
            if (i_status == 0) {
                i_status = cabeng_put_entry(entry_gen, entry);
                result = i_status;
            }
            if (entry) {
                cabeng_file_entry_release(entry);
            }

            if (src_path) {
                cabeng_free_mem(src_path);
            }

            if (src_dir_path) {
                cabeng_free_mem(src_dir_path);
            }
            
            if (ds_file_name) {
                cabeng_free_mem(ds_file_name);
            }

            if (dir_key) {
                cabeng_free_mem(dir_key);
            }
            if (file_key) {
                cabeng_free_mem(file_key);
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
        dir_key = cabeng_strdupw(start_dir_key); 
        status = dir_key ? 0 : -1;
        if (status == 0) {
            do {
                MSIHANDLE res_rec_hdl;
                LPWSTR ds_default_dir;
                LPWSTR parent_key;
                parent_key = NULL;
                ds_default_dir = NULL;
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
                    ds_default_dir = read_str_from_record(res_rec_hdl, 1);
                    if (ds_default_dir) {
                        LPCWSTR ds_parent_dir_path;
                        LPCWSTR parent_dir_path;
                        ds_parent_dir_path = get_source_dir_from_default_dir(
                                ds_default_dir);
                        parent_dir_path = get_long_name(ds_parent_dir_path);

                        if (wcscmp(L"SourceDir", parent_dir_path) == 0) {
                            parent_dir_path = L".";
                        }
                        if (dir_path) {
                            LPWSTR tmp_dir_path;
                            tmp_dir_path = join_path(parent_dir_path, dir_path);
                            cabeng_free_mem(dir_path);
                            dir_path = tmp_dir_path;
                        } else {
                            dir_path = cabeng_strdupw(parent_dir_path);
                        }
                    }
                }
                if (status == 0) {
                    parent_key = read_str_from_record(res_rec_hdl, 2);
                    if (parent_key) {
                        cabeng_free_mem(dir_key);
                        dir_key = parent_key;
                        parent_key = NULL;
                    } else {
                        dir_key = NULL;
                    }
                }

                
                if (ds_default_dir) {
                    cabeng_free_mem(ds_default_dir);
                }
                if (parent_key) {
                    cabeng_free_mem(parent_key);
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
    cabeng* entry_gen)
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
                    entry_gen,
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
    cabeng* entry_gen,
    int start_idx, int end_idx)
{
    int idx;
    int result;
    col_list* entry_list;
    result = 0;
    entry_list = col_array_list_create(10, 10,
        (int (*)(void*))
            cabeng_file_entry_get_hash,
        (int (*)(const void*, void **))
            cabeng_file_entry_copy_reference,
        (void (*)(void*)) 
            cabeng_file_entry_release_1);
    result = entry_list ? 0 : -1; 
    if (result == 0) {
        for (idx = start_idx; idx <= end_idx; idx++) {
            cabeng_file_entry* entry;
            entry = NULL;
            result = cabeng_get_entry(entry_gen, idx, &entry);
            if (entry) {
                col_list_append(entry_list, entry);
            }
            if (entry) {
                cabeng_file_entry_release(entry);
            }
            if (result) {
                break;
            }
        }
    }
    if (result == 0) {
        for (idx = 0; idx < col_list_size(entry_list); idx++) {
            cabeng_file_entry* entry;
            entry = NULL;
            result = col_list_get(entry_list, idx, (void**)&entry);
            if (entry) {
                result = output_file_entry(entry,
                    (idx + 1) == col_list_size(entry_list));
            }
            if (entry) {
                cabeng_file_entry_release(entry);
            }
            if (result) {
                break;
            }
        }
    }

    if (entry_list) {
        col_list_free(entry_list);
    }

    return result;
}

/**
 * output file entry
 */
static int
output_file_entry(
    cabeng_file_entry* entry,
    int flush_cabinet)
{
    LPCWSTR src;
    LPCWSTR dst;
    int result;
    result = 0;
    src = cabeng_file_entry_get_file_path_ref(entry);
    result = src ? 0 : -1;
    if (result == 0) {
        dst = cabeng_file_entry_get_file_name_id_ref(entry);
        result = dst ? 0 : -1;
    }    
    if (result == 0) {
        wprintf(L"%ls,%ls,MSZIP,0,0,0,%d\n", src, dst, flush_cabinet);
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
    result = wcschr(default_dir, L':');
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
        result = (LPWSTR)cabeng_alloc_mem((len + 1) * sizeof(WCHAR));
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
            result = (LPWSTR)cabeng_alloc_mem((buf_count + 1) * sizeof(WCHAR));
            if (result) {
                buf_count += 1;
                state = MsiRecordGetStringW(rec_hdl, field, result, &buf_count);
                if (state != ERROR_SUCCESS) {
                    cabeng_free_mem(result);
                    result = NULL;
                }
            }
        }
    }
    return result;
}

/**
 * get long name from short|long name string
 */
static LPCWSTR
get_long_name(
    LPCWSTR short_long_name)
{
    LPCWSTR result;
    LPCWSTR tmp_str;
    result = short_long_name;
    tmp_str = wcschr(short_long_name, L'|'); 
    if (tmp_str) {
        result = tmp_str + 1;
    }
    return result; 
}


/**
 * duplicate string
 */
static LPWSTR
cabeng_strdupw(
    LPCWSTR srcstr)
{
    size_t len;
    LPWSTR result;
    result = NULL;
    if (srcstr) {
        len = wcslen(srcstr);
        result = (LPWSTR)cabeng_alloc_mem((len + 1) * sizeof(WCHAR));
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
cabeng_alloc_mem(
    size_t size)
{
    return malloc(size);
}

/**
 * free memory
 */
static void
cabeng_free_mem(
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
    cabeng_option* option)
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
 * create entry generator
 */
cabeng*
cabeng_create(
    const cabeng_option* option)
{
    cabeng* result;
    col_list* entry_list;
    col_map* seq_entry_map;
    result = cabeng_alloc_mem(sizeof(cabeng));
    entry_list = col_array_list_create(10, 10,
        (int (*)(void*))
            cabeng_file_entry_get_hash,
        (int (*)(const void*, void **))
            cabeng_file_entry_copy_reference,
        (void (*)(void*)) 
            cabeng_file_entry_release_1);
    seq_entry_map = col_rb_map_create(
        (int (*)(const void*, const void*))cabeng_number_compare,
        (unsigned int (*)(const void*))cabeng_number_get_number,
        (unsigned int (*)(const void*))cabeng_file_entry_get_hash,
        (int (*)(const void*, void**))cabeng_file_entry_copy_reference,
        (int (*)(const void*, void**))cabeng_number_copy_ref,
        (void (*)(void*))cabeng_file_entry_release_1,
        (void (*)(void*))cabeng_number_release_1);
    if (result && entry_list && seq_entry_map) {
        result->option = option;
        result->entry_list = entry_list;
        result->seq_entry_map = seq_entry_map;
        result->ref_count = 1;
    } else {
        if (entry_list) {
            col_list_free(entry_list);
        }
        if (seq_entry_map) {
            col_map_free(seq_entry_map);
        }
        cabeng_free_mem(result);
        result = NULL;
    }
    return result;
}


/**
 * increment reference count
 */
unsigned int
cabeng_retain(
    cabeng* obj)
{
    unsigned int result;
    result = 0;
    if (obj) {
        result = ++obj->ref_count;
    } else {
        errno = EINVAL;
    }
    return result;
}

/**
 * decrement reference count
 */
unsigned int
cabeng_release(
    cabeng* obj)
{
    unsigned int result;
    result = 0;

    if (obj) {
        result = --obj->ref_count;
        if (result == 0) {
            col_list_free(obj->entry_list);
            col_map_free(obj->seq_entry_map);
            cabeng_free_mem(obj);
        }
    } else {
        errno = EINVAL;
    }

    return result;
}

/**
 * put file entry
 */
int
cabeng_put_entry(
    cabeng* obj,
    cabeng_file_entry* entry)
{
    int result;

    if (obj) {
        cabeng_number* number;
        number = NULL;
        number = cabeng_number_create(cabeng_file_entry_get_sequence(entry));
        result = number ? 0 : -1;
        if (result == 0) {
            result = col_map_put(obj->seq_entry_map, number, entry);
        }
        if (number) {
            cabeng_number_release(number);
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * get file entry
 */
int
cabeng_get_entry(
    cabeng* obj,
    int seq_num,
    cabeng_file_entry** entry)
{
    int result;
    result = 0;
    if (obj && entry) {
        cabeng_number* number;
        number = NULL;
        number = cabeng_number_create(seq_num);
        result = number ? 0 : -1;
        if (result == 0) {
            cabeng_file_entry* tmp_entry;
            tmp_entry = NULL;
            col_map_get(obj->seq_entry_map, number, (void **)&tmp_entry);
            *entry = tmp_entry;
        }
        if (number) {
            cabeng_number_release(number);
        } 
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}



/**
 * create file entry
 */
cabeng_file_entry*
cabeng_file_entry_create()
{
    cabeng_file_entry* result;
    result = (cabeng_file_entry*)cabeng_alloc_mem(sizeof(cabeng_file_entry));
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
cabeng_file_entry*
cabeng_file_entry_create_1(
    LPCWSTR file_name_id,
    LPCWSTR file_path,
    int seq)
{
    cabeng_file_entry* result;
    result = cabeng_file_entry_create();
    if (result) {
        int state;
        state = cabeng_file_entry_set_file_name_id(result, file_name_id);
        if (state == 0) {
            cabeng_file_entry_set_file_path(result, file_path);
        }
        if (state == 0) {
            cabeng_file_entry_set_sequence(result, seq);
        }
        if (state) {
            cabeng_file_entry_release(result);
            result = NULL;
        }
    }
    return result;
}


/**
 * increment reference count
 */
unsigned int
cabeng_file_entry_retain(
    cabeng_file_entry* obj)
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
cabeng_file_entry_release(
    cabeng_file_entry* obj)
{
    unsigned int result;
    result = 0;
    if (obj) {
        result = --obj->ref_count;
        if (result == 0) {
            cabeng_file_entry_set_file_name_id(obj, NULL);
            cabeng_file_entry_set_file_path(obj, NULL);
            cabeng_free_mem(obj);
        }
    }
    return result;
}

/**
 * release file entry
 */
void
cabeng_file_entry_release_1(
    cabeng_file_entry* obj)
{
    cabeng_file_entry_release(obj);
}

/**
 * get hash
 */
int
cabeng_file_entry_get_hash(
    cabeng_file_entry* obj)
{
    union {
        cabeng_file_entry* obj;
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
cabeng_file_entry_copy_reference(
    cabeng_file_entry* src,
    cabeng_file_entry** dst)
{
    int result;
    result = 0;
    if (src && dst) {
        cabeng_file_entry_retain(src);
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
cabeng_file_entry_get_file_name_id_ref(
    cabeng_file_entry* obj)
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
cabeng_file_entry_get_file_name_id(
    cabeng_file_entry* obj)
{
    LPWSTR result;
    result = NULL;
    if (obj) {
        result = cabeng_strdupw(obj->file_name_id);
    }
    return result;
}

/**
 * set file name id 
 */
int
cabeng_file_entry_set_file_name_id(
    cabeng_file_entry* obj,
    LPCWSTR file_name_id)
{
    unsigned int result;
    result = 0;
    if (obj) {
        if (obj->file_name_id != file_name_id) {
            if (obj->file_name_id) {
                cabeng_free_mem(obj->file_name_id);
            }
            obj->file_name_id = cabeng_strdupw(file_name_id);
        }
    }
    return result;
}


/**
 * get file path
 */
LPCWSTR
cabeng_file_entry_get_file_path_ref(
    cabeng_file_entry* obj)
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
cabeng_file_entry_get_file_path(
    cabeng_file_entry* obj)
{
    LPWSTR result;
    result = NULL;
    if (obj) {
        result = cabeng_strdupw(obj->file_path);
    }
    return result;
}

/**
 * set file path 
 */
int
cabeng_file_entry_set_file_path(
    cabeng_file_entry* obj,
    LPCWSTR file_path)
{
    unsigned int result;
    result = 0;
    if (obj) {
        if (obj->file_path != file_path) {
            if (obj->file_path) {
                cabeng_free_mem(obj->file_path);
            }
            obj->file_path = cabeng_strdupw(file_path);
        }
    }
    return result;
}

/**
 * get sequence
 */
int
cabeng_file_entry_get_sequence(
    cabeng_file_entry* obj)
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
cabeng_file_entry_set_sequence(
    cabeng_file_entry* obj,
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

/**
 * create number object
 */
cabeng_number*
cabeng_number_create(
    int number)
{
    cabeng_number* result;
    result = (cabeng_number*)cabeng_alloc_mem(sizeof(cabeng_number));
    if (result) {
        result->ref_count = 1;
        result->number = number;
    }

    return result;
}

/**
 * increment reference count
 */
unsigned int
cabeng_number_retain(
    cabeng_number* obj)
{
    unsigned int result;
    result = 0;
    if (obj) {
        result = ++obj->ref_count;
    } else {
        errno = EINVAL;
    }
    return result;
}

/**
 * copy object
 */
int
cabeng_number_copy_ref(
    cabeng_number* src,
    cabeng_number** dst)
{
    int result;
    result = 0;
    if (src && dst) {
        cabeng_number_retain(src);
    }
    if (dst) {
        *dst = src;
    }

    return result;
}

/**
 * decrement reference count
 */
unsigned int
cabeng_number_release(
    cabeng_number* obj)
{
    unsigned int result;
    if (obj) {
        result = --obj->ref_count;
        if (result == 0) {
            cabeng_free_mem(obj);
        }
    } else {
        errno = EINVAL;
    }
    return result;
}

/**
 * decrement reference count
 */
void
cabeng_number_release_1(
    cabeng_number* obj) 
{
    cabeng_number_release(obj);
}

/**
 * get number
 */
int
cabeng_number_get_number(
    cabeng_number* obj)
{
    int result;
    result = 0;
    if (obj) {
        result = obj->number;
    }
    return result;
}


/**
 * compare number
 */
int
cabeng_number_compare(
    cabeng_number* obj_1,
    cabeng_number* obj_2)
{
    int result;
    result = 0;
    if (obj_1 && obj_2) {
        if (obj_1 != obj_2) {
            result = obj_1->number - obj_2->number;
        }
    } else if (obj_2) {
        result = -1;
    } else if (obj_1) {
        result = 1;
    }
    return result;
}



/* vi: se ts=4 sw=4 et: */

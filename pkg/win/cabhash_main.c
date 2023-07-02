#include <windows.h>
#include <wchar.h>
#include <errno.h>
#include <msiquery.h>
#include <msi.h>
#include <wincrypt.h>
#include <pathcch.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <limits.h>
#include <sys/stat.h>

#include "col/array_list.h"
#include "csv.h"
#include "buffer/char_buffer.h"
#include "col/rb_map.h"
#include "cstr.h"
#include "number_parser.h"
#include "str_conv.h"

/**
 * cabinet hash option
 */
typedef struct _cabhash cabhash;

/**
 * cabinet entry to extract singed data
 */
typedef struct _cabhash_entry cabhash_entry;

/**
 * keep certificate and hash in heap
 */
typedef struct _cabhash_cert_hash_blob cabhash_cert_hash_blob;

/**
 * entry iterator status
 */
typedef struct _cabhash_entry_iter_state cabhash_entry_iter_state;

/**
 * entry iterator status for updating cetificate and hash
 */
typedef struct _cabhash_entry_iter_cert_hash_state
    cabhash_entry_iter_cert_hash_state;

/**
 * entry iterator status for exit
 */
typedef struct _cabhash_entry_iter_exit_state
    cabhash_entry_iter_exit_state;


struct _cabhash {
    /**
     * data input
     */
    wchar_t* input_file;

    /**
     * cab file directory
     */
    wchar_t* cab_file_dir;

    /**
     * msi path
     */
    wchar_t* msi_path;


    /**
     * show help
     */
    int show_help;

    /**
     * entries to extract cabinet code signed data.
     */
    col_list* cab_entries;


    /**
     * cabinet name certificate, hash blob map
     */
    col_map* cab_cert_hash_blob;
};

/**
 * cabinet entry to extract singed data
 */
struct _cabhash_entry {

    /**
     * reference count
     */
    unsigned int ref_count;


    /**
     * cab file name
     */
    char* cab_name;

    /**
     * certificate table key
     */
    char* cert_key;

    /**
     * certificate file path 
     */
    char* cert_file_path;

    /**
     * cert file will be deleted on exit.
     */
    int cert_file_delete_on_exit;

    /**
     * hash file path 
     */
    char* hash_file_path;

    /**
     * hash file will be delete on exit.
     */
    int hash_file_delete_on_exit;
};


/**
 * certificate, hash
 */
struct _cabhash_cert_hash_blob
{
    /**
     * reference count
     */
    unsigned int ref_count;
    /**
     * certificates
     */
    void* cert;

    /**
     * certificate length
     */
    size_t cert_size;


    /**
     * hash 
     */
    void* hash;

    /**
     * hash length
     */
    size_t hash_size;
    
};

/**
 * entry iterator status
 */
struct _cabhash_entry_iter_state 
{
    cabhash* cabhash_obj;
};


struct _cabhash_entry_iter_cert_hash_state
{
    /**
     * database object handle
     */
    MSIHANDLE msi_db_hdl;

    /**
     * cabhash object
     */
    cabhash* cabhash_obj;

    /**
     * view handle for updating certificate
     */
    MSIHANDLE update_cert_view_hdl;

    /**
     * view handle for updating hash value
     */
    MSIHANDLE update_hash_view_hdl;
};

/**
 * entry iterator status for exit
 */
struct _cabhash_entry_iter_exit_state {
    /**
     * cabhash object
     */
    cabhash* cabhash_obj;
};


/**
 * initialize cabhash object
 */
int
cabhash_init(
    cabhash* obj);


/**
 * teardown cabhash
 */
void
cabhash_tear_down(
    cabhash* obj);

/**
 * set input file
 */
int
cabhash_set_input(
    cabhash* obj,
    const wchar_t* input);

/**
 * set directory in which cabinets exist.
 */
int
cabhash_set_cab_file_dir(
    cabhash* obj,
    const wchar_t* cab_file_dir);

/**
 * set msi file path
 */
int
cabhash_set_msi_path(
    cabhash* obj,
    const wchar_t* msi_path);



/**
 * run main program
 */
int
cabhash_run(
    cabhash* obj);

/**
 * show help message
 */
int
cabhash_show_help(
    cabhash* obj);


/**
 * create cabinet path
 */
wchar_t*
cabhash_create_cabinet_path(
    cabhash* obj,
    const wchar_t* cab_name);

/**
 * create cabinet path
 */
wchar_t*
cabhash_create_cabinet_path_1(
    cabhash* obj,
    const char* cab_name);



/**
 * iterate entries for creating cert blob map
 */
int
cabhash_entry_iterator(
    cabhash_entry_iter_state* state,
    cabhash_entry* entry);

/**
 * iterate entries for creating cert blob map
 */
int
cabhash_entry_iterator_for_updating_cert_hash(
    cabhash_entry_iter_cert_hash_state* iter_state,
    cabhash_entry* entry);

/**
 * iterate entries for exit
 */
int
cabhash_entry_iterator_for_exit(
    cabhash_entry_iter_exit_state* state,
    cabhash_entry* entry);

/**
 * load csv from input file
 */
static csv*
cabhash_load_csv_from_input(
    cabhash* obj);

/**
 * load csv for extracting code signed data
 */
csv*
cabhash_load_csv_from_byte_array(
    cabhash* obj,
    const char* data,
    size_t data_length);


/**
 * load hash entries from csv
 */
int
cabhash_load_entries_from_csv(
    cabhash* obj,
    csv* csv_data);

/**
 * load entries from msi
 */
int
cabhash_load_entries_from_msi(
    cabhash* obj,
    MSIHANDLE db_hdl);

/**
 * load certiicate, hash blob with current entry
 */
int
cabhash_load_cert_hash_blob(
    cabhash* obj);

/**
 * update cert and hash in msi table
 */
int
cabhash_update_msi_with_cert_hash(
    cabhash* obj,
    MSIHANDLE db_hdl);

/**
 * iterate entries on exit
 */
int
cabhash_iterate_entries_on_exit(
    cabhash* obj);

/**
 * add entry
 */
int
cabhash_add_entry(
    cabhash* obj,
    const char* cab_name,
    const char* cert_key,
    const char* cert_file_path,
    int delete_cert_file_on_exit,
    const char* hash_file_path,
    int delete_hash_file_on_exit);

/**
 * add entry
 */
int
cabhash_add_entry_1(
    cabhash* obj,
    const wchar_t* cab_name,
    const wchar_t* cert_key,
    const wchar_t* cert_file_path,
    int delete_cert_file_on_exit,
    const wchar_t* hash_file_path,
    int delete_hash_file_on_exit);



/**
 * parse option
 */
static int
cabhash_parse_option(
    cabhash* obj,
    int argc,
    wchar_t** argv);

/**
 * put cert and hash
 */
int
cabhash_put_cert_hash(
    cabhash* obj,
    const char* cab_name,
    const void* hash,
    size_t hash_size,
    const void* cert,
    size_t cert_size);

/**
 * put cert and hash
 */
int
cabhash_put_cert_hash_blob(
    cabhash* obj,
    const char* cab_name,
    cabhash_cert_hash_blob* cert_hash_blob);

/**
 * get cert and hash
 */
int
cabhash_get_cert_hash_blob(
    cabhash* obj,
    const char* cab_name,
    cabhash_cert_hash_blob** cert_hash_blob);

/**
 * get file path and delete on exit operation flag.
 * if file_path is null or empty, then create temporary file deleted on exit.
 */
int
cabhash_get_file_and_operation(
    cabhash* obj,
    const char* file_path,
    const char* delete_on_exit_str,
    char** file_path_actual,
    int* delete_on_exit);
/**
 * create temporary file path
 */
char*
cabhash_create_temporary_file_path(
    cabhash* obj);

/**
 * create temporary file path
 */
wchar_t*
cabhash_create_temporary_file_path_1(
    cabhash* obj);

/**
 * read all from input
 */
static int
cabhash_read_all_input(
    cabhash* obj,
    char** data,
    size_t* data_size);

/**
 * create entry
 */
cabhash_entry*
cabhash_entry_create(); 

/**
 * create entry
 */
cabhash_entry*
cabhash_entry_create_1(
    const char* cab_name,
    const char* cert_key,
    const char* cert_file_path,
    int delete_cert_file_on_exit,
    const char* hash_file_path,
    int delete_hash_file_on_exit); 

/**
 * increment reference count
 */
unsigned int
cabhash_entry_retain(
    cabhash_entry* obj);

/**
 * decremeent reference count
 */
unsigned int
cabhash_entry_release(
    cabhash_entry* obj);


/**
 * copy reference
 */
int
cabhash_entry_copy_ref(
    cabhash_entry* src,
    cabhash_entry** dst);

/**
 * calcurate hash code
 */
int
cabhash_entry_hash(
    cabhash_entry* obj);

/**
 * set cab file name
 */
int
cabhash_entry_set_cab_name(
    cabhash_entry* obj,
    const char* cab_name);

/**
 * set certificate table key
 */
int
cabhash_entry_set_cert_key(
    cabhash_entry* obj,
    const char* cert_key);


/**
 * set certificate file path 
 */
int
cabhash_entry_set_cert_file_path(
    cabhash_entry* obj,
    const char* cert_file_path,
    int delete_on_exit);

/**
 * set hash file path 
 */
int
cabhash_entry_set_hash_file_path(
    cabhash_entry* obj,
    const char* hash_file_path,
    int delete_on_exit);


/**
 * get cabinet file name as utf16
 */
wchar_t*
cabhash_entry_get_cab_name_as_utf16(
    cabhash_entry* obj);

/**
 * get cert file path as utf16
 */
wchar_t*
cabhash_entry_get_cert_file_path_as_utf16(
    cabhash_entry* obj);
 
/**
 * get certificate table key as utf16
 */
wchar_t*
cabhash_entry_get_cert_key_as_utf16(
    cabhash_entry* obj);

/**
 * get hash file path as utf16
 */
wchar_t*
cabhash_entry_get_hash_file_path_as_utf16(
    cabhash_entry* obj);

/**
 * create certificate-hash object
 */
cabhash_cert_hash_blob*
cabhash_cert_hash_blob_create();

/**
 * create certificate-hash object
 */
cabhash_cert_hash_blob*
cabhash_cert_hash_blob_create_1(
    const void* cert,
    size_t cert_size,
    const void* hash,
    size_t hash_size);

/**
 * create cert hash blob from cab file
 */
cabhash_cert_hash_blob*
cabhash_cert_hash_blob_create_from_cab_file(
    const wchar_t* signed_path);

/**
 * increment reference count
 */
unsigned int
cabhash_cert_hash_blob_retain(
    cabhash_cert_hash_blob* obj);
/**
 * decrement reference count
 */
unsigned int
cabhash_cert_hash_blob_release(
    cabhash_cert_hash_blob* obj);

/**
 * copy reference
 */
int
cabhash_cert_hash_blob_copy_ref(
    cabhash_cert_hash_blob* src,
    cabhash_cert_hash_blob** dest);

/**
 * set certificatte
 */
int
cabhash_cert_hash_blob_set_cert(
    cabhash_cert_hash_blob* obj,
    const void* cert,
    size_t cert_size);

/**
 * set hash 
 */
int
cabhash_cert_hash_blob_set_hash(
    cabhash_cert_hash_blob* obj,
    const void* hash,
    size_t hash_size);

/**
 * calc hash code
 */
unsigned int
cabhash_cert_hash_blob_hash(
    cabhash_cert_hash_blob* obj);


/**
 * save certificate into file
 */
int
cabhash_cert_hash_blob_save_cert(
    cabhash_cert_hash_blob* obj,
    const char* file_path);
/**
 * save certificate into file
 */
int
cabhash_cert_hash_blob_save_hash(
    cabhash_cert_hash_blob* obj,
    const char* file_path);

/**
 * read string from record
 */
static LPWSTR
read_str_from_record(
    MSIHANDLE rec_hdl,
    UINT field);
/**
 * read all data into memory
 */
static int
read_all(
    char** data,
    size_t* data_size,
    FILE* fs);

/**
 * calculate string hash
 */
static int
str_hash(
    const char* str,
    size_t length);

/**
 * allocate memory
 */
static void*
cabhash_alloc(
    size_t size);

/**
 * allocate memory
 */
static void*
cabhash_alloc_1(
    unsigned int size);


/**
 * free memory
 */
static void
cabhash_free(
    void* obj);

/**
 * show help message
 */
int
cabhash_show_help(
    cabhash* obj)
{
    int result;
    result = 0;
    if (obj) {
        _putws(
L"cabhash [OPTION]\n"
L"-i INPUT      specify cabinet file path and column name in csv format.\n"
L"-m MSI_PATH   specify msi file to be updated with certificate and hash\n"
L"-c CAB_DIR    specify directory in where cabinet files exist.\n"
L"-h            display this message.\n"
        );
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}


/**
 * parse option
 */
static int
cabhash_parse_option(
    cabhash* obj,
    int argc,
    wchar_t** argv)
{
    int result;
    int idx;
    result = 0;
    for (idx = 1; idx < argc; idx++) {
        wchar_t* opt;
        wchar_t* next_opt;
        next_opt = NULL;
        opt = argv[idx];
        if (idx < argc - 1) {
            next_opt = argv[idx + 1];
        }
        if (wcscmp(L"-h", opt) == 0) {
            obj->show_help = 1;
        } else if (wcscmp(L"-i", opt) == 0) {
            if (next_opt) {
                result = cabhash_set_input(obj, next_opt);
                idx++;
            }
        } else if (wcscmp(L"-m", opt) == 0) {
            if (next_opt) {
                result = cabhash_set_msi_path(obj, next_opt);
                idx++;
            }
        } else if (wcscmp(L"-c", opt) == 0) {
            if (next_opt) {
                result = cabhash_set_cab_file_dir(obj, next_opt);
                idx++;
            }
        }
        if (result) {
            break;
        }
    }
    return result; 
}


/**
 * initialize cabhash object
 */
int
cabhash_init(
    cabhash* obj)
{
    int result;
    col_list* cab_entries;
    col_map* cab_cert_hash_blob;
    result = 0;
    cab_cert_hash_blob = NULL;
    cab_entries = col_array_list_create(10, 10,
        (int (*)(void*))cabhash_entry_hash,
        (int (*)(const void*, void**))cabhash_entry_copy_ref,
        (void (*)(void*))cabhash_entry_release);

    result = cab_entries ? 0 : -1;
    if (result == 0) {
        cab_cert_hash_blob = col_rb_map_create(
            (int (*)(const void*, const void*))cstr_compare,
            (unsigned int (*)(const void*))cstr_hash,
            (unsigned int (*)(const void*))cabhash_cert_hash_blob_hash,
            (int (*)(const void*, void**))cabhash_cert_hash_blob_copy_ref,
            (int (*)(const void*, void**))cstr_retain_1,
            (void (*)(void*))cabhash_cert_hash_blob_release,
            (void (*)(void*))cstr_release_1);
        result = cab_cert_hash_blob ? 0 : -1;
    }

    if (result == 0) {
        obj->cab_file_dir = NULL;
        result = cabhash_set_cab_file_dir(obj, L".\\");
    }
    if (result == 0) {
        obj->cab_entries = NULL;
        obj->cab_cert_hash_blob = NULL;
        obj->cab_entries = cab_entries;
        obj->cab_cert_hash_blob = cab_cert_hash_blob;
        obj->show_help = 0;
        obj->input_file = NULL;
        obj->msi_path = NULL;
        cab_entries = NULL;
        cab_cert_hash_blob = NULL;
    } else {
        cabhash_set_cab_file_dir(obj, NULL);
    }
    if (cab_cert_hash_blob) {
        col_map_free(cab_cert_hash_blob);
    }

    if (cab_entries) {
        col_list_free(cab_entries);
    }

    return result;
}

/**
 * tear down cabhash object
 */
void
cabhash_tear_down(
    cabhash* obj) {
    if (obj) {
        col_map_free(obj->cab_cert_hash_blob);
        col_list_free(obj->cab_entries);
        cabhash_set_input(obj, NULL);
        cabhash_set_msi_path(obj, NULL);
        cabhash_set_cab_file_dir(obj, NULL);
    }
}



/**
 * set input file
 */
int
cabhash_set_input(
    cabhash* obj,
    const wchar_t* input)
{
    int result;
    result = 0;
    if (obj) {
        if (obj->input_file != input) {
            wchar_t* new_input;
            new_input = NULL;
            if (input) {
                size_t len;
                len = wcslen(input); 
                new_input = (wchar_t*)cabhash_alloc(
                    sizeof(wchar_t) * (len + 1));
                result = new_input ? 0 : -1;
                if (result == 0) {
                    memcpy(new_input, input, sizeof(wchar_t) * (len + 1));
                }
            }
            if (result == 0) {
                cabhash_free(obj->input_file);
                obj->input_file = new_input;
            }
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * set cab file directory
 */
int
cabhash_set_cab_file_dir(
    cabhash* obj,
    const wchar_t* cab_file_dir)
{
    int result;
    result = 0;
    if (obj) {
        if (obj->cab_file_dir != cab_file_dir) {
            wchar_t* new_cab_file_dir;
            new_cab_file_dir = NULL;
            if (cab_file_dir) {
                size_t len;
                len = wcslen(cab_file_dir); 
                new_cab_file_dir = (wchar_t*)cabhash_alloc(
                    sizeof(wchar_t) * (len + 1));
                result = new_cab_file_dir ? 0 : -1;
                if (result == 0) {
                    memcpy(new_cab_file_dir,
                        cab_file_dir, sizeof(wchar_t) * (len + 1));
                }
            }
            if (result == 0) {
                cabhash_free(obj->cab_file_dir);
                obj->cab_file_dir = new_cab_file_dir;
            }
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * set msi path
 */
int
cabhash_set_msi_path(
    cabhash* obj,
    const wchar_t* msi_path)
{
    int result;
    result = 0;
    if (obj) {
        if (obj->msi_path != msi_path) {
            wchar_t* new_msi_path;
            new_msi_path = NULL;
            if (msi_path) {
                size_t len;
                len = wcslen(msi_path); 
                new_msi_path = (wchar_t*)cabhash_alloc(
                    sizeof(wchar_t) * (len + 1));
                result = new_msi_path ? 0 : -1;
                if (result == 0) {
                    memcpy(new_msi_path,
                        msi_path, sizeof(wchar_t) * (len + 1));
                }
            }
            if (result == 0) {
                cabhash_free(obj->msi_path);
                obj->msi_path = new_msi_path;
            }
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}



/**
 * run main program
 */
int
cabhash_run(
    cabhash* obj)
{
    int result;
    result = 0;
    if (obj) {
        if (!obj->show_help) {
            if (obj->msi_path) {
                MSIHANDLE db_hdl;
                unsigned int u_state;
                db_hdl = 0;
                u_state = MsiOpenDatabaseW(
                    obj->msi_path, (LPCWSTR)MSIDBOPEN_TRANSACT, &db_hdl);

                result = u_state == ERROR_SUCCESS ? 0 : -1;
                if (result == 0) {
                    if (obj->input_file) {
                        csv* csv_obj;
                        csv_obj = cabhash_load_csv_from_input(obj);
                        result = csv_obj ? 0 : -1;
                        if (result == 0) {
                            result = cabhash_load_entries_from_csv(
                                obj, csv_obj);
                        }
                        if (csv_obj) {
                            csv_release(csv_obj);
                        }
                    } else {
                        result = cabhash_load_entries_from_msi(obj, db_hdl);
                    }
                }
                if (result == 0) {
                    result = cabhash_load_cert_hash_blob(obj);
                }
                if (result == 0) {
                    result = cabhash_update_msi_with_cert_hash(obj, db_hdl);
                }

                if (result == 0) {
                    MsiDatabaseCommit(db_hdl);
                }
                if (result == 0) {
                    result = cabhash_iterate_entries_on_exit(obj);
                }
                if (db_hdl) {
                    MsiCloseHandle(db_hdl);
                }
            }
        } else {
            result = cabhash_show_help(obj);
        }

    } else {
        result = -1;
        errno = EINVAL;
    }

    return result;
}

/**
 * add entry
 */
int
cabhash_add_entry(
    cabhash* obj,
    const char* cab_name,
    const char* cert_key,
    const char* cert_file_path,
    int delete_cert_file_on_exit,
    const char* hash_file_path,
    int delete_hash_file_on_exit)
{
    int result;
    cabhash_entry* entry;
    entry = cabhash_entry_create_1(
        cab_name, cert_key,
        cert_file_path, delete_cert_file_on_exit,
        hash_file_path, delete_hash_file_on_exit);
    result = entry ? 0 : -1;
    if (result == 0) {
        result = col_list_append(obj->cab_entries, entry);
    }

    if (entry) {
        cabhash_entry_release(entry);
    }
    return result;
}

/**
 * add entry
 */
int
cabhash_add_entry_1(
    cabhash* obj,
    const wchar_t* cab_name_w,
    const wchar_t* cert_key_w,
    const wchar_t* cert_file_path_w,
    int delete_cert_file_on_exit,
    const wchar_t* hash_file_path_w,
    int delete_hash_file_on_exit)
{
    int result;
    char* cab_name;
    char* cert_key;
    char* cert_file_path;
    char* hash_file_path;

    cert_key = NULL;
    cab_name = str_conv_utf16_to_utf8(cab_name_w,
        wcslen(cab_name_w) + 1,
        cabhash_alloc,
        cabhash_free);

    result = cab_name ? 0 : -1; 
    if (result == 0) {
        cert_key = str_conv_utf16_to_utf8(cert_key_w,
            wcslen(cert_key_w) + 1,
            cabhash_alloc,
            cabhash_free);
        result = cert_key ? 0 : -1;
    }
    if (result == 0) {
        cert_file_path = str_conv_utf16_to_utf8(cert_file_path_w,
            wcslen(cert_file_path_w) + 1,
            cabhash_alloc,
            cabhash_free);
        result = cert_file_path ? 0 : -1;
    }
    if (result == 0) {
        hash_file_path = str_conv_utf16_to_utf8(hash_file_path_w,
            wcslen(hash_file_path_w) + 1,
            cabhash_alloc,
            cabhash_free);
        result = hash_file_path ? 0 : -1;
    }
    if (result == 0) {
        result = cabhash_add_entry(obj, cab_name, cert_key,
            cert_file_path, delete_cert_file_on_exit,
            hash_file_path, delete_hash_file_on_exit);
    }
    cabhash_free(cert_file_path);
    cabhash_free(hash_file_path);
    cabhash_free(cab_name);
    cabhash_free(cert_key);
    return result;
}

/**
 * update cert and hash in msi table
 */
int
cabhash_update_msi_with_cert_hash(
    cabhash* obj,
    MSIHANDLE db_hdl)
{
    int result;
    unsigned int u_state;
    MSIHANDLE view_for_cert_hdl;
    MSIHANDLE view_for_hash_hdl;
    
    result = 0;
    view_for_cert_hdl = 0;
    view_for_hash_hdl = 0;
    if (result == 0) {
        u_state = MsiDatabaseOpenViewW(db_hdl,
            L"UPDATE `MsiDigitalCertificate` "
            L"SET `CertData`= ? "
            L"WHERE `DigitalCertificate`= ?",
            &view_for_cert_hdl);
        result = u_state == ERROR_SUCCESS ? 0 : -1;
    }

    if (result == 0) {
        u_state = MsiDatabaseOpenViewW(db_hdl,
            L"UPDATE `MsiDigitalSignature` "
            L"SET `Hash` = ? "
            L"WHERE `SignObject`= ? AND "
            L"`Table` = 'Media'",
            &view_for_hash_hdl);
        result = u_state == ERROR_SUCCESS ? 0 : -1;
    }

    if (result == 0) {
        cabhash_entry_iter_cert_hash_state iter_state;
        memset(&iter_state, 0, sizeof(iter_state));
        iter_state.msi_db_hdl = db_hdl;
        iter_state.cabhash_obj = obj;
        iter_state.update_cert_view_hdl = view_for_cert_hdl;
        iter_state.update_hash_view_hdl = view_for_hash_hdl;
        result = col_list_forward_iterate(obj->cab_entries, 
            (int (*)(void*, const void*))
                cabhash_entry_iterator_for_updating_cert_hash,
            &iter_state);
    }
 
    if (view_for_cert_hdl) {
        MsiViewClose(view_for_cert_hdl);
    }

    if (view_for_hash_hdl) {
        MsiViewClose(view_for_hash_hdl);
    }

    return result;
}

/**
 * iterate entries for creating cert blob map
 */
int
cabhash_entry_iterator_for_updating_cert_hash(
    cabhash_entry_iter_cert_hash_state* iter_state,
    cabhash_entry* entry)
{
    int result; 
    int state;
    unsigned int u_state;
    LPWSTR cab_name_w;
    LPWSTR cert_key_w;
    LPWSTR cert_file_path_w;
    LPWSTR hash_file_path_w;
    cabhash_cert_hash_blob* cert_hash_blob;
    cert_hash_blob = NULL;
    result = 0;
    state = 0;
    u_state = 0;
    cab_name_w = NULL;
    cert_key_w = NULL;
    cert_file_path_w = NULL;
    hash_file_path_w = NULL;
    state = cabhash_get_cert_hash_blob(
        iter_state->cabhash_obj, entry->cab_name, &cert_hash_blob); 
    if (state == 0) {
        cab_name_w = (LPWSTR)str_conv_utf8_to_utf16(
            entry->cab_name, strlen(entry->cab_name) + 1,
            cabhash_alloc, cabhash_free);
        state = cab_name_w ? 0 : -1;
    } 
    if (state == 0) {
        cert_key_w = (LPWSTR)str_conv_utf8_to_utf16(
            entry->cert_key, strlen(entry->cert_key) + 1,
            cabhash_alloc, cabhash_free);
        state = cert_key_w ? 0 : -1;
    }
    if (state == 0) {
        state = cabhash_cert_hash_blob_save_cert(cert_hash_blob,
            entry->cert_file_path);
    }
    if (state == 0) {
        cert_file_path_w = cabhash_entry_get_cert_file_path_as_utf16(entry);
        state = cert_file_path_w ? 0 : -1;
    }
    if (state == 0) {
        state = cabhash_cert_hash_blob_save_hash(cert_hash_blob,
            entry->hash_file_path);
    }
    if (state == 0) {
        hash_file_path_w = cabhash_entry_get_hash_file_path_as_utf16(entry);
        state = hash_file_path_w ? 0 : -1;
    }
    if (state == 0) {
        MSIHANDLE rec_hdl;
        rec_hdl = MsiCreateRecord(2);
        state = rec_hdl ? 0 : -1;
        result = state;
        if (state == 0) {
            u_state = MsiRecordSetStringW(
                rec_hdl, 2, cert_key_w);
            state = u_state == ERROR_SUCCESS ? 0 : -1;
        }
        if (state == 0) {
            u_state = MsiRecordSetStreamW(
                rec_hdl, 1, cert_file_path_w);
            state = u_state == ERROR_SUCCESS ? 0 : -1;
        }
        if (state == 0) {
            u_state = MsiViewExecute(iter_state->update_cert_view_hdl, rec_hdl);
            state = u_state == ERROR_SUCCESS ? 0 : -1;
        }
        if (rec_hdl) {
            MsiCloseHandle(rec_hdl);
        }
        rec_hdl = MsiCreateRecord(2);
        state = rec_hdl ? 0 : -1;
        result = state;
        if (state == 0) {
            u_state = MsiRecordSetStringW(
                rec_hdl, 2, cab_name_w);
            state = u_state == ERROR_SUCCESS ? 0 : -1;
        }
        if (state == 0) {
            u_state = MsiRecordSetStreamW(
                rec_hdl, 1, hash_file_path_w);
            state = u_state == ERROR_SUCCESS ? 0 : -1;
        }
        if (state == 0) {
            u_state = MsiViewExecute(iter_state->update_hash_view_hdl, rec_hdl);
            state = u_state == ERROR_SUCCESS ? 0 : -1;
        }
        if (rec_hdl) {
            MsiCloseHandle(rec_hdl);
        }
    }

    cabhash_free(cert_file_path_w);
    cabhash_free(hash_file_path_w);
    cabhash_free(cert_key_w);
    cabhash_free(cab_name_w);
    if (cert_hash_blob) {
        cabhash_cert_hash_blob_release(cert_hash_blob);
    }
    return result;
}
 
/**
 * load entries from msi
 */
int
cabhash_load_entries_from_msi(
    cabhash* obj,
    MSIHANDLE db_hdl)
{
    int result;
    unsigned int u_state;
    MSIHANDLE view_hdl;
    result = 0;
    view_hdl = 0;

    if (result == 0) {
        u_state = MsiDatabaseOpenViewW(db_hdl,
            L"SELECT `Media`.`Cabinet`, "
            L"`MsiDigitalCertificate`.`DigitalCertificate` "
            L"FROM `Media`, `MsiDigitalCertificate`, `MsiDigitalSignature` "
            L"WHERE `Media`.`Cabinet`=`MsiDigitalSignature`.`SignObject` "
            L"AND `MsiDigitalSignature`.`Table`='Media' "
            L"AND `MsiDigitalCertificate`.`DigitalCertificate`"
                L"=`MsiDigitalSignature`.`DigitalCertificate_` ",
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
            LPWSTR cab_file_w;
            LPWSTR cert_key_w;
            wchar_t* cert_file_path_w;
            wchar_t* hash_file_path_w;
            rec_hdl = 0;
            cab_file_w = NULL;
            cert_key_w = NULL;
            cert_file_path_w = NULL;
            hash_file_path_w = NULL;

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
                cab_file_w = read_str_from_record(rec_hdl, 1);
                i_status = cab_file_w ? 0 : -1;
                result = i_status;
            }
            if (i_status == 0) {
                cert_key_w = read_str_from_record(rec_hdl, 2);
                i_status = cert_key_w ? 0 : -1;
                result = i_status;
            }
           
            if (i_status == 0) {
                cert_file_path_w = cabhash_create_temporary_file_path_1(obj);
                result = cert_file_path_w ? 0 : -1;
            }
            if (i_status == 0) {
                hash_file_path_w = cabhash_create_temporary_file_path_1(obj);
                result = hash_file_path_w ? 0 : -1;
            }

            if (i_status == 0) {
                i_status = cabhash_add_entry_1(obj,
                    cab_file_w, cert_key_w,
                    cert_file_path_w, 1,
                    hash_file_path_w, 1);
                result = i_status;
            }

            cabhash_free(hash_file_path_w);
            cabhash_free(cert_file_path_w);
            cabhash_free(cab_file_w);
            cabhash_free(cert_key_w);
 
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
            result = (LPWSTR)cabhash_alloc((buf_count + 1) * sizeof(WCHAR));
            if (result) {
                buf_count += 1;
                state = MsiRecordGetStringW(rec_hdl, field, result, &buf_count);
                if (state != ERROR_SUCCESS) {
                    cabhash_free(result);
                    result = NULL;
                }
            }
        }
    }
    return result;
}


/**
 * load hash entries from csv
 */
int
cabhash_load_entries_from_csv(
    cabhash* obj,
    csv* csv_data)
{
    int result;
    result = 0;
    if (obj && csv_data) {
        unsigned int r_idx;
        for (r_idx = 0; r_idx < csv_get_row_count(csv_data); r_idx++) {
            char* cab_name;
            char* cert_key;
            char* delete_cert_on_exit_str;
            char* delete_hash_on_exit_str;
            int delete_cert_on_exit;
            int delete_hash_on_exit;
            char* cert_file_path_csv;
            char* hash_file_path_csv;
            char* cert_file_path;
            char* hash_file_path;
            cab_name = NULL;
            cert_key = NULL;
            cert_file_path_csv = NULL;
            hash_file_path_csv = NULL;
            cert_file_path = NULL;
            hash_file_path = NULL;
            delete_cert_on_exit_str = NULL;
            delete_hash_on_exit_str = NULL;
            delete_cert_on_exit = 0;
            delete_hash_on_exit = 0;
            csv_get_value(csv_data, r_idx, 0, &cab_name);
            csv_get_value(csv_data, r_idx, 1, &cert_key);
            csv_get_value(csv_data, r_idx, 2, &cert_file_path_csv);
            csv_get_value(csv_data, r_idx, 3, &delete_cert_on_exit_str);
            csv_get_value(csv_data, r_idx, 4, &hash_file_path_csv);
            csv_get_value(csv_data, r_idx, 5, &delete_hash_on_exit_str);

            cabhash_get_file_and_operation(obj, cert_file_path_csv,
                delete_cert_on_exit_str,
                &cert_file_path, &delete_cert_on_exit);

            cabhash_get_file_and_operation(obj, hash_file_path_csv,
                delete_hash_on_exit_str,
                &hash_file_path, &delete_hash_on_exit);
            if (cab_name && cert_key &&
                cert_file_path && hash_file_path) {
                result = cabhash_add_entry(obj,
                    cab_name, cert_key,
                    cert_file_path, delete_cert_on_exit,
                    hash_file_path, delete_hash_on_exit);
            }

            cabhash_free(cert_file_path);
            cabhash_free(hash_file_path);
            {
                size_t idx;
                char* csv_strs[] = {
                    cert_file_path_csv,
                    hash_file_path_csv,
                    delete_cert_on_exit_str,
                    delete_hash_on_exit_str,
                    cert_key,
                    cab_name
                };
                for (idx = 0;
                    idx < sizeof(csv_strs)/ sizeof(csv_strs[0]); idx++) {
                    if (csv_strs[idx]) {
                        csv_free_obj(csv_data, csv_strs[idx]);
                    }
                }
            }
            if (result) {
                break;
            }
        }
    } else {
        errno = EINVAL;
        result = -1;
    }
    return result;
}


/**
 * get file path and delete on exit operation flag.
 * if file_path is null or empty, then create temporary file deleted on exit.
 */
int
cabhash_get_file_and_operation(
    cabhash* obj,
    const char* file_path,
    const char* delete_on_exit_str,
    char** file_path_actual,
    int* delete_on_exit)
{
    int result;
    size_t file_path_len;
    result = 0;
    file_path_len = strlen(file_path);
    if (file_path && file_path_len) {
        char* file_path_d;
        file_path_d = (char*)cabhash_alloc(file_path_len + 1);
        result = file_path_d ? 0 : -1;
        if (result == 0) {
            int delete_opt;
            memcpy(file_path_d, file_path, file_path_len + 1);
            delete_opt = 0;
            number_parser_str_to_int(delete_on_exit_str, 0, &delete_opt);
            *file_path_actual = file_path_d;
            *delete_on_exit = delete_opt;
        }
    } else {
        char* tmp_file;
        tmp_file = cabhash_create_temporary_file_path(obj);

        if (tmp_file) {
            *file_path_actual = tmp_file;
            *delete_on_exit = 1;
        } else {
            result = -1;
        }
    } 

    return result;
}

/**
 * create temporary file path
 */
char*
cabhash_create_temporary_file_path(
    cabhash* obj)
{
    wchar_t* tmp_name_buffer_w;
    char* result;
    result = NULL;
    tmp_name_buffer_w = (wchar_t*)cabhash_alloc(
        sizeof(wchar_t) * PATH_MAX); 
    if (tmp_name_buffer_w) {
        size_t idx;

        for (idx = 0; idx < TMP_MAX; idx++) {
            wchar_t* tmp_path_w;
            tmp_path_w = _wtmpnam(tmp_name_buffer_w);

            if (tmp_path_w) {
                int fd;
                fd = _wopen(tmp_path_w,
                        _O_CREAT | _O_RDWR | _O_EXCL, 
                        _S_IWRITE | _S_IREAD);
                if (fd) {
                    _close(fd); 
                    result = str_conv_utf16_to_utf8(
                        tmp_path_w, wcslen(tmp_path_w) + 1,
                        cabhash_alloc, cabhash_free);

                    break;
                }
            } else {
                break;
            } 
        }

    }
    if (tmp_name_buffer_w) {
        cabhash_free(tmp_name_buffer_w);
    }
    return result;
}


/**
 * create temporary file path
 */
wchar_t*
cabhash_create_temporary_file_path_1(
    cabhash* obj)
{
    wchar_t* result;
    char* tmp_file_path;
    result = NULL;
    tmp_file_path = cabhash_create_temporary_file_path(obj);

    if (tmp_file_path) {
        result = (wchar_t*)str_conv_utf8_to_utf16(
            tmp_file_path, strlen(tmp_file_path) + 1,
            cabhash_alloc, cabhash_free);
    }
    

    cabhash_free(tmp_file_path);
    return result;
}


MSIHANDLE
cabhash_open_msi(
    cabhash* obj)
{
    MSIHANDLE result;
    UINT u_state;
    result = 0;
    u_state = 0;
    u_state = MsiOpenDatabaseW(obj->msi_path, 
        (LPCWSTR)MSIDBOPEN_TRANSACT, &result);


    return result;
}

/**
 * load csv for extracting code signed data
 */
csv*
cabhash_load_csv_from_byte_array(
    cabhash* obj,
    const char* data,
    size_t data_length)
{
    csv* result;
    result = csv_create_1(cabhash_alloc_1, cabhash_free);

    if (result) {
        int status;
        const static char* NEW_LINES[] = {
            "\r\n",
            "\n"
        };
        status = csv_load(result,
            data, data_length,
            ",", NEW_LINES,
            sizeof(NEW_LINES) / sizeof(NEW_LINES[0]));
    }
    return result;
}

/**
 * load certiicate, hash blob with current entry
 */
int
cabhash_load_cert_hash_blob(
    cabhash* obj)
{
    int result;
    if (obj) {
        cabhash_entry_iter_state state;
        memset(&state, 0, sizeof(state));
        state.cabhash_obj = obj;
        result = col_list_forward_iterate(
            obj->cab_entries, 
            (int (*)(void*, const void*))cabhash_entry_iterator,
            &state);
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * iterate entries for creating cert blob map
 */
int
cabhash_entry_iterator(
    cabhash_entry_iter_state* iter_state,
    cabhash_entry* entry)
{
    int result;
    wchar_t* cab_path;
    cabhash_cert_hash_blob* cert_hash_blob;
    result = 0;
    cert_hash_blob = NULL;

    cab_path = cabhash_create_cabinet_path_1(
        iter_state->cabhash_obj, entry->cab_name);

    result = cab_path ? 0 : -1;
    if (result == 0) {
        int state;
        cert_hash_blob = cabhash_cert_hash_blob_create_from_cab_file(
            cab_path);
        state = cert_hash_blob ? 0 : -1;
        if (state == 0) {
            result = cabhash_put_cert_hash_blob(
                iter_state->cabhash_obj,
                entry->cab_name, cert_hash_blob);
        }
        if (cert_hash_blob) {
            cabhash_cert_hash_blob_release(cert_hash_blob);
        }
    }

    if (cab_path) {
        cabhash_free(cab_path);
    }

    return result;
}


/**
 * create cabinet path
 */
wchar_t*
cabhash_create_cabinet_path(
    cabhash* obj,
    const wchar_t* cab_name)
{
    wchar_t* result; 
    int state;
    result = NULL;
    state = 0;
    if (obj->cab_file_dir) {
        HRESULT h_res;
        LPWSTR combined_path;
        combined_path = NULL;
        h_res = PathAllocCombine(
            obj->cab_file_dir,
            cab_name, PATHCCH_ALLOW_LONG_PATHS, &combined_path);
        if (SUCCEEDED(h_res)) {
            size_t path_len;
            path_len = wcslen(combined_path);
            result = cabhash_alloc((path_len + 1) * sizeof(wchar_t));
            if (result) {
                memcpy(result,
                    combined_path, (path_len + 1) * sizeof(wchar_t));
            }
            LocalFree(combined_path);
        }
    }
    return result;
}

/**
 * create cabinet path
 */
wchar_t*
cabhash_create_cabinet_path_1(
    cabhash* obj,
    const char* cab_name)
{
    wchar_t* result;
    wchar_t* cab_name_w;
    int state;
    result = NULL;
    cab_name_w = NULL;
    state = 0;
    cab_name_w = (wchar_t*)str_conv_utf8_to_utf16(cab_name,
        strlen(cab_name) + 1,
        cabhash_alloc, cabhash_free);
    state = cab_name_w ? 0 : -1;
    if (state == 0) {
        result = cabhash_create_cabinet_path(obj, cab_name_w);
    } 

    cabhash_free(cab_name_w);
    return result;
}

/**
 * put cert and hash
 */
int
cabhash_put_cert_hash(
    cabhash* obj,
    const char* cab_name,
    const void* hash,
    size_t hash_size,
    const void* cert,
    size_t cert_size)
{
    int result;
    cabhash_cert_hash_blob* blob;

    blob = cabhash_cert_hash_blob_create_1(
        cert, cert_size, hash, hash_size);
    result = blob ? 0 : -1;
    if (result == 0) {
        result = cabhash_put_cert_hash_blob(obj, cab_name, blob);
    }

    if (blob) {
        cabhash_cert_hash_blob_release(blob);
    }

    return result;
}

/**
 * put cert and hash
 */
int
cabhash_put_cert_hash_blob(
    cabhash* obj,
    const char* cab_name,
    cabhash_cert_hash_blob* cert_hash_blob)
{
    int result;
    cstr* cab_name_key;

    cab_name_key = cstr_create_02(cab_name, cabhash_alloc_1, cabhash_free);    
    result = cab_name_key ? 0 : -1;

    if (result == 0) {
        result = col_map_put(
            obj->cab_cert_hash_blob, cab_name_key, cert_hash_blob);
    }

    if (cab_name_key) {
        cstr_release(cab_name_key);
    }
    return result;
}


/**
 * get cert and hash
 */
int
cabhash_get_cert_hash_blob(
    cabhash* obj,
    const char* cab_name,
    cabhash_cert_hash_blob** cert_hash_blob)
{
    int result;
    cstr* cab_name_key;

    cab_name_key = cstr_create_02(cab_name, cabhash_alloc_1, cabhash_free);    
    result = cab_name_key ? 0 : -1;

    if (result == 0) {
        result = col_map_get(
            obj->cab_cert_hash_blob, cab_name_key, (void**)cert_hash_blob);
    }

    if (cab_name_key) {
        cstr_release(cab_name_key);
    }
    return result;
}

/**
 * iterate entries on exit
 */
int
cabhash_iterate_entries_on_exit(
    cabhash* obj)
{
    int result;
    if (obj) {
        cabhash_entry_iter_exit_state state;
        result = 0;
        memset(&state, 0, sizeof(state));
        state.cabhash_obj = obj;
        result = col_list_forward_iterate(obj->cab_entries, 
            (int (*)(void*, const void*))
                cabhash_entry_iterator_for_exit,
            &state); 
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * iterate entries for exit
 */
int
cabhash_entry_iterator_for_exit(
    cabhash_entry_iter_exit_state* state,
    cabhash_entry* entry)
{
    int result;
    result = 0;

    if (entry->cert_file_delete_on_exit) {
        wchar_t* file_path_w;
        file_path_w = cabhash_entry_get_cert_file_path_as_utf16(entry);
        if (file_path_w) {
            _wremove(file_path_w);
        }
        cabhash_free(file_path_w);
    }
    if (entry->hash_file_delete_on_exit) {
        wchar_t* file_path_w;
        file_path_w = cabhash_entry_get_hash_file_path_as_utf16(entry);
        if (file_path_w) {
            _wremove(file_path_w);
        }
        cabhash_free(file_path_w);
    }
    return result;
}


/**
 * load csv from input file
 */
static csv*
cabhash_load_csv_from_input(
    cabhash* obj)
{
    csv* result;
    result = NULL;

    if (obj && obj->input_file) {
        size_t input_data_size;
        char* input_data;
        int state;
        input_data_size = 0;
        input_data = NULL;
        state = cabhash_read_all_input(obj, &input_data, &input_data_size);
        if (state == 0) {
            result = cabhash_load_csv_from_byte_array(obj,
                input_data, input_data_size);
        }
        cabhash_free(input_data);
    } else {
        errno = EINVAL;
    }

    return result;
}

/**
 * read all from input
 */
static int
cabhash_read_all_input(
    cabhash* obj,
    char** data,
    size_t* data_size)
{
    int result;
    result = 0;
    if (obj && obj->input_file) {
        FILE* fs;
        fs = _wfopen(obj->input_file, L"r");
        if (fs) {
            result = read_all(data, data_size, fs);
        }

        if (fs) {
            fclose(fs);
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;

}

/**
 * read all data into memory
 */
static int
read_all(
    char** data,
    size_t* data_size,
    FILE* fs)
{
    buffer_char_buffer* contents_buffer;
    int result;
    const static size_t tmp_buffer_size = 1024;
    char* tmp_buffer;
    result = 0;
    tmp_buffer = NULL;
    contents_buffer = buffer_char_buffer_create(10);
    result = contents_buffer ? 0 : -1;
    if (result == 0) {
        tmp_buffer = (char*)cabhash_alloc(tmp_buffer_size);
        result = tmp_buffer ? 0 : -1;
    }
    if (result == 0) {
        while (1) {
            size_t read_size;
            read_size = fread(tmp_buffer, 1, tmp_buffer_size, fs);

            if (read_size) {
                result = buffer_char_buffer_append(
                    contents_buffer, tmp_buffer, read_size);
            } else {
                break;
            }
            if (result) {
                break;
            }
        }
    }
    if (result == 0) {
        char* contents;
        contents = (char*)cabhash_alloc(
            buffer_char_buffer_get_size(contents_buffer));
        if (contents) {
            buffer_char_buffer_copy_to(contents_buffer, contents);
            *data = contents;
            *data_size = buffer_char_buffer_get_size(contents_buffer);
        } else {
            result = -1;
        }
    }

    if (contents_buffer) {
        buffer_char_buffer_release(contents_buffer);
    }

    if (tmp_buffer) {
        cabhash_free(tmp_buffer);
    }
    return result;
}

/**
 * create entry
 */
cabhash_entry*
cabhash_entry_create()
{
    cabhash_entry* result;
    result = (cabhash_entry*)cabhash_alloc(sizeof(*result));

    if (result) {
        result->ref_count = 1;
        result->cab_name = NULL;
        result->cert_key = NULL;
        result->cert_file_path = NULL;
        result->cert_file_delete_on_exit = 0;
        result->hash_file_path = NULL;
        result->hash_file_delete_on_exit = 0;
    }
    return result;
}

/**
 * create entry
 */
cabhash_entry*
cabhash_entry_create_1(
    const char* cab_name,
    const char* cert_key,
    const char* cert_file_path,
    int delete_cert_file_on_exit,
    const char* hash_file_path,
    int delete_hash_file_on_exit)
{
    cabhash_entry* result;
    int state;
    result = cabhash_entry_create();
    state = result ? 0 : -1;
    if (state == 0) {
        state = cabhash_entry_set_cab_name(result, cab_name);
    }
    if (state == 0) {
        state = cabhash_entry_set_cert_key(result, cert_key);
    }
    if (state == 0) {
        state = cabhash_entry_set_cert_file_path(
            result, cert_file_path, delete_cert_file_on_exit);
    }
    if (state == 0) {
        state = cabhash_entry_set_hash_file_path(
            result, hash_file_path, delete_hash_file_on_exit);
    }
    if (state) {
        if (result) {
            cabhash_entry_release(result);
            result = NULL;
        }
    }
    return result;
}


/**
 * increment reference count
 */
unsigned int
cabhash_entry_retain(
    cabhash_entry* obj)
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
cabhash_entry_release(
    cabhash_entry* obj)
{
    unsigned int result;
    result = 0;
    if (obj) {
        result = --obj->ref_count;
        if (result == 0) {
            cabhash_entry_set_cab_name(obj, NULL);
            cabhash_entry_set_cert_key(obj, NULL);
            cabhash_free(obj);
        }
    } else {
        errno = EINVAL;
    }

    return result;
}


/**
 * copy reference
 */
int
cabhash_entry_copy_ref(
    cabhash_entry* src,
    cabhash_entry** dst)
{
    int result;
    result = 0;
    if (src && dst) {
        cabhash_entry_retain(src);
    }
    if (dst) {
        *dst = src;
    }

    return result;
}

/**
 * calcurate hash code
 */
int
cabhash_entry_hash(
    cabhash_entry* obj)
{
    int result;
    result = 0;
    if (obj) {
        if (obj->cab_name) {
            result ^= str_hash(obj->cab_name, strlen(obj->cab_name));
        }
        if (obj->cert_key) {
            result ^= str_hash(obj->cert_key, strlen(obj->cert_key));
        }
        if (obj->cert_file_path) {
            result ^= str_hash(obj->cert_file_path,
                strlen(obj->cert_file_path));
        }
        if (obj->hash_file_path) {
            result ^= str_hash(obj->hash_file_path,
                strlen(obj->hash_file_path));
        }
    }
    return result;
}



/**
 * set cabinet file name 
 */
int
cabhash_entry_set_cab_name(
    cabhash_entry* obj,
    const char* cab_name)
{
    int result;
    result = 0;
    if (obj) {
        if (obj->cab_name != cab_name) {
            char* new_cab_name;
            new_cab_name = NULL;
            if (cab_name) {
                size_t len;
                len = strlen(cab_name);
                new_cab_name = (char*)cabhash_alloc(len + 1);
                result = new_cab_name ? 0 : -1;
                if (result == 0) {
                    memcpy(new_cab_name, cab_name, len + 1);
                }
            }
            if (result == 0) {
                cabhash_free(obj->cab_name);
                obj->cab_name = new_cab_name;
            }
        }
    } else {
        errno = EINVAL;
        result = -1;
    }
    return result;
}



/**
 * set certificate table key
 */
int
cabhash_entry_set_cert_key(
    cabhash_entry* obj,
    const char* cert_key)
{
    int result;
    result = 0;
    if (obj) {
        if (obj->cert_key != cert_key) {
            char* new_cert_key;
            new_cert_key = NULL;
            if (cert_key) {
                size_t len;
                len = strlen(cert_key);
                new_cert_key = (char*)cabhash_alloc(len + 1);
                result = new_cert_key ? 0 : -1;
                if (result == 0) {
                    memcpy(new_cert_key, cert_key, len + 1);
                }
            }
            if (result == 0) {
                cabhash_free(obj->cert_key);
                obj->cert_key = new_cert_key;
            }
        }
    } else {
        errno = EINVAL;
        result = -1;
    }
    return result;
}

/**
 * set cert file name
 */
int
cabhash_entry_set_cert_file_path(
    cabhash_entry* obj,
    const char* cert_file_path,
    int delete_on_exit)
{
    int result;
    result = 0;
    if (obj) {
        if (obj->cert_file_path != cert_file_path) {
            char* new_cert_file_path;
            new_cert_file_path= NULL;
            if (cert_file_path) {
                size_t len;
                len = strlen(cert_file_path);
                new_cert_file_path= (char*)cabhash_alloc(len + 1);
                result = new_cert_file_path ? 0 : -1;
                if (result == 0) {
                    memcpy(new_cert_file_path, cert_file_path, len + 1);
                }
            }
            if (result == 0) {
                cabhash_free(obj->cert_file_path);
                obj->cert_file_path = new_cert_file_path;
                obj->cert_file_delete_on_exit = delete_on_exit;
            }
        }
    } else {
        errno = EINVAL;
        result = -1;
    }
    return result;
}

/**
 * set hash path
 */
int
cabhash_entry_set_hash_file_path(
    cabhash_entry* obj,
    const char* hash_file_path,
    int delete_on_exit)
{
    int result;
    result = 0;
    if (obj) {
        if (obj->hash_file_path != hash_file_path) {
            char* new_hash_file_path;
            new_hash_file_path = NULL;
            if (hash_file_path) {
                size_t len;
                len = strlen(hash_file_path);
                new_hash_file_path= (char*)cabhash_alloc(len + 1);
                result = new_hash_file_path ? 0 : -1;
                if (result == 0) {
                    memcpy(new_hash_file_path, hash_file_path, len + 1);
                }
            }
            if (result == 0) {
                cabhash_free(obj->hash_file_path);
                obj->hash_file_path = new_hash_file_path;
                obj->hash_file_delete_on_exit = delete_on_exit;
            }
        }
    } else {
        errno = EINVAL;
        result = -1;
    }
    return result;
}

/**
 * get cabinet file name as utf16
 */
wchar_t*
cabhash_entry_get_cab_name_as_utf16(
    cabhash_entry* obj)
{
    wchar_t* result;
    result = NULL;
    if (obj) {
        if (obj->cab_name) {
            result = (wchar_t*)str_conv_utf8_to_utf16(
                obj->cab_name, strlen(obj->cab_name),
                cabhash_alloc, cabhash_free);
        }
    } else {
        errno = EINVAL;
    }
    return result;
}

/**
 * get certification table key as utf16
 */
wchar_t*
cabhash_entry_get_cert_key_as_utf16(
    cabhash_entry* obj)
{
    wchar_t* result;
    result = NULL;
    if (obj) {
        if (obj->cert_key) {
            result = (wchar_t*)str_conv_utf8_to_utf16(
                obj->cert_key, strlen(obj->cert_key),
                cabhash_alloc, cabhash_free);
        }
    } else {
        errno = EINVAL;
    }
    return result;
}

/**
 * get cert file path as utf16
 */
wchar_t*
cabhash_entry_get_cert_file_path_as_utf16(
    cabhash_entry* obj)
{
    wchar_t* result;
    result = NULL;
    if (obj) {
        if (obj->cert_file_path) {
            result = (wchar_t*)str_conv_utf8_to_utf16(
                obj->cert_file_path,
                strlen(obj->cert_file_path) + 1,
                cabhash_alloc, cabhash_free);
        }
    } else {
        errno = EINVAL;
    }
    return result;
}

/**
 * get hash file path as utf16
 */
wchar_t*
cabhash_entry_get_hash_file_path_as_utf16(
    cabhash_entry* obj)
{
    wchar_t* result;
    result = NULL;
    if (obj) {
        if (obj->hash_file_path) {
            result = (wchar_t*)str_conv_utf8_to_utf16(
                obj->hash_file_path,
                strlen(obj->hash_file_path) + 1,
                cabhash_alloc, cabhash_free);
        }
    } else {
        errno = EINVAL;
    }
    return result;
}

/**
 * create certificate-hash object
 */
cabhash_cert_hash_blob*
cabhash_cert_hash_blob_create()
{
    cabhash_cert_hash_blob* result;
    result = (cabhash_cert_hash_blob*)cabhash_alloc(sizeof(*result));
    if (result) {
        result->ref_count = 1;
        result->cert = NULL;
        result->cert_size = 0;
        result->hash = NULL;
        result->hash_size = 0;
    }
    return result;
}

/**
 * create certificate-hash object
 */
cabhash_cert_hash_blob*
cabhash_cert_hash_blob_create_1(
    const void* cert,
    size_t cert_size,
    const void* hash,
    size_t hash_size)
{
    cabhash_cert_hash_blob* result;
    result = cabhash_cert_hash_blob_create();
    if (result) {
        int state;
        state = cabhash_cert_hash_blob_set_cert(result, cert, cert_size);
        if (state == 0) {
            state = cabhash_cert_hash_blob_set_hash(result, hash, hash_size);
        }
        if (state) {
            cabhash_cert_hash_blob_release(result);
            result = NULL;
        }
    }
    return result;
}

/**
 * create cert hash blob from cab file
 */
cabhash_cert_hash_blob*
cabhash_cert_hash_blob_create_from_cab_file(
    const wchar_t* signed_path)
{
    const CERT_CONTEXT* cert_ctx;
    void* hash;
    int small_hash[] = { 0 };
    DWORD dw_hash_size;
    HRESULT h_res;
    int status;
    cabhash_cert_hash_blob* result;
    dw_hash_size = 0;
    hash = NULL;
    cert_ctx = NULL;
    result = NULL;
    h_res = MsiGetFileSignatureInformationW(signed_path,
        MSI_INVALID_HASH_IS_FATAL,
        &cert_ctx, (void*)small_hash, &dw_hash_size);
    status = h_res == HRESULT_FROM_WIN32(ERROR_MORE_DATA) ? 0 : -1;

    if (cert_ctx) {
        CertFreeCertificateContext(cert_ctx);
        cert_ctx = NULL;
    }

    if (status == 0) {
        hash = cabhash_alloc(dw_hash_size);
        status = hash ? 0 : -1; 
    }
    if (status == 0) {
        h_res = MsiGetFileSignatureInformationW(signed_path,
            MSI_INVALID_HASH_IS_FATAL,
            &cert_ctx, (LPBYTE)hash, &dw_hash_size);
        status = h_res == ERROR_SUCCESS ? 0 : -1;
    }

    if (status == 0) {
        result = cabhash_cert_hash_blob_create_1(
            cert_ctx->pbCertEncoded,
            cert_ctx->cbCertEncoded,
            hash,
            dw_hash_size);
    }

    if (cert_ctx) {
        CertFreeCertificateContext(cert_ctx);
    }
    if (hash) {
        cabhash_free(hash);
    }

    return result;
}


/**
 * increment reference count
 */
unsigned int
cabhash_cert_hash_blob_retain(
    cabhash_cert_hash_blob* obj)
{
    unsigned int result;
    result = 0;
    if (obj) {
        result = ++obj->ref_count;
    }
    return result;
}

/**
 * decrement reference count
 */
unsigned int
cabhash_cert_hash_blob_release(
    cabhash_cert_hash_blob* obj)
{
    unsigned int result;
    result = 0;
    if (obj) {
        result = --obj->ref_count;
        if (result == 0) {
            cabhash_cert_hash_blob_set_cert(obj, NULL, 0);
            cabhash_cert_hash_blob_set_hash(obj, NULL, 0);
            cabhash_free(obj);
        }
    }
    return result;
}

/**
 * copy reference
 */
int
cabhash_cert_hash_blob_copy_ref(
    cabhash_cert_hash_blob* src,
    cabhash_cert_hash_blob** dest)
{
    int result;
    result = 0;
    if (src && dest) {
        cabhash_cert_hash_blob_retain(src);
    }
    if (dest) {
        *dest = src;
    }
    return result;
}

/**
 * set certificatte
 */
int
cabhash_cert_hash_blob_set_cert(
    cabhash_cert_hash_blob* obj,
    const void* cert,
    size_t cert_size)
{
    int result;
    result = 0;
    if (obj) {
        if (obj->cert != cert) {
            void* new_cert;
            new_cert = NULL;
            if (cert) {
                new_cert = cabhash_alloc(cert_size);
                result = new_cert ? 0 : -1;
                if (result == 0) {
                    memcpy(new_cert, cert, cert_size);
                }
            } else {
                cert_size = 0;
            }
            if (result == 0) {
                cabhash_free(obj->cert);
                obj->cert = new_cert;
                obj->cert_size = cert_size;
            } 
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * set hash 
 */
int
cabhash_cert_hash_blob_set_hash(
    cabhash_cert_hash_blob* obj,
    const void* hash,
    size_t hash_size)
{
    int result;
    result = 0;
    if (obj) {
        if (obj->hash != hash) {
            void* new_hash;
            new_hash = NULL;
            if (hash) {
                new_hash = cabhash_alloc(hash_size);
                result = new_hash ? 0 : -1;
                if (result == 0) {
                    memcpy(new_hash, hash, hash_size);
                }
            } else {
                hash_size = 0;
            }
            if (result == 0) {
                cabhash_free(obj->hash);
                obj->hash = new_hash;
                obj->hash_size = hash_size;
            } 
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * calc hash code
 */
unsigned int
cabhash_cert_hash_blob_hash(
    cabhash_cert_hash_blob* obj)
{
    int result;
    result = 0;
    if (obj) {
        if (obj->cert) {
            result ^= str_hash((const char*)obj->cert, obj->cert_size);
        }
        if (obj->hash) {
            result ^= str_hash((const char*)obj->hash, obj->hash_size);
        }
    }
    return result;
}

/**
 * save certificate into file
 */
int
cabhash_cert_hash_blob_save_cert(
    cabhash_cert_hash_blob* obj,
    const char* file_path)
{
    int result; 
    result = 0;
    if (obj && file_path && obj->cert && obj->cert_size) {
        wchar_t* file_path_w;
        file_path_w = str_conv_utf8_to_utf16(
            file_path, strlen(file_path) + 1, cabhash_alloc, cabhash_free);
        if (file_path_w) {
            FILE* fs;
            fs = _wfopen(file_path_w, L"w");
            result = fs ? 0 : -1;
            if (result == 0) {
                size_t written_size;
                written_size = fwrite(obj->cert, 1, obj->cert_size, fs);
                result = obj->cert_size == written_size ? 0 : -1;
            }
            if (fs) {
                fclose(fs);
            }
        } else {
            result = -1;
        }
    } else {
        result = -1;
        errno = EINVAL;
    }

    return result;
}

/**
 * save certificate into file
 */
int
cabhash_cert_hash_blob_save_hash(
    cabhash_cert_hash_blob* obj,
    const char* file_path)
{
    int result; 
    result = 0;
    if (obj && file_path && obj->hash && obj->hash_size) {
        wchar_t* file_path_w;
        file_path_w = str_conv_utf8_to_utf16(
            file_path, strlen(file_path) + 1, cabhash_alloc, cabhash_free);
        if (file_path_w) {
            FILE* fs;
            fs = _wfopen(file_path_w, L"w");
            result = fs ? 0 : -1;
            if (result == 0) {
                size_t written_size;
                written_size = fwrite(obj->hash, 1, obj->hash_size, fs);
                result = obj->hash_size == written_size ? 0 : -1;
            }
            if (fs) {
                fclose(fs);
            }
        } else {
            result = -1;
        }
    } else {
        result = -1;
        errno = EINVAL;
    }

    return result;
}


/**
 * calculate string hash
 */
static int
str_hash(
    const char* str,
    size_t length)
{
    int result;
    size_t i;
    size_t size_int;
    result = 0;
    size_int = length / sizeof(int);
    for (i = 0; i < size_int; i++) {
        result ^= *((int*)&str[i * sizeof(int)]);
    }
    {
        int tmp_value;
        tmp_value = 0;
        for (i = 0; i < length % sizeof(int); i++) {
            tmp_value += str[size_int * sizeof(int) + i];
        }
        result ^= tmp_value;
    }

    return result;
}


/**
 * allocate memory
 */
static void*
cabhash_alloc(
    size_t size)
{
    return malloc(size);
}

/**
 * allocate memory
 */
void*
cabhash_alloc_1(
    unsigned int size)
{
    return cabhash_alloc(size);
}

/**
 * free memory
 */
static void
cabhash_free(
    void* heap_obj)
{
    free(heap_obj);
}

int
wmain(
    int argc,
    wchar_t** argv)
{

    int result;
    cabhash cabhash_obj;
    result = 0;
    result = cabhash_init(&cabhash_obj);

    if (result == 0) {
        result = cabhash_parse_option(&cabhash_obj, argc, argv);
    }
    if (result == 0) {
        result = cabhash_run(&cabhash_obj);
    }

    cabhash_tear_down(&cabhash_obj);
    return result;
}

/* vi: se ts=4 sw=4 et: */

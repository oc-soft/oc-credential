#ifndef __FILE_DESC_H__
#define __FILE_DESC_H__

#ifdef __cplusplus
#define _FILE_DESC_ITFC_BEGIN extern "C" {
#define _FILE_DESC_ITFC_END }
#else
#define _FILE_DESC_ITFC_BEGIN 
#define _FILE_DESC_ITFC_END 
#endif

_FILE_DESC_ITFC_BEGIN 

/**
 * file descritor object
 */
typedef struct _file_desc file_desc;

/**
 * file descritor virtual function table
 */
typedef struct _file_desc_vtbl file_desc_vtbl;

/**
 * file descritor object
 */
struct _file_desc {
    const file_desc_vtbl* vtbl;
};

/**
 * file descritor virtual function table
 */
struct _file_desc_vtbl {

    /**
     * increment reference count
     */
    unsigned int
    (*retain)(file_desc*);

    /**
     * decrement reference count
     */
    unsigned int
    (*release)(file_desc*);


    /**
     * get file descriptor
     */
    int (*get_desc)(file_desc*);

    /**
     * close file descriptor
     */
    int (*close)(file_desc*);
};

/**
 * increment reference count
 */
unsigned int 
file_desc_retain(
    file_desc* obj);

/**
 * decrement reference count
 */
unsigned int 
file_desc_release(
    file_desc* obj);



/**
 * get file descriptor
 */
int 
file_desc_get_desc(
    file_desc* obj);

/**
 * close file descriptor
 */
int 
file_desc_close(
    file_desc* obj);


_FILE_DESC_ITFC_END 


/* vi: se ts=4 sw=4 et: */
#endif

#ifndef __CREDENTIAL_STORAGE_KEY_ID_TABLE_H__
#define __CREDENTIAL_STORAGE_KEY_ID_TABLE_H__


#ifdef __cplusplus
#define _CREDENTIAL_STORAGE_KEY_ID_TABLE_ITFC_BEGIN extern "C" {
#define _CREDENTIAL_STORAGE_KEY_ID_TABLE_ITFC_END }
#else
#define _CREDENTIAL_STORAGE_KEY_ID_TABLE_ITFC_BEGIN 
#define _CREDENTIAL_STORAGE_KEY_ID_TABLE_ITFC_END
#endif

_CREDENTIAL_STORAGE_KEY_ID_TABLE_ITFC_BEGIN 

/**
 * query existence of table
 */
int
credential_storage_key_id_table_exists(
    const char* table_name);

_CREDENTIAL_STORAGE_KEY_ID_TABLE_ITFC_END

/* vi: se ts=4 sw=4 et: */
#endif

#ifndef __LMD_UI_READER_H__
#define __LMD_UI_READER_H__

#include "lmd_ui.h"

#ifdef __cplusplus
#define _LMD_UI_READER_ITFC_BEGIN extern "C" {
#define _LMD_UI_READER_ITFC_END }
#else
#define _LMD_UI_READER_ITFC_BEGIN 
#define _LMD_UI_READER_ITFC_END 
#endif

_LMD_UI_READER_ITFC_BEGIN 

/**
 * read number from ui
 */
int
lmd_ui_reader_get_number_from_ui(
    lmd_ui* ui,
    int* number);

_LMD_UI_READER_ITFC_END
#endif

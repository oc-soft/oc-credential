#ifndef __WASM_I_H__
#define __WASM_I_H__

#ifdef __cplusplus
#define _WASM_I_ITFC_BEGIN extern "C" {
#define _WASM_I_ITFC_END }
#else
#define _WASM_I_ITFC_BEGIN 
#define _WASM_I_ITFC_END 
#endif

_WASM_I_ITFC_BEGIN 

#ifndef EMSCRIPTEN_KEEPALIVE
#define WASM_EXPORT
#else
#define WASM_EXPORT EMSCRIPTEN_KEEPALIVE
#endif
_WASM_I_ITFC_END 

/* vi: se ts=4 sw=4 et: */
#endif

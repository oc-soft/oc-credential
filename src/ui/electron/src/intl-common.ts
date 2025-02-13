import { Buffer } from 'node:buffer'
import type { BufferRef } from 'ref'
import ref from 'ref'
import CString from './c-string'

/**
 * international common library
 */
export default class IntlCommon {
  /**
   * convert native c string represention to javascript string
   */
  static toStr(
    nativeObj: any, defaultStr: string | null): string | null {
    let result = defaultStr
    if (Buffer.isBuffer(nativeObj)) {
      const nullPtr = ref.getNullPointer(true) 
      if (ref.comparePointer(nativeObj, nullPtr) != 0) {
        const strlen = CString.strlen(nativeObj) as number
        const strBuff = Buffer.alloc(strlen + 1, 0)
        const strBuffRef = Buffer.alloc(ref.sizeof.pointer)
        const strBuffRef0 = strBuffRef as BufferRef
        strBuffRef0.writePointer(strBuff)
        ref.copyMemory(strBuffRef, nativeObj, strlen + 1)
        result = ref.readCString(strBuff, 0) 
      }
    }
    return result
  } 
}

// vi: se ts=2 sw=2 et:

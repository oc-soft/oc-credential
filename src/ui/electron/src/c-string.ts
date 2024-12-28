import { Buffer } from 'node:buffer'
import ref from 'ref'
import { Library as LibraryFFI } from 'ffi'


type CStringLibrary = {
  
 /**
  * string length
  */
 strlen(str: object): number | null | undefined
}

/**
 * manage c language string functions.
 */
export default class CString {

  /**
   * cstring library
   */
  static stringLib?: CStringLibrary

  /**
   * load c string library.
   */
  static loadStringLibraryI(
    library: string): CStringLibrary {
    const lib = LibraryFFI(library,
      {
        'strlen': [ ref.types.size_t, [ ref.refType(ref.types.char, true) ]] 
      })
    const result = {
      strlen: (str: object): number? => {
        let result = undefined
        if (Buffer.isBuffer(str) && str.type.indirection == 2) {
          const nullPtr = ref.getNullPointer(true)
          if (ref.comparePointer(str, nullPtr) != 0) {
            result = lib.strlen(str) 
          }
        } 
        return result
      }
    } as CStringLibrary 
    return result 
  } 

  /**
   * get c sttring library
   */
  static getLibrary(): CStringLibrary {
    if (!CString.stringLib) {
      CString.stringLib = CString.loadStringLibraryI('libc')
    }
    return CString.stringLib 
  }

  /**
   * get c string length from indirection 2 pointer
   */
  static strlen(str: object): number | null | undefined {
    return CString.getLibrary().strlen(str)
  }
}

// vi: se ts=2 sw=2 et: 

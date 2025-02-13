import LocaleConst from './locale-const'
import { Buffer } from 'node:buffer'
import ref from 'ref'
import type { BufferRef } from 'ref'
import { Library as LibraryFFI } from 'ffi'
import CString from './c-string'


/**
 * posix locale
 */
class LocaleConstImpl implements LocaleConst {
  LC_CTYPE = 0
  LC_NUMERIC = 1
  LC_TIME = 2
  LC_COLLATE = 3
  LC_MONETARY = 4
  LC_MESSAGES = 5
  LC_ALL = 6
  LC_PAPER = 7
  LC_NAME = 8
  LC_ADDRESS = 9
  LC_TELEPHONE = 10
  LC_MEASUREMENT = 11
  LC_IDENTIFICATION = 12
}

/**
 * locale library
 */
export class Locale {

  /**
   * locale library instance
   */
  static localeLibImpl?: LocaleLibrary 

  /**
   * load locale library
   */
  static loadLocaleLibrary(): LocaleLibrary {
    return this.loadLocaleLibraryI('libc')
  }

  /**
   * get library
   */
  static get library(): LocaleLibrary {
    let result = null
    if (!this.localeLibImpl) {
      this.localeLibImpl = this.loadLocaleLibrary()
      result = this.localeLibImpl
    }
    return result as LocaleLibrary
  }

  /**
   * set locale
   */
  setlocale(category: number,
    locale: string | null | undefined) : string | null | undefined {
    return Locale.library.setlocale(category, locale) 
  }


  /**
   * load locace library internal
   */
  static loadLocaleLibraryI(
    library: string): LocaleLibrary {
    const charType = ref.types.char
    const intType = ref.types.int
    const exCharPtr = ref.refType(charType, true)
    const charPtr = ref.refType(charType)
    const lib = LibraryFFI(library,
      {
        'setlocale': [ exCharPtr, [ intType, charPtr] ]
      })

    const result = {
      setlocale: (category: number, locale?: string | null | undefined) => {
        let localeBuf = null
        if (typeof locale === 'string') {
          localeBuf = ref.allocCString(locale.toString())
        }
        const res = lib.setlocale(category, localeBuf) 
        let result = null
        if (Buffer.isBuffer(res)) {
          const nullPtr = ref.getNullPointer(true) 
          if (ref.comparePointer(res, nullPtr) != 0) {
            const strlen = CString.strlen(res) as number
            const strBuff = Buffer.alloc(strlen + 1, 0)
            const strBuffRef = Buffer.alloc(ref.sizeof.pointer)
            const strBuffRef0 = strBuffRef as BufferRef
            strBuffRef0.writePointer(strBuff)
            ref.copyMemory(strBuffRef, res, strlen + 1)
            result = ref.readCString(strBuff, 0) 
          }
        }
        return result
      }
    } 
    return result as LocaleLibrary
  }
}

export const LocaleConstant = new LocaleConstImpl()

// vi: se ts=2 sw=2 et:

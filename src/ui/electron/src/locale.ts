import ref from 'ref'
import { Library as LibraryFFI } from 'ffi'
import os from 'node:os'
import type { BufferRef } from 'ref'
import process from 'node:process'
import { Buffer } from 'node:buffer'
import LocaleWin from './locale-win'
import LocalePosix from './locale-posix'
import LocaleCommon from './locale-common'
import CString from './c-string'


let localeConstants : LocaleCommon = LocalePosix
switch (os.platform()) {
  case 'win32':
    localeConstants = LocaleWin
}

/**
 * locale library
 */
type LocaleLibrary = {
  setlocale(category: number,
    locale: string | null | undefined) : string | null | undefined
}


export default class Locale {

  static LC_CTYPE = localeConstants.LC_CTYPE 
  static LC_NUMERIC = localeConstants.LC_NUMERIC 
  static LC_TIME = localeConstants.LC_TIME
  static LC_COLLATE = localeConstants.LC_COLLATE
  static LC_MONETARY  = localeConstants.LC_MONETARY
  static LC_ALL = localeConstants.LC_ALL
  static LC_MESSAGES = localeConstants.LC_MESSAGES

  /**
   * locale library instance
   */
  static localeLib?: LocaleLibrary 

  /**
   * load locale library
   */
  static loadLocaleLibrary(): LocaleLibrary {
    let libName = 'libc'
    switch (os.platform()) {
    case 'win32':
      libName = 'ucrtbase'
    } 
    return Locale.loadLocaleLibraryI(libName)
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
    const lib = LibraryFFI('libc',
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

  /**
   * get locale library
   */
  static getLocaleLibrary(): LocaleLibrary {
    if (!Locale.localeLib) {
      Locale.localeLib = Locale.loadLocaleLibrary()
    } 
    return Locale.localeLib
  }

  static setlocale(
    category: number,
    locale: string | undefined): string | null | undefined {
    const lib = Locale.getLocaleLibrary()
    const result = lib.setlocale(Locale.LC_MESSAGES, null)

    console.log(result)

    return result
  }
}

// vi: se ts=2 sw=2 et:

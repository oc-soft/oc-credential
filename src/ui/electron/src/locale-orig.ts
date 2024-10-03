import ref from 'ref'
import ffi from 'ffi'
import os from 'node:os'
import LocaleWin from './locale-win'
import LocalePosix from './locale-posix'
import LocaleCommon from './locale-common'

let localeConstants : LocaleCommon = LocalePosix
switch (os.platform()) {
  case 'win32':
    localeConstants = LocaleWin
}

/**
 * locale library
 */
type LocaleLibrary = {
  setlocale(category: number, locale: string | Buffer) : Buffer 
}


export default class Locale {

  static LC_CTYPE = localeConstants.LC_CTYPE 
  static LC_NUMERIC = localeConstants.LC_NUMERIC 
  static LC_TIME = localeConstants.LC_TIME
  static LC_COLLATE = localeConstants.LC_COLLATE
  static LC_MONETARY  = localeConstants.LC_MONETARY
  static LC_ALL = localeConstants.LC_ALL

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
      libName = 'ucrt'
    } 
    return Locale.loadLocaleLibraryI(libName)
  }


  /**
   * load locace library internal
   */
  static loadLocaleLibraryI(
    library: string): LocaleLibrary {
    const lib = ffi.Library(library,
      {
        'setlocale': [ ref.types.CString, [ref.types.int, ref.types.CString] ]
      })
    return lib as LocaleLibrary
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
    locale: string | undefined): string {

    console.error('start setlocale -1')
    const lib = Locale.getLocaleLibrary()

    let res : Buffer | undefined
    console.error('start setlocale -2')
    
    if (locale === undefined) {
      res = lib.setlocale(category, ref.NULL_POINTER)
    } else {
      res = lib.setlocale(category, locale) 
    }
    console.error('start setlocale -3')

    return ref.readCString(res, 0)
  }
}

// vi: se ts=2 sw=2 et:

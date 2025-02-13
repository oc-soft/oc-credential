
import os from 'node:os'

import { 
  LocaleConstant as LocaleConstantWin,
  Locale as LocaleWin
} from './locale-win'
import { 
  LocaleConstant as LocaleConstantPosix,
  Locale as LocalePosix
} from './locale-posix'
import LocaleConst from './locale-common'

let localeConstants : LocaleConst = LocaleConstantPosix
switch (os.platform()) {
  case 'win32':
    localeConstants = LocaleConstantWin
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
    let result = undefined
    switch (os.platform()) {
    case 'win32':
      result = new LocaleWin()
      break 
    default:
      result = new LocalePosix()
    } 
    return result
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
    const result = lib.setlocale(category, locale)
    return result
  }
}

// vi: se ts=2 sw=2 et:

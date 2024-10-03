import ref from 'ref'
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

  static setlocale(
    category: number,
    locale: string | undefined): string {

    console.error(ref.types.CString)

    return "locale"
  }
}

// vi: se ts=2 sw=2 et:

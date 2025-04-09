import process from 'node:process'
import LocaleConst from './locale-const'
import { Intl } from './intl-darwin'

/**
 * posix locale
 */
class LocaleConstImpl implements LocaleConst {
  LC_ALL = 0
  LC_COLLATE = 1
  LC_CTYPE = 2
  LC_MONETARY = 3
  LC_NUMERIC = 4
  LC_TIME = 5
  LC_MESSAGES = 6

}

/**
 * locale library
 */
export class Locale {

  /**
   * set locale
   */
  setlocale(category: number,
    locale: string | null | undefined) : string | null | undefined {
    return Intl.library.setlocale(category, locale) 
  }
}

export const LocaleConstant = new LocaleConstImpl()

// vi: se ts=2 sw=2 et:

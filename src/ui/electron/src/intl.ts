import { Buffer } from 'node:buffer'
import os from 'node:os'
import type { BufferRef } from 'ref'
import ref from 'ref'
import { Library as LibraryFFI } from 'ffi'
import CString from './c-string'
import { Intl as IntlPosix } from './intl-posix'
import { Intl as IntlWin } from './intl-win'
import { Intl as IntlDarwin } from './intl-darwin'
import type { IntlLibrary } from './intl-type'

/**
 * gettext international library binding.
 */
export default class Intl {

  /**
   * international library instance
   */
  static intlLib?: IntlLibrary 

  /**
   * load international library
   */
  static loadIntlLibrary(): IntlLibrary {
    let result = IntlPosix
    switch (os.platform()) {
    case 'win32':
      result = IntlWin 
      break
    case 'darwin':
      result = IntlDarwin
      break
    } 
    return result
  }


  /**
   * international library instance
   */
  static get library():IntlLibrary {
    let result = this.intlLib
    if (!result) {
      this.intlLib = this.loadIntlLibrary()
      result = this.intlLib
    }
    return result 
  }

  /**
   * set default text domain
   */
  static textdomain(domain: string | null): string {
    return this.library.textdomain(domain)
  }

  /**
   * bind text domain with specific directry
   */
  static bindtextdomain(domain: string, dir: string | null): string {
    return this.library.bindtextdomain(domain, dir)
  }

  /**
   * bind domain with a specific codeset
   */
  static bindTextdomainCodeset(
    domain: string, codeset: string | null): string {
    return this.library.bindTextdomainCodeset(domain, codeset)
  }

  /**
   * get translated message 
   */
  static gettext(msg: string): string {
    return this.library.gettext(msg)
  }
}

// vi: se ts=2 sw=2 et:

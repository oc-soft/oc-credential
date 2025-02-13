import { Buffer } from 'node:buffer'
import type { BufferRef } from 'ref'
import ref from 'ref'
import { Library as LibraryFFI } from 'ffi'
import CString from './c-string'
import type { IntlLibrary } from './intl-type'
import IntlCommon from './intl-common'

/**
 * gettext international library binding.
 */
export class Intl {

  /**
   * international library instance
   */
  static intlLib?: IntlLibrary 

  /**
   * load international library
   */
  static loadIntlLibrary(): IntlLibrary {
    let libName = 'libc'
    return this.loadIntlLibraryI(libName)
  }

  /**
   * load international library internal
   */
  static loadIntlLibraryI(
    library: string): IntlLibrary {
    const charType = ref.types.char
    const intType = ref.types.int
    const exCharPtr = ref.refType(charType, true)
    const charPtr = ref.refType(charType)
    const lib = LibraryFFI(library,
      {
        'textdomain': [ exCharPtr, [charPtr] ],
        'bindtextdomain': [ exCharPtr, [charPtr, charPtr]],
        'bind_textdomain_codeset': [ exCharPtr, [charPtr, charPtr]],
        'gettext': [ exCharPtr, [charPtr] ]
      })

    const result = {
      textdomain: (domain: string | null) => {
        let domainParam
        if (typeof domain == 'string') {
          domainParam = ref.allocCString(domain) 
        } else {
          domainParam = domain
        }
        const res = lib.textdomain(domainParam)
        return IntlCommon.toStr(res, null) as string
      },
      bindtextdomain: (domain: string, dir: string | null) => {
        let domainParam
        if (typeof domain == 'string') {
          domainParam = ref.allocCString(domain) 
        } else {
          domainParam = domain
        }
        let dirParam
        if (typeof dir == 'string') {
          dirParam = ref.allocCString(dir) 
        } else {
          dirParam = dir
        }
   
        const res = lib.bindtextdomain(domainParam, dirParam)
        return IntlCommon.toStr(res, null) as string
      },
      bindTextdomainCodeset: (domain: string, codeset: string | null) => {
        let domainParam
        if (typeof domain == 'string') {
          domainParam = ref.allocCString(domain) 
        } else {
          domainParam = domain
        }
        let codesetParam
        if (typeof codeset == 'string') {
          codesetParam = ref.allocCString(codeset) 
        } else {
          codesetParam = codeset
        }
        const res = lib.bind_textdomain_codeset(domainParam, codesetParam)
        return IntlCommon.toStr(res, null) as string
      },
      gettext: (msg: string) => {
        let msgParam
        if (typeof msg == 'string') {
          msgParam = ref.allocCString(msg) 
        } else {
          msgParam = msg
        }
        const res = lib.gettext(msgParam) 
        let result = IntlCommon.toStr(res, msg) as string
        return result
      }
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

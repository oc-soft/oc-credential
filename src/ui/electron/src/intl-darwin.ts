import process from 'node:process'
import { Buffer } from 'node:buffer'
import type { BufferRef } from 'ref'
import ref from 'ref'
import { Library as LibraryFFI } from 'ffi'
import { LibraryPath } from './library-path-darwin'
import CString from './c-string'
import type { IntlLibrary } from './intl-type'
import IntlCommon from './intl-common'


export interface IntlLibraryDarwin extends IntlLibrary {
  setlocale(category: number,
    locale?: string | null | undefined): string | null | undefined
}


/**
 * gettext international library binding.
 */
export class Intl {

  /**
   * international library instance
   */
  static intlLib?: IntlLibraryDarwin

  /**
   * load international library
   */
  static loadIntlLibrary(): IntlLibraryDarwin {
    let libName = LibraryPath.findPath('libintl.8') as string
    return this.loadIntlLibraryI(libName)
  }

  /**
   * load international library internal
   */
  static loadIntlLibraryI(
    library: string): IntlLibraryDarwin {
    const charType = ref.types.char
    const intType = ref.types.int
    const exCharPtr = ref.refType(charType, true)
    const charPtr = ref.refType(charType)
    const lib = LibraryFFI(library,
      {
        'textdomain': [ exCharPtr, [charPtr] ],
        'bindtextdomain': [ exCharPtr, [charPtr, charPtr]],
        'bind_textdomain_codeset': [ exCharPtr, [charPtr, charPtr]],
        'gettext': [ exCharPtr, [charPtr] ],
        'libintl_setlocale': [ exCharPtr, [ intType, charPtr] ]
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
        const result = IntlCommon.toStr(res, null) as string
        return result
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
        const result = IntlCommon.toStr(res, null) as string
        return result
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
        const res = lib.bind_textdomain_codeset(
          domainParam, codesetParam)
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
      },
      setlocale: (category: number, locale?: string | null | undefined) => {
        process.stderr.write("intl-darwin setlocale\n")
        let localeBuf = null
        if (typeof locale === 'string') {
          localeBuf = ref.allocCString(locale.toString())
        }
        const res = lib.libintl_setlocale(category, localeBuf) 
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
    return result 
  }


  /**
   * international library instance
   */
  static get library(): IntlLibraryDarwin {
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

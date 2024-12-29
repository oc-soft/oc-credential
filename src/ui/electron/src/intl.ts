import { Buffer } from 'node:buffer'
import os from 'node:os'
import type { BufferRef } from 'ref'
import ref from 'ref'
import { Library as LibraryFFI } from 'ffi'
import CString from './c-string'

/**
 * international libray type
 */
type IntlLibrary = {

  /**
   * set default text domain
   */
  textdomain(domain: string | null): string

  /**
   * bind text domain with specific directry
   */
  bindtextdomain(domain: string, dir: string | null): string

  /**
   * bind domain with a specific codeset
   */
  bindTextdomainCodeset(domain: string, codeset: string | null): string

  /**
   * get message text
   */
  gettext(msg: string): string

}

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
    let libName = 'libc'
    switch (os.platform()) {
    case 'win32':
      libName = 'libintllib'
    } 
    return this.loadIntlLibraryI(libName)
  }

  /**
   * convert native c string represention to javascript string
   */
  static toStr(
    nativeObj: any, defaultStr: string | null): string | null {
    let result = defaultStr
    if (Buffer.isBuffer(nativeObj)) {
      const nullPtr = ref.getNullPointer(true) 
      if (ref.comparePointer(nativeObj, nullPtr) != 0) {
        const strlen = CString.strlen(nativeObj) as number
        const strBuff = Buffer.alloc(strlen + 1, 0)
        const strBuffRef = Buffer.alloc(ref.sizeof.pointer)
        const strBuffRef0 = strBuffRef as BufferRef
        strBuffRef0.writePointer(strBuff)
        ref.copyMemory(strBuffRef, nativeObj, strlen + 1)
        result = ref.readCString(strBuff, 0) 
      }
    }
    return result
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
        return this.toStr(res, null) as string
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
        return this.toStr(res, null) as string
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
        return this.toStr(res, null) as string
      },
      gettext: (msg: string) => {
        let msgParam
        if (typeof msg == 'string') {
          msgParam = ref.allocCString(msg) 
        } else {
          msgParam = msg
        }
        const res = lib.gettext(msgParam) 
        let result = this.toStr(res, msg) as string
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

import path from 'node:path'
import fs from 'node:fs'
import { app } from 'electron'
import type { LibraryPath } from './library-path'
import { Library as LibraryFFI, LIB_EXT  } from 'ffi'


/**
 * libray path implmentation
 */
class LibraryPathImpl {

  /**
   * search paths
   */
  static searchPaths: string[] = [
    `${app.getAppPath()}/lib`,
    '/usr/lib',
    '/opt/local/lib', // macport
    '/usr/local/lib' // brew
  ]

  /**
   * find dynamic libray path
   * @param libName library name
   * @return library path to be able to load library by dlopen
   */
  findPath(libName: string): string | undefined {
    let libFileName = libName
    if (!libFileName.endsWith(LIB_EXT)) {
      libFileName += LIB_EXT
    }
    let result: string | undefined
    if (!libFileName.startsWith('/')) {
      const searchPaths = LibraryPathImpl.searchPaths
      for (let idx = 0; idx < searchPaths.length; idx++) {
        const libPath = path.join(searchPaths[idx], libFileName)
        if (fs.existsSync(libPath)) {
          result = libPath
          break
        }
      }
    } else {
      if (fs.existsSync(libFileName)) {
        result = libFileName
      }
    }
    return result
  }
}

const libraryPath = new LibraryPathImpl()


export {
  libraryPath as LibraryPath, 
  libraryPath as default 
}

// vi: se ts=2 sw=2 et:

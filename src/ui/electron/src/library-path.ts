import path from 'node:path'

import { Library as LibraryFFI } from 'ffi'



/**
 * library path utility.
 */
export type LibraryPath = {
  /**
   * find dynamic libray path
   * @param libName library name
   * @return library path to be able to load library by dlopen
   */
  findPath(libName: string): string
}

// vi: se ts=2 sw=2 et:

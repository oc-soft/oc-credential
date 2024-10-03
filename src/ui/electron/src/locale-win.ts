import LocaleCommon from './locale-common'



/**
 * windows locale
 */
export class Locale implements LocaleCommon {
  LC_ALL = 0 
  LC_COLLATE = 1
  LC_CTYPE = 2
  LC_MONETARY = 3
  LC_NUMERIC = 4
  LC_TIME = 5
}


export default new Locale()
// vi: se ts=2 sw=2 et:

import LocaleCommon from './locale-common'

/**
 * posix locale
 */
export class Locale implements LocaleCommon {
  LC_CTYPE = 0
  LC_NUMERIC = 1
  LC_TIME = 2
  LC_COLLATE = 3
  LC_MONETARY = 4
  LC_MESSAGES = 5
  LC_ALL = 6
  LC_PAPER = 7
  LC_NAME = 8
  LC_ADDRESS = 9
  LC_TELEPHONE = 10
  LC_MEASUREMENT = 11
  LC_IDENTIFICATION = 12
}

export default new Locale()
// vi: se ts=2 sw=2 et:

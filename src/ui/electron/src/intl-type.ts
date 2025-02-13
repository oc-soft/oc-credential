
/**
 * international libray type
 */
export interface IntlLibrary {

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


// vi: se ts=2 sw=2 et:

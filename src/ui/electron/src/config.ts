import * as path from 'node:path'
import * as fs from 'node:fs'

export default class Config {

  /**
   * configuration object
   */
  static configObj: { [key: string] : string } | undefined

  /**
   * get configuration value for a key
   */
  static getConfig(key: string, defaultValue: string): string {
    let config: { [key1 : string] : string } | undefined

    if (!this.configObj) {
      try {
        const contents = fs.readFileSync(
          path.join(__dirname, 'config.json'), {
            encoding: 'utf8'
          })
        this.configObj = JSON.parse(contents) as { [key: string] : string }
      } catch {
      }
    }

    let result = defaultValue
    config = this.configObj
    if (config && key in config) {
      result = config[key]
    } 
    return result
  }

  /**
   * get oauth token login url
   */
  static get tokenLoginUrl(): string {
    return this.getConfig('token-login-url',
      'https://repos.oc-soft.net/oauth-entry')
  }

  /**
   * get url to access. The url has to receive oauth token.
   */
  static get requestUrl(): string {
    return this.getConfig('request-url',
      'https://repos.oc-soft.net')
  }

  /**
   * get url to get oauth token
   */
  static get oauthTokenUrl(): string {
    return this.getConfig('oauth-token-url',
      'https://repos.oc-soft.net/oauth-token')
  }
}

// vi: se ts=2 sw=2 et:

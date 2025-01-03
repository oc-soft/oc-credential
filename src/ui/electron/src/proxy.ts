import type { ProxyConfig } from 'electron'
import { env } from 'node:process'
import { URL } from 'node:url'

/**
 * proxy entry
 */
type ProxyEntry = {
  /**
   * protocol
   */
  protocol: string
  /**
   * host name
   */
  hostname: string
  /**
   * port number
   */
  port?: number | string 
  /**
   * proxy url
   */
  url: string
  /**
   * user name
   */
  name?: string 
  /**
   * password 
   */
  password?: string
}

/**
 * name and password
 */
type NamePassword = {
  /**
   * name
   */
  name: string,
  /**
   * password
   */
  password?: string
}


/**
 * proxy configuration
 */
type Configuration = {
  /**
   * configuration for electron
   */
  config: ProxyConfig,

  /**
   * log in username and password
   */
  authentication: {
    /**
     * url to name and password dictionary
     */
    [key: string]: NamePassword
  }
}

/**
 * manage proxy
 */
class Proxy {


  /**
   * proxy configuration
   */
  static proxyConfig: Configuration | undefined

  /**
   * get proxy config
   */
  static get config(): Configuration {
    if (!this.proxyConfig) {
      this.proxyConfig = this.createProxyConfigFromEnv()
    }
    return this.proxyConfig as Configuration
  }

  /**
   * convert proxy url for electron
   */
  static convertProxyUrlForElectron(proxyUrl: string): ProxyEntry {

    const url = new URL(proxyUrl)
    
    let name = url.username
    let password = url.password

    let urlStr
    if (url.port) {
      urlStr = `${url.protocol}//${url.hostname}:${url.port}`
    } else {
      urlStr = `${url.protocol}//${url.hostname}`
    }
    
    const res = {
      hostname: url.hostname,
      protocol: url.protocol,
      url: urlStr,
      port: url.port,
      name,
      password
    } 

    return res 
  }

  /**
   * get port number from protocol
   */
  static getDefaultPort(protocol: string): number | undefined {
    let result = undefined
    switch (protocol) {
      case 'http:':
      case 'ws:':
        result = 80
        break
      case 'https:':
      case 'wss:':
        result = 443
        break
      case 'ftp:':
        result = 21
        break
    }
    return result
  }
  /**
   * update authentication
   */
  static updateAuthentication(
    entry: ProxyEntry,
    authentication: {
      [url: string]: NamePassword 
    }) {
    if (entry.name) {
      let keys = []
      if (entry.port) {
        keys.push(`${entry.hostname}:${entry.port}`)
      } else {
        keys.push(entry.hostname)
        const port = this.getDefaultPort(entry.protocol)
        if (port) {
          keys.push(`${entry.hostname}:${port}`)
        }
      } 
      keys.forEach(key => {
        authentication[key] = {
          name: entry.name as string,
          password: entry.password 
        }
      })
    }
  }
    
    
  /**
   * create proxy configuration from environment
   */
  static createProxyConfigFromEnv(): Configuration | undefined {
    let result
    const proxies = [] as string[]
    const authentication: {
      [url: string]: NamePassword
    } = {
    }
    if (env.http_proxy) {
      const proxyEntry = this.convertProxyUrlForElectron(env.http_proxy) 
      proxies.push(`http=${proxyEntry.url}`)
      this.updateAuthentication(proxyEntry, authentication)
    }
    if (env.https_proxy) {
      const proxyEntry = this.convertProxyUrlForElectron(env.https_proxy) 
      proxies.push(`https=${proxyEntry.url}`)
      this.updateAuthentication(proxyEntry, authentication)
    }
    if (env.all_proxy) {
      const proxyEntry = this.convertProxyUrlForElectron(env.all_proxy) 
      if (!env.http_proxy) {
        proxies.push(`http=${proxyEntry.url}`)
        this.updateAuthentication(proxyEntry, authentication)
      }
      if (!env.https_proxy) {
        proxies.push(`https=${proxyEntry.url}`)
        this.updateAuthentication(proxyEntry, authentication)
      }
    }
    let proxyBypassRules: string | undefined
    if (proxies.length)  {
      let noProxy = env.no_proxy
      if (!noProxy) {
        noProxy = env.NO_PROXY
      }
      if (noProxy) {
        proxyBypassRules = noProxy.toString()
      }
    }
    if (proxies.length) {
      const config: ProxyConfig= {
        mode: 'fixed_servers',
        proxyRules: proxies.join(','),
      }
      if (proxyBypassRules) {
        config.proxyBypassRules = proxyBypassRules
      }
      result = {
        config,
        authentication
      }
    }
    return result
  }
}


export {
  Proxy as default,
  Proxy
}

// vi: se ts=2 sw=2 et:

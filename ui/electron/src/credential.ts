import createCredCore from 'occredcore'
import { 
  readDescriptor, 
  writeDescriptor,
  encode as descToStr,
  decode as strToDesc 
} from './desc'
import { createWindow } from './credential-ui'
import config from './config/app.json'

/**
 * credential application 
 */
export class Credential {


  /**
   * version
   */
  get version(): string {
    return config.version
  }
  
  /**
   * constructor
   */
  constructor() {
  }


  /**
   * get git credential
   */
  async getCredential(): Promise<number> {

    let result = 0
    const credMod = await createCredCore()
    const descIn = await readDescriptor()
    let descOut = credMod.credential_get(descIn as string) 

    if (!descOut) {
      const token = await new Promise<string | undefined> (
        (resolve, reject) => {
          function tokenHdlr(token0: string){
            resolve(token0)
            win.off('close', closeHdlr)
          }
          function closeHdlr(event: Event) {
            resolve(undefined)
          }
          const win = createWindow(tokenHdlr) 
          win.on('close', closeHdlr)
      })
      if (token) {
        const desc = strToDesc(descIn as string)
        if (desc) {
          desc.password = token
          descOut = descToStr(desc) || '' 
        }
      }
    }
    if (descOut) {
      writeDescriptor(descOut)
    } else {
      result = -1
    }
    return result
  }


  /**
   * store git credential
   */
  async storeCredential(): Promise<number> {
    let result = 0
    const credMod = await createCredCore()
    const descIn = await readDescriptor()
    let state = credMod.credential_store(descIn as string) 
    result = state ? 0 : -1
    return result
   }

  /**
   * erase credential
   */
  async eraseCredential(): Promise<number> {
    let result = 0
    const credMod = await createCredCore()
    const descIn = await readDescriptor()
    let state = credMod.credential_erase(descIn as string) 
    result = state ? 0 : -1
    return result
  }
}

// vi: se ts=2 sw=2 et:

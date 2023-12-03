import { BrowserWindow } from 'electron'
import {
  Descriptor,
  encode as descToStr
} from 'oc-soft/common'
import { 
  readDescriptor, 
  writeDescriptor,
} from './desc'
import { createWindow } from './credential-ui'
import config from 'oc-soft/config/app.json'


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
   * generate token
   */
  generateToken(
    descriptor: string | undefined,
    service: string | undefined): Promise<string | undefined> {
    const self = this
    return new Promise<string | undefined> (
      (resolve, reject) => {
        function tokenHdlr(token0: string){
          resolve(token0)
          win.off('close', closeHdlr)
        }
        function handleCloseRequest(win: BrowserWindow) {
          win.close() 
        }
        function closeHdlr(event: Event) {
          resolve(undefined)
        }
        const win = createWindow(descriptor, service, 
          tokenHdlr, handleCloseRequest) 
        win.on('close', closeHdlr)
    })
  }
  /**
   * generate git credential token
   */
  async generateCredential(service: string | undefined): Promise<number> {
    let result = 0
    let descOut: string | undefined

    let descIn: string | undefined
    descIn = await readDescriptor() 

    const token = await this.generateToken(descIn, service)
    if (token) {
      const desc = {
        password: token
      } as Descriptor 
      descOut = descToStr(desc) 
    }
    if (descOut) {
      writeDescriptor(descOut)
    } else {
      result = -1
    }
    return result
  }
}

// vi: se ts=2 sw=2 et:

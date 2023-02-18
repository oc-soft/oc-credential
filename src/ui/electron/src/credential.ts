import { 
  Descriptor, 
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
   * generate token
   */
  generateToken(): Promise<string | undefined> {
    return new Promise<string | undefined> (
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
  }
  /**
   * generate git credential token
   */
  async generateCredential(): Promise<number> {
    let result = 0
    let descOut: string | undefined
    const token = await this.generateToken()
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

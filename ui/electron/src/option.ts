import { Credential as App } from './credential'
import config from './config/app.json'

/**
 * option
 */
export class Option {


  /**
   * run the option operation
   */
  run: (app: App) => Promise<number>

  /**
   * constructor
   */
  constructor() {
    this.run = this.help
  }

  /**
   * print help string
   */
  async help(app: App): Promise<number> {
    process.stderr.write(this.createHelpMessage())
    return 0
  }

  /**
   * print version
   */
  async version(app: App): Promise<number> {
    const version = `oc-credential-helper(${app.version})`
    process.stderr.write(version)
    return 0
  }

  /**
   * do run git credential get
   * @param { App } application
   */
  async get(app: App): Promise<number> {
    return app.getCredential()
  }

  /**
   * do run git credentail store
   */
  async store(app: App): Promise<number> {
    return app.storeCredential()
  }

  /**
   * do run git credentail erase
   */
  async erase(app: App): Promise<number> {
    return app.eraseCredential()
  }

  /**
   * do nothing for future usage
   */
  async futureUse(app: App): Promise<number> {
    return 0
  }

  /**
   *
   * @return helpmessage
   */
  createHelpMessage(): string {
    return `${config.name}
-h, --help    print this message
-v, --version print version
get           do run credential get 
store         do run credential store
erase         do run credential erase
`
  }

  /**
   * parse argv
   */
  parse(argv: string[]) {
    const self = this
    
    for (let idx = 0; idx < argv.length; idx++) {
      const arg = argv[idx]
      switch (arg) {
        case '-h':
        case '--help':
          self.run = self.help
          break
        case '--version':
          self.run = self.version
          break
        case 'get':
          self.run = self.get
          break
        case 'store':
          self.run = self.store
          break
        case 'erase':
          self.run = self.erase
          break
        default:
          self.run = self.futureUse
          break
      }  
    }
  }
}

// vi: se ts=2 sw=2 et:

import { Credential as App } from './credential'
import config from './config/app.json'

/**
 * electron option
 */
interface ElectronOption {
  /**
   * command line option
   */
  options: string[]
  /**
   * if the option need a value, the hasValue is true
   * if the option dose not need a value, the hasValue is false
   * if the option need a value optionally, the hasValue is null
   */
  hasValue: boolean | null 
}
/** 
 * electron predefined option
 */
const electronOptions : ElectronOption[] = [
  {
    options: ['-v', '--version'],
    hasValue: false,
  },
  {
    options: ['-a', '--abi'],
    hasValue: false
  },
  {
    options: ['-r', '--require'],
    hasValue: true
  },
  {
    options: ['-i','--interactive', '-repl'],
    hasValue: false
  },
  {
    options: ['--test-type=webdriver'],
    hasValue: false
  },
  {
    options: ['--no-help'],
    hasValue: false
  },
  {
    options: ['-'],
    hasValue: false
  }
]

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
    this.run = this.generate
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
   * generate credential token
   */
  async generate(app: App): Promise<number> {
    return await app.generateCredential()
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
`
  }

  
  /**
   * create command arguments for the app
   */
  skipElectronOptions(argv: string[]): string[] {
    const result = [] as string[]
    let idx: number = 0;
    for (idx = 0; idx < argv.length; idx++) {
      let isElectronOption = false
      electronOptions.forEach(opt => {
        isElectronOption = 
          opt.options.find(opt0 => opt0 === argv[idx]) !== undefined
        if (isElectronOption && opt.hasValue === true) {
          idx++
        }
      })
      if (!isElectronOption) {
        isElectronOption = argv[idx].startsWith('--app=') 
      }
      if (!isElectronOption) {
        // last invalid election option is file.
        // It is election entry point file.
        idx++
      }
      if (!isElectronOption) {
        break
      }
    }
    return argv.slice(idx) 
  }

  /**
   * parse argv
   */
  parse(argv: string[]) {
    const self = this
    const args = this.skipElectronOptions(argv)
    
    for (let idx = 0; idx < args.length; idx++) {
      const arg = args[idx]
      switch (arg) {
        case '-h':
        case '--help':
          self.run = self.help
          break
        case '--version':
          self.run = self.version
          break
        case 'get':
          self.run = self.generate
          break
        default:
          self.run = self.futureUse
          break
      }  
    }
  }
}

// vi: se ts=2 sw=2 et:

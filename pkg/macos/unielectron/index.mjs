import process from 'node:process'
import path from 'node:path'
import electronUniversal from '@electron/universal'

/**
 * universal electron  generator
 */
class App {

  /**
   * constructor
   */
  construtor() {
    this.doRun = this.showHelp
  }

  /**
   * run main program
   */
  async run() {
    await this.doRun()
  }

  /**
   * display help message
   */
  showHelp() {
    const msg=`node unielectron [OPTIONS]
--x64=[x64 electron path]         specify x64 electron path. 
--arm64=[arm64 electron path]     specify arm64 electron path.
--out=[universal electron path]   specify output path for universal electron.
--help                            show this message.`
    console.log(msg)
  }


  /**
   * create universal electron module
   */
  async universalElectron() {
    const param = {
      x64AppPath: this.x64,
      arm64AppPath: this.arm64,
      outAppPath: this.out
    }
    for (const key in param) {
      if (!path.isAbsolute(param[key])) {
        param[key] = path.join(process.cwd(), param[key])
      }
    }
    electronUniversal.makeUniversalApp(param)
  }

  /**
   * parse argument option
   */
  parseOption() {
    const self = this
    const strOptions=[
      [ '--x64=', str => self.x64 = str ],
      [ '--arm64=', str => self.arm64 = str ],
      [ '--out=', str => self.out = str ]
    ]
    let showHelp = false
    for (let idx = 0; idx < process.argv.length; idx++) {
      const arg=process.argv[idx]
      let optionProcessed = false
      for (let optIdx = 0; optIdx < strOptions.length; optIdx++) {
        if (arg.startsWith(strOptions[optIdx][0])
          && arg.length > strOptions[optIdx][0].length) {
          strOptions[optIdx][1](
            arg.substring(strOptions[optIdx][0].length))
          optionProcessed = true
        }
      }
      if (!optionProcessed) {
        switch (arg) {
          case '--help':
          case '-h':
            this.doRun = this.showHelp
            showHelp = true
            break
        }
      }
    }
    if (!showHelp && this.x64 != void 0
      && this.arm64 != void 0
      && this.out != void 0) {
      this.doRun = this.universalElectron
    } 
  }
}


// run application
await (async() => {
  const app = new App()
  app.parseOption()
  await app.run()
})()

// vi: se ts=2 sw=2 et:

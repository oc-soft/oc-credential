import fs from 'node:fs/promises'
import process from 'node:process'
import path from 'node:path'


let procedure

const app = {
  excludes : new Set()
}

/**
 * show help message
 */
function showHelp() {
  const scriptName = path.basename(process.argv[1])
  const msg = `${scriptName} [OPTIONS]
-h, --help                    show this message
-e, --excludes=[MODULE_NAME]  specify to exclude module 
`
  process.stdout.write(msg) 
}

/**
 * parse command line options
 */
function parseOption() {
  let mode = 'cmd'
  let cmd
  let option
  /**
   * @param {number} idx - current option index for argv
   */
  function processOption(idx) {
    if (!option) {
      option = process.argv[idx]
    }
    switch (cmd) {
      case '-e':
      case '--excludes':
        app.excludes.add(option)
        break
    } 
    mode = 'cmd'
    option = null
  }
  for (let i = 2; i < process.argv.length; i++) {
    if (mode == 'cmd') {
      let cmdAndOption = process.argv[i].split('=')
      option = cmdAndOption[1]
      cmd = cmdAndOption[0] 
      switch (cmd) {
        case '--help':
        case '-h':
          procedure = showHelp
          break
        case '-e':
        case '--excludes':
          mode = 'option'
          if (option) {
            processOption(i)
          }
          break
      }
    } else {
      processOption(i)
    }
  }
  if (mode == 'option') {
    processOption(process.argv.length)
  }
}

/**
 * resolve module dependencies 
 * @param {object} pkgLock - it keep package lock json object.
 * @param {string} module - the module to be listed up sub module
 * @param {function} iterator - called each resolved submodule
 */
function resolveModule(
  pkgLock,
  module,
  iterator) {
  const modPath = `node_modules/${module}`
  const modObj = pkgLock['packages'][modPath]
  if (modObj) {
    iterator(module, modPath, modObj)
    const deps = modObj.dependencies
    if (deps) {
      for (const depMod in deps) {
        resolveModule(pkgLock, depMod, iterator)
      }
    }
  }
}

/**
 * main procedure
 */
async function run() {
  const depends = []
  const pkgLockStr = await fs.readFile('package-lock.json', 'utf8')
  const pkgLock = JSON.parse(pkgLockStr)
  const rootDepends = pkgLock['packages'][''].dependencies
  const processed = new Set()
  for (const key in rootDepends) {
    if (!app.excludes.has(key)) {
      resolveModule(pkgLock, key, (name, modPath, mod) => {
        if (!processed.has(name) && !app.excludes.has(name)) {
          console.log(modPath)
          processed.add(name)
        }
      })
    }
  }
}

procedure = run

parseOption()

await procedure()


// vi: se ts=2 sw=2 et:

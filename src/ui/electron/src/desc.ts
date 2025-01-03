import process from 'node:process'

/**
 * descriptor keys 
 */
const descriptorKeys: Set<string> = new Set<string>([
  'protocol',
  'host',
  'path',
  'username',
  'password'
  ])


/**
 * read descriptor from stdin
 */
export async function readDescriptor(): Promise<string | undefined> {
  return new Promise<string | undefined>((resolve, reject) => {
    let buf: string | undefined
    function dataHdlr(chunk: string) {
      buf = buf || ""
      buf += chunk

      if (buf.endsWith("\n\n") || buf.endsWith("\r\n\r\n")) {
        resolve(buf)
      }
    }
    function errHdlr(err: Error) {
      process.stdin.off('error', errHdlr)
      process.stdin.off('data', dataHdlr)
      process.stdin.off('end', endHdlr)
      reject(err)
    }
    function endHdlr() {
      process.stdin.off('error', errHdlr)
      process.stdin.off('data', dataHdlr)
      process.stdin.off('end', endHdlr)
      resolve(buf)
    }
    process.stdin.on('data', dataHdlr) 
    process.stdin.on('error', errHdlr)
    process.stdin.on('end', endHdlr)
  })  
}

/**
 * write descriptor into stdout
 * @param {string} descriptor 
 */
export async function writeDescriptor(desc: string): Promise<void> {
  return await new Promise((resolve, reject) => {
    function drainHdlr () {
      resolve()
      process.stdout.off('drain', drainHdlr)
    }
    let state = process.stdout.write(desc)
    if (!state) {
      process.stdout.on('drain', drainHdlr)
    } else {
      resolve(undefined)
    }
  })
}

// vi: se ts=2 sw=2 et:

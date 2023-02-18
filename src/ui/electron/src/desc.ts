/**
 * git credential descriptor
 */
export interface Descriptor {
  protocol: string | undefined
  host: string | undefined
  path: string | undefined
  username: string | undefined
  password: string | undefined
  [key: string]: string | undefined
}

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
  return await new Promise<string | undefined>((resolve, reject) => {
    let buf: string | undefined
    function dataHdlr(chunk: string) {
      buf = buf || ""
      buf += chunk
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


/**
 * convert string to Descriptor object
 * @param {string} descriptor string representaion
 * @return {Descriptor} desriptor object
 */
export function decode(desc: string): Descriptor | undefined {
  const lines: string[] = desc.split("\n")

  let result: Descriptor | undefined
  for (const line of lines) {
    const keyEnd = line.indexOf('=')
    if (0 < keyEnd) {
      const key = line.substring(0, keyEnd)
      const value = line.substring(keyEnd + 1)
      if (descriptorKeys.has(key)) {
        result = result || {} as Descriptor
        result[key] = value
      }
    }
  }
  return result
}

/**
 * convert from Descriptor object to string 
 * @param {Descriptor} desriptor object
 * @return {string} descriptor string representaion
 */
export function encode(desc: Descriptor): string | undefined {
  let result
  const lines:string[] = []
  descriptorKeys.forEach(key => {
    if (key in desc) { 
      lines.push(`${key}=${desc[key]}`)
    }
  })
  if (lines.length) {
    result = lines.join("\n")
    result += "\n\n"
  }
  return result
}



// vi: se ts=2 sw=2 et:

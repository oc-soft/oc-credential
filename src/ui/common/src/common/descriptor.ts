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

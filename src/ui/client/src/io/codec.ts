
/**
 * decode base64 url 
 */
export function decodeBase64Url(base64Url: string): Uint8Array {

  let base64 = base64Url.replace(/-/, '+')  
  base64 = base64.replace(/_/, '/')

  const padIdx = base64.length % 4
  if (padIdx) {
    base64 = base64.padEnd(base64.length + 3 - padIdx, '=')
  }

  return Uint8Array.from(atob(base64), elem => elem.codePointAt(0))
}


// vi: se ts=2 sw=2 et:

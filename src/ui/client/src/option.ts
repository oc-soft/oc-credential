import { decodeBase64Url } from './io/codec'

import { Descriptor, decode } from 'oc-soft/common'

/**
 * get service
 */
export function getService(): string | undefined {

  let search = globalThis.document.location.search

  let result : string | undefined = undefined
  if (search) {
    searchParams = new URLSearchParams(search)
    result = searchParams.get('service')
  }
  return result
}

/**
 * get descriptor string
 */
export function getDescritorStr(): string | undefined {
  let search = globalThis.document.location.search

  let result : string | undefined = undefined
  if (search) {
    searchParams = new URLSearchParams(search)
    const desc64 = searchParams.get('desc')
    if (desc64) {
      const byteArray = decodeBase64Url(desc64)
      const decoder = new TextDecoder()
      result = decoder.decode(byteArray)
    }
  }
  return result
}  

/**
 * get descriptor
 */
export function getDescriptor(): Descriptor | undefined {

  const descStr = getDescriptorStr()
  let result: Descriptor | undefined = undefined
  if (descStr) {
    result = decode(descStr)
  }
  return result 
}


// vi: se ts=2 sw=2 et: 

import { BrowserWindow } from 'electron'
import type { UploadRawData } from 'electron'
import { bind as bindIpc } from './ipc'
import * as path from 'node:path'
import * as fs from 'node:fs'
import { Buffer } from 'node:buffer'
import Config from './config' 
import { decode } from 'oc-soft/common'

/**
 * get request url
 */
function getRequestUrl(
  descriptorStr?: string): string {
  let requestUrl: string | undefined
  if (descriptorStr) {
    const desc = decode(descriptorStr) 
    if (desc && desc.protocol && desc.host) {
      if ('http' == desc.protocol || 'https' == desc.protocol) {
        requestUrl = `${desc.protocol}://${desc.host}`
      }
    }
  }
  if (!requestUrl) {
    requestUrl = Config.requestUrl
  }
  return requestUrl as string
}
  
/**
 * create post option for load url
 */
function createPostOption(
  initialSelection?: string,
  descriptorStr?: string): object {

  const searchParams = new URLSearchParams()
  searchParams.append('url', getRequestUrl(descriptorStr))
  if (initialSelection) {
    searchParams.append('selection', initialSelection)
  } 

  const result = {
    postData: [
      {
        type: 'rawData',
        bytes: Buffer.from(searchParams.toString())
      }
    ],
    extraHeaders: [
        'Content-Type: application/x-www-form-urlencoded'
    ].join("\n") 
  }
  return result
}

/**
 * create window
 */
export function createWindow(
  descriptor: string | undefined,
  service: string | undefined,
  tokenHdlr: (token: string) => void,
  closeRequestHdlr: (win: BrowserWindow) => void): BrowserWindow {
  const result = new BrowserWindow({
    width: 800,
    height: 600,
    webPreferences: {
      preload: path.join(import.meta.dirname, 'preload.js')
    }
  })
  
  bindIpc(tokenHdlr, () => closeRequestHdlr(result))

  let loadOption: object | undefined = undefined 
    
  result.loadURL(Config.tokenLoginUrl, 
    createPostOption(service, descriptor))
  // result.webContents.openDevTools()
  return result
}

// vi: se ts=2 sw=2 et:

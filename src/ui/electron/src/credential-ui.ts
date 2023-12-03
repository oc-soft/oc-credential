import { BrowserWindow } from 'electron'
import type { UploadRawData } from 'electron'
import { bind as bindIpc } from './ipc'
import * as path from 'node:path'
import * as fs from 'node:fs'
import { Buffer } from 'node:buffer'
import Config from './config' 


/**
 * create post option for load url
 */
function createPostOption(initialSelection?: string): object {

  const searchParams = new URLSearchParams()
  searchParams.append('url', Config.requestUrl)
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
      preload: path.join(__dirname, 'preload.js')
    }
  })
  
  bindIpc(tokenHdlr, () => closeRequestHdlr(result))

  let loadOption: object | undefined = undefined 
    
  result.loadURL(Config.tokenLoginUrl, 
    createPostOption(service))
  // result.webContents.openDevTools()
  return result
}

// vi: se ts=2 sw=2 et:

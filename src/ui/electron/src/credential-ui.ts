import { BrowserWindow } from 'electron'
import { bind as bindIpc } from './ipc'
import * as path from 'path'

export function createWindow(
  tokenHdlr: (token: string)=>void): BrowserWindow {
  const result = new BrowserWindow({
    width: 800,
    height: 600,
    webPreferences: {
      preload: path.join(__dirname, 'preload.js')
    }
  })
  bindIpc(tokenHdlr)
  result.loadFile('./dist/index.html')
  // win.webContents.openDevTools()
  return result
}

// vi: se ts=2 sw=2 et:

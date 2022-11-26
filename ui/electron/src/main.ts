import { app, BrowserWindow } from 'electron'
import { bind as bindIpc } from './ipc'
import path from 'path'

const createWindow = () => {
  const win = new BrowserWindow({
    width: 800,
    height: 600,
    webPreferences: {
      preload: path.join(__dirname, 'preload.js')
    }
  })
  bindIpc()
  win.loadFile('./dist/index.html')
  win.webContents.openDevTools()
}
(async ()=> {
  await app.whenReady()
  createWindow()
})() 
// vi: se ts=2 sw=2 et:

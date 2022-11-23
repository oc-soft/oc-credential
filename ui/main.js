const { app, BrowserWindow } = require('electron')

const createWindow = () => {
  const win = new BrowserWindow({
    width: 800,
    height: 600
  })
  win.loadFile('./dist/index.html')
  win.webContents.openDevTools()
}
(async ()=> {
  await app.whenReady()
  createWindow()
})() 
// vi: se ts=2 sw=2 et:

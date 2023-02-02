import { contextBridge, ipcRenderer } from 'electron'

/**
 * bind event handler between main process and reder process
 */
function bind() {
  contextBridge.exposeInMainWorld('ocApi', {
    saveOauthToken: async (token: string)=> {
      return await ipcRenderer.invoke('save-oauth-token', token)
    }
  })
}

bind()

// vi: se ts=2 sw=2 et:

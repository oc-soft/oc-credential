import type { IpcRendererEvent } from 'electron'
import { contextBridge, ipcRenderer } from 'electron'

import type { AboutInfo } from './common-types'


/**
 * bind event handler between main process and reder process
 */
function bind() {
  contextBridge.exposeInMainWorld('ocApi', {
    saveOauthToken: async (token: string)=> {
      return await ipcRenderer.invoke('save-oauth-token', token)
    },
    requestCloseWindow: async () => {
      return await ipcRenderer.invoke('request-close-window')
    },
    onToggleAbout: async (
      callback: (info?: AboutInfo) => Promise<void>) => {
      ipcRenderer.on('toggle-about',
        async (_event: IpcRendererEvent, info?: AboutInfo) => {
        await callback(info)
      }) 
    }
  })
}

bind()

// vi: se ts=2 sw=2 et:

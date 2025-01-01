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
    onVisbleAbout: async (
      callback: (visible: boolean, info?: AboutInfo) => Promise<boolean>) => {
      ipcRenderer.on('visible-about',
        async (_event: IpcRendererEvent,
          visible: boolean, info?: AboutInfo) => {
        await callback(visible, info)
      }) 
    }
  })
}

bind()

// vi: se ts=2 sw=2 et:

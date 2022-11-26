import { ipcMain } from 'electron'
import { saveOauthToken } from './io/oauth'

/**
 * bind ipc event handlers
 */
export function bind() {
  ipcMain.handle('save-oauth-token',
    async (event, token)=> { await saveOauthToken(token) })
}

// vi: se ts=2 sw=2 et:

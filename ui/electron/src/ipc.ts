import { ipcMain } from 'electron'

/**
 * bind ipc event handlers
 */
export function bind(
  tokenHdlr: (token: string) => void) {
  ipcMain.handle('save-oauth-token',
    (event, token)=> tokenHdlr(token))
}

// vi: se ts=2 sw=2 et:

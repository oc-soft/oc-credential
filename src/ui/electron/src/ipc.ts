import { ipcMain } from 'electron'

/**
 * bind ipc event handlers
 */
export function bind(
  tokenHdlr: (token: string) => void,
  closeRequestHdlr: () => void) {
  ipcMain.handle('save-oauth-token',
    (event, token)=> tokenHdlr(token))
  ipcMain.handle('request-close-window',
    async (event) => closeRequestHdlr())
}

// vi: se ts=2 sw=2 et:

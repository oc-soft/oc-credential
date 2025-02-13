import os from 'node:os'
import { MenuLinux } from './menu-linux'
import Intl from './intl'
import type { BrowserWindow } from 'electron'
import { app } from 'electron'
import type { StartUrlParam } from './common-types'



/**
 * attach application menu
 */
export function attachMenu(
  startUrlParam: StartUrlParam,
  window?: BrowserWindow) {
  const localePath = `${app.getAppPath()}/locale`
  let localePathDbg
  localePathDbg = Intl.bindtextdomain('ui-menu', localePath)
  let codeSetDbg = Intl.bindTextdomainCodeset('ui-menu', 'UTF-8')
  let currentDomain = Intl.textdomain('ui-menu')

  switch (os.platform()) {
  case 'darwin':
    break
  case 'win32':
    break
  default:
    if (window) {
      MenuLinux.attachMenu(window, startUrlParam) 
    }
    break
  }  
}


// vi: se ts=2 sw=2 et:


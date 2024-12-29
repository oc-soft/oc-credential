import os from 'node:os'
import { MenuLinux } from './menu-linux'
import Intl from './intl'
import { app } from 'electron'



/**
 * attach application menu
 */
export function attachMenu() {
  const localePath = `${app.getAppPath()}/locale`

  let localePathDbg
  localePathDbg = Intl.bindtextdomain('ui-menu', localePath)
  console.log(localePathDbg)
  let codeSetDbg = Intl.bindTextdomainCodeset('ui-menu', 'UTF-8')
  console.log(codeSetDbg)

  switch (os.platform()) {
  case 'win32':
    break
  default:
    MenuLinux.attachMenu() 
    break
  }  
}


// vi: se ts=2 sw=2 et:

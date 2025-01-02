import type { 
  MenuItemConstructorOptions,
  BaseWindow,
  MenuItem,
  KeyboardEvent } from 'electron'
import { Menu, app, BrowserWindow } from 'electron'

import Intl from './intl'
import type { AboutInfo } from './common-types'


/**
 * attach
 */
export class MenuLinux {

  /**
   * application menu
   */
  static get menu(): Menu {
    const menuParams = [
      // { role: 'fileMenu' }
      {
        label: Intl.gettext('&File'),
        submenu: [
          {
            label: Intl.gettext('E&xit'),
            role: 'quit'
          }
        ] as MenuItemConstructorOptions[] 
      } as MenuItemConstructorOptions,
      // { role: 'editMenu' }
      {
        label: Intl.gettext('&Edit'),
        submenu: [
          {
            label: Intl.gettext('&Undo'),
            role: 'undo'
          },
          {
            label: Intl.gettext('&Redo'),
            role: 'redo'
          },
          { type: 'separator' },
          {
            label: Intl.gettext('C&ut'),
            role: 'cut'
          },
          { 
            label: Intl.gettext('&Copy'),
            role: 'copy'
          },
          {
            label: Intl.gettext('&Paste'),
            role: 'paste'
          },
          {
            label: Intl.gettext('&Delete'),
            role: 'delete'
          },
          { type: 'separator' },
          {
            label: Intl.gettext('&Select all'),
            role: 'selectAll'
          }
        ] as MenuItemConstructorOptions[] 
      } as MenuItemConstructorOptions,
      // { role: 'viewMenu' }
      {
        label: Intl.gettext('&View'),
        submenu: [
          {
            label: Intl.gettext('&Restart'),
            role: 'reload'
          },
          {
            label: Intl.gettext('R&estart no cache'),
            role: 'forceReload'
          },
          {
            label: Intl.gettext('Toggle &DevTools'),
            role: 'toggleDevTools'
          },
          { type: 'separator' },
          {
            label: Intl.gettext('Re&set zoom'),
            role: 'resetZoom'
          },
          {
            label: Intl.gettext('Zoom &in'),
            role: 'zoomIn'
          },
          {
            label: Intl.gettext('Zoom &out'),
            role: 'zoomOut'
          },
          { type: 'separator' },
          {
            label: Intl.gettext('Toggle &fullscreen'),
            role: 'togglefullscreen'
          }
        ] as MenuItemConstructorOptions[] 
      } as MenuItemConstructorOptions,

      // { role: 'windowMenu' }
      {
        label: Intl.gettext('&Window'),
        submenu: [
          {
            label: Intl.gettext('&Minimize'),
            role: 'minimize'
          },
          {
            label: Intl.gettext('&Zoom'),
            role: 'zoom'
          }
        ] as MenuItemConstructorOptions[] 
      } as MenuItemConstructorOptions,
      {
        label: Intl.gettext('&Help'),
        role: 'help',
        submenu: [
          {
            label: Intl.gettext('&ToggleAbout'),
            click: async (menuItem: MenuItem,
              window: BaseWindow | undefined,
              event: KeyboardEvent) => {
              if (window instanceof BrowserWindow) {
                const aboutInfo = {
                  version: app.getVersion()
                }
                window.webContents.send('toggle-about', aboutInfo)
              }
            }
          } 
        ]
      } as MenuItemConstructorOptions 
    ]
    return Menu.buildFromTemplate(menuParams)
  }

  /**
   * attach menu to application
   */
  static attachMenu(): void {

    Menu.setApplicationMenu(this.menu) 
    // Intl.bindtextdomain('ui-menu')
    
  }
}

// vi: se ts=2 sw=2 et:

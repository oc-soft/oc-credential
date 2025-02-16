import type { 
  MenuItemConstructorOptions,
  BaseWindow,
  MenuItem,
  KeyboardEvent } from 'electron'
import { Menu, app, BrowserWindow } from 'electron'

import Intl from './intl'
import type { AboutInfo, StartUrlParam } from './common-types'
import MenuUtil from './menu-util'
import Config from './config' 
import { loadStartUrl } from './credential-ui'

/**
 * attach menu
 */
export class MenuWin {

  /**
   * application menu
   */
  static createMenu(startUrlParam: StartUrlParam): Menu {
    const menuParams = [
      // { role: 'fileMenu' }
      {
        label: Intl.gettext('&File'),
        id: 'file',
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
        id: 'edit',
        submenu: [
          {
            label: Intl.gettext('&Undo'),
            id: 'undo',
            role: 'undo'
          },
          {
            label: Intl.gettext('&Redo'),
            id: 'redo',
            role: 'redo'
          },
          { type: 'separator' },
          {
            label: Intl.gettext('Cu&t'),
            id: 'cut',
            role: 'cut'
          },
          { 
            label: Intl.gettext('&Copy'),
            id: 'copy',
            role: 'copy'
          },
          {
            label: Intl.gettext('&Paste'),
            id: 'paste',
            role: 'paste'
          },
          {
            label: Intl.gettext('&Delete'),
            id: 'delete',
            role: 'delete'
          },
          { type: 'separator' },
          {
            label: Intl.gettext('&Select all'),
            id: 'select-all',
            role: 'selectAll'
          }
        ] as MenuItemConstructorOptions[] 
      } as MenuItemConstructorOptions,
      // { role: 'viewMenu' }
      {
        label: Intl.gettext('&View'),
        id: 'view',
        submenu: [
          {
            label: Intl.gettext('Re&start'),
            id: 'restart',
            visible: false,
            click: async (menuItem: MenuItem,
              window: BaseWindow | undefined,
              event: KeyboardEvent) => {
              if (window instanceof BrowserWindow) {
                await loadStartUrl(window, startUrlParam)
              }
            }
          },
          {
            label: Intl.gettext('&Reload'),
            visible: false,
            id: 'reload',
            role: 'reload'
          },
          {
            label: Intl.gettext('R&eload no cache'),
            visible: false,
            id: 'force-reload',
            role: 'force-reload'
          },
          {
            label: Intl.gettext('Toggle &DevTools'),
            id: 'toggle-dev-tools',
            role: 'toggleDevTools'
          },
          { type: 'separator' },
          {
            label: Intl.gettext('Re&set zoom'),
            id: 'reset-zoom',
            role: 'resetZoom'
          },
          {
            label: Intl.gettext('Zoom &in'),
            id: 'zoom-in',
            role: 'zoomIn'
          },
          {
            label: Intl.gettext('Zoom &out'),
            id: 'zoom-out',
            role: 'zoomOut'
          },
          { type: 'separator' },
          {
            label: Intl.gettext('Toggle &fullscreen'),
            id: 'togglefullscreen',
            role: 'togglefullscreen'
          }
        ] as MenuItemConstructorOptions[] 
      } as MenuItemConstructorOptions,

      // { role: 'windowMenu' }
      {
        label: Intl.gettext('&Window'),
        id: 'window',
        submenu: [
          {
            label: Intl.gettext('&Minimize'),
            id: 'minimize',
            role: 'minimize'
          },
          {
            label: Intl.gettext('&Zoom'),
            id: 'zoom',
            role: 'zoom'
          }
        ] as MenuItemConstructorOptions[] 
      } as MenuItemConstructorOptions,
      {
        label: Intl.gettext('&Help'),
        id: 'help',
        role: 'help',
        submenu: [
          {
            label: Intl.gettext('&ToggleAbout'),
            id: 'toggle-about',
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
  static attachMenu(window: BrowserWindow,
    startUrlParam: StartUrlParam): void {

    const menu = this.createMenu(startUrlParam)

    window.setMenu(menu)
    this.attachMenuOperation(window, menu)

    // Intl.bindtextdomain('ui-menu')
  }


  /**
   * attach menu operation to window
   */
  static attachMenuOperation(window: BrowserWindow, menu: Menu): void {
    window.webContents.on('did-navigate', (event: Event,
      url: string,
      httpResponseCode: number,
      httpStatusText: string) => {
      const toggleAbout = MenuUtil.findMenuItem(menu, 'help', 'toggle-about')
      if (toggleAbout) {
        toggleAbout.enabled = Config.tokenLoginUrl == url
      }
      const restart = MenuUtil.findMenuItem(menu, 'view', 'restart')
      if (restart) {
        restart.visible = Config.tokenLoginUrl != url
      }
      const reload = MenuUtil.findMenuItem(menu, 'view', 'reload')
      if (reload) {
        reload.visible = Config.tokenLoginUrl != url
      }
      const forceReload = MenuUtil.findMenuItem(menu, 'view', 'force-reload')
      if (forceReload) {
        forceReload.visible = Config.tokenLoginUrl != url
      }
    })
  }
  /**
   * attach menu operation to window
   * this method did not work well on linux environment
   */
  static attachMenuOperation0(window: BrowserWindow, menu: Menu): void {
    const helpItem = MenuUtil.findMenuItem(menu, 'help')
    if (helpItem && helpItem.submenu) {
      helpItem.submenu.on('menu-will-show', (event: Event) => {
        if (helpItem.submenu) {
          const toggleItem = MenuUtil.findMenuItem(helpItem.submenu,
            'toggle-about') 
          if (toggleItem) {
            const url = window.webContents.getURL()
            toggleItem.enabled = 
              window.webContents.getURL() == Config.tokenLoginUrl 
          }
        }
      })
    }
  }
}

// vi: se ts=2 sw=2 et:

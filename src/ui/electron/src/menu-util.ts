import { Menu, MenuItem } from 'electron'

/**
 * helper functions for menu
 */
class MenuUtil {

  /**
   * find menu item from root menu
   */
  static findMenuItem(menu: Menu, ...ids: string[]): MenuItem | undefined {

    return this.findMenuItemFromMenuItems(menu.items, ...ids)
  }

  /**
   * find menu item from root menu
   */
  static findMenuItemFromMenuItems(
    menuItems: MenuItem[], ...ids: string[]): MenuItem | undefined {
    let result
    if (ids.length) {
      const id = ids.shift()
      const item = menuItems.find(item => item.id == id) 
      if (item) {
        if (ids.length) {
          if (item.submenu)  {
            result = this.findMenuItem(item.submenu, ...ids)
          }
        } else {
          result = item
        }   
      }
    }
    return result
  }
}


export {
  MenuUtil,
  MenuUtil as default
}

// vi: se ts=2 sw=2 et:

import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import { resolve, join } from 'path'

// https://vitejs.dev/config/
export default defineConfig(({command, mode, ssrBuild})=> {
  const config = {
    resolve: {
      alias: {
        'oc-soft': resolve(__dirname, 'client/src')
      }
    },
    plugins: [vue() ]
  }
  
  config.root = './client'
  config.base = './'

  config.build = {
    outDir: '../dist',
    rollupOptions: {
      input: {
        main: 'client/index.html'
      }
    }
  }
  return config
})
// vi: se ts=2 sw=2 et:

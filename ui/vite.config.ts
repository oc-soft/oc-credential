import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import { resolve } from 'path'

// https://vitejs.dev/config/
export default defineConfig(({command, mode, ssrBuild})=> {
  const config = {
    plugins: [vue()]
  }
  const build = { 
  }
  
  build.sourcemap = true 
  // config.build = build 
  config.base = './'
  return config
})
// vi: se ts=2 sw=2 et:

import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'

// https://vitejs.dev/config/
export default defineConfig(({command, mode, ssrBuild})=> {
  const config = {
    plugins: [vue()]
  }
  if (command === 'serve') {
    config.build = {
      sourcemap: true
    }
  }
  return config
})
// vi: se ts=2 sw=2 et:

import typescript from '@rollup/plugin-typescript'

const external = [
  'electron'
]
const output = {
  dir: 'dist',
  format: 'cjs'
}
const plugins = [
  typescript({
    tsconfig: './electron/tsconfig.json'
  })
]
export default [
  {
    input: 'electron/src/main.ts',
    external,
    output,
    plugins
  },
  {
    input: 'electron/src/preload.ts',
    external,
    output,
    plugins
  }
]

// vi: se ts=2 sw=2 et:

import { app } from 'electron'
import { Credential } from './credential'
import { Option } from './option'


(async ()=> {

  await app.whenReady()

  const opt = new Option()
  const cred = new Credential()

  const argv = process.argv.slice(1)
  opt.parse(argv)
  process.exitCode = await opt.run(cred)
  
})() 
// vi: se ts=2 sw=2 et:

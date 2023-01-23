import { app } from 'electron'
import { Credential } from './credential'
import { Option } from './option'

(async ()=> {
  await app.whenReady()
  const cred = new Credential()
  const opt = new Option()
  
  console.log("parse start")
  opt.parse(process.argv.slice(1))
  process.exitCode = await opt.run(cred)
  
})() 
// vi: se ts=2 sw=2 et:

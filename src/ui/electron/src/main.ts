import { app } from 'electron'
import { Credential } from './credential'
import { Option } from './option'
import Locale from './locale'
import Intl from './intl'

(async ()=> {

  await app.whenReady()

  Locale.setlocale(Locale.LC_MESSAGES, '')
  console.log(Intl.textdomain(null))
  const opt = new Option()
  const cred = new Credential()

  const argv = process.argv.slice(1)
  opt.parse(argv)

  
  process.exitCode = await opt.run(cred)
  
})() 
// vi: se ts=2 sw=2 et:

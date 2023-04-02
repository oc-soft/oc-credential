/**
 * save oauth token
 */
export async function saveOauthToken(token: string): Promise<boolean> {
  const api = globalThis.ocApi
  if (api && api.saveOauthToken) {
    return await api.saveOauthToken(token)
  } else {
    return false
  }
}

// vi: se ts=2 sw=2 et:

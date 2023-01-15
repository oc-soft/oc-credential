

mergeInto(LibraryManager.library, {
  $app_config_start__deps: ['$ccall'],
  $app_config_start__docs: `
  /**
   * initialize application cofiguration
   * @return {number} you get zero if succeeded
   */
  `,
  $app_config_start: function() {
    return ccall('_app_config_start', 'number', [], [])
  }, 

  $app_config_stop__deps: ['$ccall'],
  $app_config_stop__docs: `
  /**
   * tear down application configuration
   * @return {number} you get zero if succeeded.
   */
  `,
  $app_config_stop: function() {
    return ccall('__app_config_stop', 'number', [], []) 
  },
  $user_resource_get_credential_data_directory__deps: [
    '$ccall',
    '$user_resource_free'],
  $user_resource_get_credential_data_directory__docs: `
  /**
   * get user resource directory.
   * return {string | undefined} you get a user data directory.
   */
  `,
  $user_resource_get_credential_data_directory: function() {
    const data_dir = ccall('_user_resource_get_credential_data_directory',
      'number', [], [])
    let result
    if (data_dir) {
      result = UTF8ToString(data_dir)
    }
    if (data_dir) {
      user_resource_free(data_dir)
    }
    return result
  },

  $user_resource_free__deps: ['$ccall'],
  $user_resource_free__docs: `
  /**
   * free object which you get from this interface
   * @param {number} the object pointer you get
   */
  `,
  $user_resource_free: function(obj) {
    ccall('_user_resource_free', ['number'], [obj]) 
  },
  $user_resource_get_credential_data_path__deps: [
    '$ccall',
    '$user_resource_free'
  ],
  $user_resource_get_credential_data_path__docs: `
  /**
   * get credential data path
   * @return {string | undefined} credential data path
   */
  `,
  $user_resource_get_credential_data_path: function() {
    const data_path = ccall('_user_resource_get_credential_data_path',
      'number', [], [])
    let result
    if (data_path) {
      result = UTF8ToString(data_path)
    }
    if (data_path) {
      user_resource_free(data_path)
    }
    return result
  },
  $credential_get__deps: [
    '$ccall',
    'malloc',
    'free',
    '$getTypeSize'
  ],
  $credential_get__docs: `
  /**
   * git credential get operation
   * @param {string} desc crendential descriptor to query
   * @return {string} credential descriptor for the query
   */
  `,
  $credential_get: function(desc) {
    const descResRef = _malloc(getTypeSize('void*'))
    const descResLenRef = _malloc(getTypeSize('size_t'))
    const descLength = lengthBytesUTF8(desc)
    const descBuf = _malloc(descLength) 
    if (descBuf) {
      stringToUTF8Array(desc, HEAP8, descBuf, descLength)
    }
    let result 
    if (descResRef && descResLenRef && descBuf) {
      const res = ccall('_credential_get', 'number',
        ['number', 'number', 'number', 'number'],
        [descBuf, descLength, descResRef, descResLenRef]
      )
      if (res) {
        const descPtr = getValue(descResRef, `i${getTypeSize('void*') * 8}`) 
        const descLen = getValue(descResLen, `i${getTypeSize('size_t') * 8}`)
        result = UTF8ToString(descPtr, descLen)
        ccall('_credential_free', 'undefined', ['number'], [descPtr])
      }
    }
    if (descBuf) {
      _free(descBuf)
    }
    if (descResRef) {
      _free(descRefRef)
    }
    if (descResLenRef) {
      _free(descResLenRef)
    }
  },
  $credential_base_0__deps: [
    '$ccall',
    'malloc',
    'free',
    '$getTypeSize'
  ],
  $credential_base_0__doc: `
  /**
   * credential 'function_name' operation
   * @param {string} function_name
   * @param {string} descriptor
   * @return {boolean} you get true if succeeded
   */
  `,
  $credential_base_0: function (function_name, desc) {
    const descLength = lengthBytesUTF8(desc)
    const descBuf = _malloc(descLength) 
    if (descBuf) {
      stringToUTF8Array(desc, HEAP8, descBuf, descLength)
    }
    let result = false
    if (descResRef && descResLenRef && descBuf) {
      const res = ccall(function_name, 'number',
        ['number', 'number'],
        [descBuf, descLength]
      )
      result = res == 0
    }
    if (descBuf) {
      _free(descBuf)
    }
    return result
  },
  $credential_store__deps: [
    '$credential_base_0'
  ],
  $credential_store__doc: `
  /**
   * git credential store operation
   * @param {string} descriptor
   * @return {boolean} you get true if succeeded
   */
  `,
  $credential_store: function (function_name, desc) {
    return crendential_base_0('_credential_store', desc) 
  },
  $credential_erase__deps: [
    '$credential_base_0'
  ],
  $credential_erase: `
  /**
   * git credential erase operation
   * @param {string} descriptor
   * @return {boolean} you get true if succeeded
   */
  `,
  $credential_erase: function (function_name, desc) {
    return crendential_base_0('_credential_erase', desc) 
  }
})
// vi: se ts=2 sw=2 et:

<template>
  <Message :message="msg" />
  <select @change="selected">
    <option disabled value="google"></option>
    <option value="google">Google</option>
    <option value="apple">Apple</option>
    <option value="github">Github</option>
  </select>
  <div>
    <button @click="signIn">Sign In</button>
  </div>
</template>
<script lang="ts">
import Message from './Message.vue'
import { saveOauthToken } from 'oc-soft/io'
export default {
  components: {
    Message
  }
}
</script>
<script setup lang="ts">

import { ref } from 'vue'
import { getService, getDescriptor } from 'oc-soft/option'


let service = getService()

if (typeof service == void 0) {
  service = ''
}

const msg = ref(service)

/**
 * handle selected event.
 */
function selected(event: Event) {
  const selected = (event.target as HTMLSelectElement).selectedOptions
  if (selected.length) {
    msg.value = selected[0].innerHTML
  }
}

/**
 * sign in with token
 */
function signIn(event: Event) {
  saveOauthToken('it is a dummy token')  
}

</script>

<!-- /* vi: se ts=2 sw=2 et: */ -->

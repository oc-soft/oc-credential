<template>
  <Message :message="msg" />
  <select @change="selected">
    <option disabled value="google"></option>
    <option value="google">Google</option>
    <option value="apple">Apple</option>
    <option value="github">Github</option>
  </select>
</template>
<script setup lang="ts">
import { ref } from 'vue'

const msg = ref('')
function selected(event: Event) {
  const selected = (event.target as HTMLSelectElement).selectedOptions
  if (selected.length) {
    msg.value = selected[0].innerHTML
  }
}

</script>
<script lang="ts">
import Message from './Message.vue'
export default {
  components: {
    Message
  }
}
</script>

<!-- /* vi: se ts=2 sw=2 et: */ -->

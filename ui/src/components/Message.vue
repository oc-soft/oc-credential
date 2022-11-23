<template>
  <div>{{ message }}</div>
</template>
<script setup lang="ts"> 
  defineProps<{
    message: string
  }>()
</script>

<!-- vi: se ts=2 sw=2 et: -->

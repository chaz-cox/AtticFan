<script setup>
defineProps({
  _status: String,
});
</script>

<template>
  <div class="mb-4">
    <h3 class="text-lg font-semibold text-black">Fan Status: <span class="text-blue-700">{{ _status }}</span></h3>
  </div>
</template>


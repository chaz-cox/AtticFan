<script setup>
defineProps({
  _temp: Number,
  _setTemp: Number,
});
import SvgIcon from '@jamescoyle/vue-icon'
import { mdiTemperatureFahrenheit } from '@mdi/js'
const path = mdiTemperatureFahrenheit;
</script>

<template>
  <div class="mb-4 p-4 border-4 border-black rounded-lg">
  <h2 class="text-2xl font-semibold text-black flex items-center">
      Temperature in Attic: 
      <span class="text-orange-800 text-2xl ml-2">{{ _temp }}</span> 
      <svg-icon type="mdi" :path="path" class=""></svg-icon>
  </h2>
  <br />
  <h2 class="text-2xl font-semibold text-black flex items-center">
      Auto Temperature Set:   
      <span class="text-orange-800 text-2xl ml-2">{{ _setTemp }}</span> 
      <svg-icon type="mdi" :path="path" class=""></svg-icon>
  </h2>
  </div>
</template>


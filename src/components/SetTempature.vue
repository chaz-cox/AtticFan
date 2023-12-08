<script setup>
import { ref, defineProps, defineEmits } from 'vue';
import SvgIcon from '@jamescoyle/vue-icon'
import { mdiTemperatureFahrenheit } from '@mdi/js'

const emit = defineEmits();
const path = mdiTemperatureFahrenheit;

const {_setTemp} = 
defineProps({
  _setTemp: Number,
});

const handleSetButtonClick = (isSet) => {
    var newValue;
    if (isSet){
        newValue = document.getElementById('setTemperature').value;
    }else{
        newValue = _setTemp;
    }
    emit('set',newValue);
};

</script>

<template>
  <div class="mb-4 p-4">
    <div class="flex flex-col"> <!-- Updated to flex flex-col -->
      <div class="mt-4 flex items-center"> <!-- Updated to flex items-center -->
        <label for="setTemperature" class="text-lg font-semibold text-black">Temperature: </label>
        <div class="flex items-center"> <!-- New flex container -->
          <input
            type="number"
            id="setTemperature"
            name="setTemperature"
            class="w-20 px-2 py-1 bg-gray-800 text-white rounded-md focus:outline-none focus:ring focus:border-blue-500"
            :value="_setTemp"
          />
          <label  class="text-lg font-semibold text-black"><svg-icon type="mdi" :path="path"></svg-icon> </label>
        </div>
      </div>
      <button @click="handleSetButtonClick(true)" 
      class="text-sm font-semibold text-white bg-blue-500 hover:bg-blue-700 px-2 py-1 border border-blue-700 rounded mt-2 w-16">Set</button> 
      <button @click="handleSetButtonClick(false)" 
      class="text-sm font-semibold text-white bg-blue-500 hover:bg-blue-700 px-2 py-1 border border-blue-700 rounded mt-2 w-16">Cancel</button> 
      </div>
  </div>
</template>


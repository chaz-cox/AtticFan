<script setup>
import { ref, defineProps, defineEmits } from 'vue';

const  emit  = defineEmits();

const {_title , _content , _log} = 
defineProps({
    _title:String,
    _content:String,
    _log:Boolean,
});

const startDate = ref('');
const endDate = ref('');

const close = () => {
    startDate.value = null;
    endDate.value = null;
    submit();
}

const submit = () => {
  // Validate and process the input data as needed
  emit('submit', { startDate: startDate.value, endDate: endDate.value });
};
</script>

<template>
  <div  class="fixed inset-0 bg-black bg-opacity-50 flex items-center justify-center">
    <div class="bg-white p-8 rounded-md w-96">
      <h2 class="text-2xl font-semibold mb-4">{{ _title }}</h2>
      <p class="mb-4">{{ _content }}</p>

      <!-- Date Range Input (if Log is true) -->
      <div v-if="_log" class="mb-4">
        <label for="startDate" class="block text-sm font-medium text-gray-700">Start Date:</label>
        <input type="date" v-model="startDate" id="startDate" name="startDate" class="mt-1 p-2 border rounded-md w-full" />

        <label for="endDate" class="block text-sm font-medium text-gray-700 mt-4">End Date:</label>
        <input type="date" v-model="endDate" id="endDate" name="endDate" class="mt-1 p-2 border rounded-md w-full" />
      </div>

      <div class="flex justify-end">
        <button @click="close" class="mr-2 px-4 py-2 bg-gray-500 text-white rounded-md">Close</button>
        <button @click="submit" v-if="_log" class="px-4 py-2 bg-blue-500 text-white rounded-md">Submit</button>
      </div>
    </div>
  </div>
</template>



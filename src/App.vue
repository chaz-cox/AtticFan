<script setup>
import { ref } from 'vue';
import Status from './components/Status.vue';
import Tempature from './components/Tempature.vue';
import SetTempature from './components/SetTempature.vue';
import LogDialog from './components/Dialog.vue';
import { IntersectingCirclesSpinner } from 'epic-spinners'

const SERVICE_UUID = 'a1c3103b-efff-49cd-9ed5-afe405dbf51d';
const CHARACTERISTIC_UUID = 'dba31013-52aa-45e5-a22d-329f6d02aba0';
var myCharacteristic = null;
var dec = new TextDecoder();
var enc = new TextEncoder();

const showSetTemperature = ref(false);
var setTemp = null;
const atticTemp = ref(null);

var loading = false;
var auto = false;

const status = ref("None");

const viewLog = ref(false);
const viewLogNoBLE = ref(false);
const connected= ref(false);
const log = ref(false);

const toggleViewLog = (isLog) =>{
    if (checkBLE()){
        log.value = isLog;
        if(isLog){
            viewLog.value = !viewLog.value;
        }
    }
};

const toggleIsConnected = () =>{
    bluetooth.value = !bluetooth.value;
}

const toggleSetTemperature = () => {
  showSetTemperature.value = !showSetTemperature.value;
};

const setTemperature = (value) =>{
    if (checkBLE()){
        setTemp= value;
        if (auto){
            handleAuto();
        }
    }
    toggleSetTemperature();
}

const submitDialog = (dateRange) =>{
    if ( dateRange.startDate != null || dateRange.endDate != null){
        console.log("Start Date:",dateRange.startDate);
        console.log("End Date:",dateRange.endDate);
    }
    if (connected){
        toggleViewLog(true);
    }else{
        //if you hit connect -> connect else nothing
        connected.value = true;
    }
    viewLogNoBLE.value = false;
}

const handleManualON = () =>{
    let message = "fan_on";
    if(checkBLE()){
        myCharacteristic.writeValue(enc.encode(message));
    }
    auto = false;
    load();
}

const handleManualOFF = () =>{
    let message = "fan_off";
    if(checkBLE()){
        myCharacteristic.writeValue(enc.encode(message));
    }
    auto = false;
    load();
}

const handleAuto = () =>{
    if (checkBLE()){
        if(setTemp == null){
            alert("max Auto is null");
        }else{
            let lenOfTemp = String(setTemp).length;
            let message = String(lenOfTemp)+"_auto_"+String(setTemp);
            myCharacteristic.writeValue(enc.encode(message));
            auto = true;
            load();
        }
    }
}

const checkBLE = () =>{
    if (!connected.value){
       viewLogNoBLE.value = true;
       return false;
    }
    return true;
}

const load = () =>{
    loading = true;
     setTimeout(() => {
        loading= false;
      }, 1000);
};

const handleConnect = () =>{
    loading = true;
    navigator.bluetooth.requestDevice({
    filters: [{ services: [SERVICE_UUID] }]
  }).then(device => {
    return device.gatt.connect();
  }).then(server => {
    return server.getPrimaryService(SERVICE_UUID);
  }).then(service => {
    return service.getCharacteristic(CHARACTERISTIC_UUID);
  }).then(characteristic => {
    myCharacteristic = characteristic;
    readValuePeriodically();
    connected.value = true;
  }).catch((error) => {
      console.error('Connection error:', error);
      connected.value = false; // Set connected to false on error or cancel
  }).finally(() =>{
      loading = false;
  });
}

const readValuePeriodically = async () => {
    let isReading = false;
    try {
      while (true) {
          if(!isReading){
            const value = await myCharacteristic.readValue();
            const decodedValue = dec.decode(value);
            let lenOfTemps = decodedValue[0] - '0';
            lenOfTemps +=1;
            let statusValue = decodedValue.slice(1,decodedValue.length-lenOfTemps);
            let temps = decodedValue.slice(decodedValue.length-lenOfTemps);
            let Split = temps.indexOf(',');
            let atemp = temps.slice(0,Split);
            let maxtemp = temps.slice(Split+1);
            if (statusValue == '_auto_on_'){
                status.value = "ON - AUTO";
                auto = true;
            }else if (statusValue == '_auto_off_'){
                status.value = "OFF - AUTO";
                auto = true;
            }else if( statusValue == '_manual_on_'){
                status.value= "ON - MANUAL"
                auto = false;
            }else if( statusValue == '_manual_off_'){
                status.value= "OFF - MANUAL"
                auto = false;
            }else{
                console.error("ERR:",decodedValue);
                auto = false;
            }
            atticTemp.value = atemp;
            setTemp = maxtemp;
            isReading = false;
          }
        await new Promise((resolve) => setTimeout(resolve, 1500)); // Adjust the interval as needed
      }
    } catch (error) {
      console.error('Error reading value:', error);
      console.error('Error details:', error.message, error.code, error.name);
      isReading = false;
    }
};




</script>

<template>
  <div class="flex flex-col items-center justify-center h-screen bg-gradient-to-b from-blue-200 via-red-400 to-blue-300">
    <h1 class="mb-4 text-4xl font-extrabold leading-none tracking-tight text-black border-b-4 border-white p-4 md:text-5xl lg:text-6xl">
      Attic Fan
    </h1>
    <Status :_status="status" />
    <Tempature :_temp="atticTemp" :_setTemp="setTemp" />
    <button @click="toggleSetTemperature" v-if="!showSetTemperature" class="small-button">Set Max Temperature</button>
    <SetTempature v-if="showSetTemperature" :_setTemp="setTemp" @set="setTemperature" />
    <LogDialog v-if="viewLog" _title="Attic Fan Log" _content="Select a date range to see if the fan was on that day, and how long it was on." :_log=viewLog @submit="submitDialog" /> 
    <LogDialog v-if="viewLogNoBLE" _title="Connect To Bluetooth" _content="Fan data not avalible unless you connect to it via bluetooth!" :_log=false @submit="submitDialog" />

    <intersecting-circles-spinner
    v-if="loading"
  :animation-duration="1200"
  :size="70"
  color="#ff1d5e"
/>
    <div class="space-y-4 text-center">
      <button class="button" @click="handleManualON">Manual ON</button>
      <button class="button" @click="handleManualOFF">Manual OFF</button>
      <button class="button" @click="handleAuto">Auto</button>
      <button class="button" @click="toggleViewLog(true)">View Log</button>
      <br />
      <button v-if="!connected" class="small-button" @click="handleConnect">Connect</button>
    </div>
  </div>
</template>

<style scoped>
.button {
  @apply px-8 bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4 border border-blue-700 rounded;
}
.small-button {
  @apply px-4 bg-blue-500 hover:bg-blue-700 text-white font-bold py-1 px-2 border border-blue-700 rounded;
}
</style>


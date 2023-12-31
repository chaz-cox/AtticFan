 #include <BLEDevice.h>
 #include "heltec.h"
 #include "DHT.h"
 
 #define DHTPIN 14
 #define DHTTYPE DHT11
 #define FANPIN 13

  #define DEVICE_NAME         "Attic Fan"
  #define SERVICE_UUID        "a1c3103b-efff-49cd-9ed5-afe405dbf51d"
  #define CHARACTERISTIC_UUID "dba31013-52aa-45e5-a22d-329f6d02aba0"

  BLECharacteristic *pCharacteristic;
  String message = "";
  String status = "";
  float maxTemp=0;
  bool Auto = false;
  DHT dht(DHTPIN,DHTTYPE, 6);

  void printToScreen(String s) {
    Heltec.display->clear();
    Heltec.display->drawString(0, 0, s);
    Heltec.display->display();
  }

  class MyServerCallbacks: public BLEServerCallbacks {
      void onConnect(BLEServer* pServer) {
        printToScreen("BLE client connected.");
      };

      void onDisconnect(BLEServer* pServer) {
        printToScreen("BLE client disconnected.");
        BLEDevice::startAdvertising();
      }
  };

  class MyCharacteristicCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *characteristic) {
      message = String(characteristic->getValue().c_str());
      printToScreen(message);

      if (message == "fan_on") {
        digitalWrite(FANPIN, LOW);
        status = "_manual_on_";
        Auto = false;
      }
      if (message == "fan_off") {
        digitalWrite(FANPIN, HIGH);
        status = "_manual_off_";
        Auto = false;
      }
      if (isDigit(message[0])){
        int lenOfTemp = message[0] - '0';
        maxTemp = (message.substring(message.length()-lenOfTemp)).toFloat();
        Serial.println("lenofTemp:"+String(lenOfTemp)+" maxTemp:"+ String(maxTemp) + " message: "+message);
        Auto = true; 
      } 
    }  
  };

  void setup() {
    Heltec.begin(true /*display*/, false /*LoRa*/, true /*Serial*/);
    printToScreen("Heltec ON");
    digitalWrite(FANPIN, HIGH);

    pinMode(FANPIN,OUTPUT);
    Serial.begin(9600);
    dht.begin();

    BLEDevice::init(DEVICE_NAME);

    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *pService = pServer->createService(SERVICE_UUID);

    pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
    );
    pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());
    pCharacteristic->setValue("Init");

    pService->start();

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    BLEDevice::startAdvertising();
  }

  void loop() {
    delay(10000);
      float h = dht.readHumidity();
      float f = dht.readTemperature(true);
      if(isnan(h) || isnan(f)){
        printToScreen("Couldn't get tempature.");
        return;
      }
      float index = dht.computeHeatIndex(f,h);

      if(Auto){
        if (f > maxTemp){

          Serial.print("Temp:"+String(f)+" MaxTemp:"+String(maxTemp)+'\n');
          if (status != "_auto_on_"){ //if it is already on no need to turn it on again.
           digitalWrite(FANPIN, LOW);
           status = "_auto_on_";
          }
        }else{
          if (status != "_auto_off_"){
            digitalWrite(FANPIN, HIGH);
            status = "_auto_off_";
          }
        }
      }

      String letter = String(String(String(f).length() + String(int(maxTemp)).length())) + status + String(f)+ ","+String(int(maxTemp));
      
      pCharacteristic->setValue(letter.c_str());
      printToScreen("Humidity: " + String(h)+ "\n"+"Tempature: " + String(f)+ "*F\n"+"Feels Like: " + String(index)+ "*F\n"); 
  }

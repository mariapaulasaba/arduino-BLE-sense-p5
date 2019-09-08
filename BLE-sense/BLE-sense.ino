/*
 * This example shows how to read sensor data from the Nano BLE Sense 
 * and to store in a service that can be read by central devices.
 * 
 * WIP, Last Updated 9/7/2018
 * Created by Maria Paula Saba and Maxwell Dayvson
 */



#include <ArduinoBLE.h>
#include <Arduino_APDS9960.h>
#include <Arduino_HTS221.h>


BLEService nanoService("19b10010-e8f2-537e-4f6c-d104768a1214"); // BLE Nano Service
BLEByteCharacteristic proxCharacteristic("19b10011-e8f2-537e-4f6c-d104768a1214", BLERead | BLENotify);
BLEByteCharacteristic tempCharacteristic("19b10012-e8f2-537e-4f6c-d104768a1214", BLERead | BLENotify);
BLEByteCharacteristic redCharacteristic("19b10013-e8f2-537e-4f6c-d104768a1214", BLERead | BLENotify);
BLEByteCharacteristic greenCharacteristic("19b10014-e8f2-537e-4f6c-d104768a1214", BLERead | BLENotify);
BLEByteCharacteristic blueCharacteristic("19b10015-e8f2-537e-4f6c-d104768a1214", BLERead | BLENotify);

void setup() {
  //starts Serial
   Serial.begin(9600);
  while (!Serial);

  //check if everything is working
  if (!APDS.begin()) {
    Serial.println("Error initializing APDS9960 sensor!");
  }
  
  if (!HTS.begin()) {
    Serial.println("Failed to initialize pressure sensor!");
  }
  
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }
  

  //BLE setup
  BLE.setLocalName("Arduino NANO BLE");
  BLE.setAdvertisedService(nanoService);
  nanoService.addCharacteristic(proxCharacteristic);
  nanoService.addCharacteristic(tempCharacteristic);
  nanoService.addCharacteristic(redCharacteristic);
  nanoService.addCharacteristic(greenCharacteristic);
  nanoService.addCharacteristic(blueCharacteristic);
  BLE.addService(nanoService);

  
  BLE.advertise();
  Serial.println("BLE LED Peripheral");
}

void loop() {
 
  
  //central
  BLEDevice central = BLE.central();
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
//
    while (central.connected()) {
     
     //Proximity Sensor   
        if (APDS.proximityAvailable()) {
          int proxValue = APDS.readProximity();
          proxCharacteristic.writeValue(proxValue);
       }


      //Color Sensor
        if (APDS.colorAvailable()) {
          int r, g, b;
          APDS.readColor(r, g, b);
          r = map(r, 0, 4097, 0, 255);
          g = map(g, 0, 4097, 0, 255);
          b = map(b, 0, 4097, 0, 255);
          redCharacteristic.writeValue(r);
          greenCharacteristic.writeValue(g);
          blueCharacteristic.writeValue(b);
        }



      //Temperature sensor
       float tempValue = HTS.readTemperature(FAHRENHEIT);
       tempCharacteristic.writeValue(tempValue);
    
    delay(1000);

    }
  }
}

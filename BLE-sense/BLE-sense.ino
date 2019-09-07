/*
 * This example shows how to read sensor data from the Nano BLE Sense 
 * and to store in a service that can be read by central devices.
 * 
 * WIP, Last Updated 9/7/2018
 * Created by Maria Paula Saba and Maxwell Dayvison
 */



#include <ArduinoBLE.h>
#include <Arduino_APDS9960.h>
#include <Arduino_HTS221.h>


BLEService nanoService("19b10010-e8f2-537e-4f6c-d104768a1214"); // BLE Nano Service
BLEByteCharacteristic proximityCharacteristic("19b10011-e8f2-537e-4f6c-d104768a1214", BLERead | BLENotify);
BLEByteCharacteristic temperatureCharacteristic("19b10012-e8f2-537e-4f6c-d104768a1214", BLERead | BLENotify);
int proximityValue; 
float temperatureValue;

void setup() {
  //starts Serial
   Serial.begin(9600);
//  while (!Serial);

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
  
  proximityValue = 50.0;

  //BLE setup
  BLE.setLocalName("Arduino NANO BLE");
  BLE.setAdvertisedService(nanoService);
  nanoService.addCharacteristic(proximityCharacteristic);
  nanoService.addCharacteristic(temperatureCharacteristic);
  BLE.addService(nanoService);
  proximityCharacteristic.writeValue(proximityValue);
  temperatureCharacteristic.writeValue(temperatureValue);
  
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
        if (APDS.proximityAvailable()) {
          proximityValue = APDS.readProximity();
          proximityCharacteristic.writeValue(proximityValue);
       }
 
       float temperature = HTS.readTemperature(FAHRENHEIT);
       temperatureCharacteristic.writeValue(temperature);
       delay(300);
    }
  }
}

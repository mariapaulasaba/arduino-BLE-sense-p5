
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
#include <Arduino_LPS22HB.h>
#include <Arduino_LSM9DS1.h>


BLEService nanoService("19b10010-e8f2-537e-4f6c-d104768a1214"); // BLE Nano Service
BLEByteCharacteristic proxCharacteristic("19b10011-e8f2-537e-4f6c-d104768a1214", BLERead | BLENotify);
BLEByteCharacteristic tempCharacteristic("19b10012-e8f2-537e-4f6c-d104768a1214", BLERead | BLENotify);
BLEByteCharacteristic redCharacteristic("19b10013-e8f2-537e-4f6c-d104768a1214", BLERead | BLENotify);
BLEByteCharacteristic greenCharacteristic("19b10014-e8f2-537e-4f6c-d104768a1214", BLERead | BLENotify);
BLEByteCharacteristic blueCharacteristic("19b10015-e8f2-537e-4f6c-d104768a1214", BLERead | BLENotify);
BLEByteCharacteristic humidCharacteristic("19b10016-e8f2-537e-4f6c-d104768a1214", BLERead | BLENotify);
BLEByteCharacteristic pressCharacteristic("19b10017-e8f2-537e-4f6c-d104768a1214", BLERead | BLENotify);
BLEFloatCharacteristic accelXCharacteristic("19b10018-e8f2-537e-4f6c-d104768a1214", BLERead | BLENotify);
BLEFloatCharacteristic accelYCharacteristic("19b10019-e8f2-537e-4f6c-d104768a1214", BLERead | BLENotify);
BLEFloatCharacteristic accelZCharacteristic("19b10020-e8f2-537e-4f6c-d104768a1214", BLERead | BLENotify);
//BLEByteCharacteristic gyroCharacteristic("19b10019-e8f2-537e-4f6c-d104768a1214", BLERead | BLENotify);
//BLEByteCharacteristic magneCharacteristic("19b10020-e8f2-537e-4f6c-d104768a1214", BLERead | BLENotify);
//BLEByteCharacteristic gestureCharacteristic("19b10021-e8f2-537e-4f6c-d104768a1214", BLERead | BLENotify);

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
  
 if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
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
  nanoService.addCharacteristic(humidCharacteristic);
  nanoService.addCharacteristic(pressCharacteristic);
  nanoService.addCharacteristic(accelXCharacteristic);
  nanoService.addCharacteristic(accelYCharacteristic);
  nanoService.addCharacteristic(accelZCharacteristic);

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
          Serial.println("proximity");
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


       //Gesture
//       if (APDS.gestureAvailable()) {
//          // a gesture was detected
//          int gestureValue = APDS.readGesture();
//          Serial.println(gestureValue);
//          gestureCharacteristic.writeValue(gestureValue);
//        }


      //Temperature sensor
       float tempValue = HTS.readTemperature(FAHRENHEIT);
       tempCharacteristic.writeValue(tempValue);
       
     //Humidity  
       float humidityValue = HTS.readHumidity();
       humidCharacteristic.writeValue(humidityValue);

      //Pressure  
      float pressureValue = BARO.readPressure(PSI);
      pressCharacteristic.writeValue(pressureValue);

       //Accelerometer
       

        if (IMU.accelerationAvailable()) {
          float x, y, z;
          IMU.readAcceleration(x, y, z);
          accelXCharacteristic.writeValue(x);
          accelYCharacteristic.writeValue(y);
          accelZCharacteristic.writeValue(z);
          Serial.println("Accelerometer");
          Serial.print(x);
          Serial.print('\t');
          Serial.print(y);
          Serial.print('\t');
          Serial.println(z);
        }
      
      //Gyroscope
        if (IMU.gyroscopeAvailable()) {
           float x, y, z;
          IMU.readGyroscope(x, y, z);
//      
//          Serial.println("Gyroscope");
//          Serial.print(x);
//          Serial.print('\t');
//          Serial.print(y);
//          Serial.print('\t');
//          Serial.println(z);
        }

      
      //Magnetometer
      if (IMU.magneticFieldAvailable()) {
            float x, y, z;
            IMU.readMagneticField(x, y, z);
//             Serial.println("Magnetometer");
//     
//            Serial.print(x);
//            Serial.print('\t');
//            Serial.print(y);
//            Serial.print('\t');
//            Serial.println(z);
          }
    

    
    delay(300);

    }
  }
}

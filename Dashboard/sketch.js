/*
*
* This example works as a BLE central that connects to a Nano BLE Sense and 
* displays data read from it.
* 
* WIP, Last Updated 9/7/2018
* Created by Maria Paula Saba and Maxwell Dayvson
*/


//UUID set on the nanoService
const serviceUuid = "19b10010-e8f2-537e-4f6c-d104768a1214";


let isConnected = false;
let myBLE;
let senseValues = [];

let proximity;
let temperature;
let redColor;
let greenColor;
let blueColor;

let humidity;
let pressure;
let accelX;
let accelY;
let accelZ;
// let gyroscope;
// let rssi;

function setup() {

  myBLE = new p5ble();

  createCanvas(windowWidth, windowHeight);
  background(0);
 
  const connectButton = select('#connect');
  connectButton.mousePressed(connectToBle);
  const disconnectButton = select('#disconnect');
  disconnectButton.mousePressed(disconnectToBle);
}

function connectToBle() {
        myBLE.connect(serviceUuid, receivedCharacteristics);    
}

function disconnectToBle() {
  myBLE.disconnect();
  isConnected = myBLE.isConnected();
}

function onDisconnected() {
  console.log('Device got disconnected.');
  isConnected = false;
}


function receivedCharacteristics(error, characteristics) {
  if (error) console.log('error: ', error);
 
  isConnected = myBLE.isConnected();
    
  console.log(characteristics[1]);
  
  for (let i = 0; i < characteristics.length; i++) {
      myBLE.startNotifications(characteristics[i], function(value){
        if(i == 7){
            handleSensorData(value, i, 'float32');
        }
        else handleSensorData(value, i);
          
      });
    }    
}

function handleSensorData(value, index) {
    senseValues[index] = value;
    console.log(senseValues);
    proximity = senseValues[0];
    temperature = senseValues[1];
    redColor  = senseValues[2];
    greenColor  = senseValues[3];   
    blueColor  = senseValues[4];  
    humidity = senseValues[5];
    pressure = senseValues[6];
    accelX = senseValues[7];
    accelY = senseValues[8];
    accelZ = senseValues[9];
}

function draw() {
   background(50);

   if(isConnected){    
       
       
       textSize(16);
       fill(255);
       text('Color Sensor', 20, 40);
       text('Proximity: '+proximity, 180, 40);
       text('Pressure: '+pressure, 180, 80);
       text('Humidity: '+humidity+'%', 420, 40);
       text('Temperature: '+temperature+'F', 420, 80);
       text('Accelerometer: '+accelX+", "+accelY+", "+accelZ, 640, 40);
//       switch(gesture){
//               
//         case '0':
//         text('Gesture Detected: UP', 640, 80); 
//         break;
//            
//         case '1':
//         text('Gesture Detected: DOWN', 640, 80); 
//         break;
//            
//         case '2':
//         text('Gesture Detected: LEFT', 640, 80); 
//         break;
//               
//         case '3':
//         text('Gesture Detected: RIGHT', 640, 80); 
//         break;
//                                    
//          default:
//          //text('No Gesture Detected', 640, 80); 
//          break;     
//       }

       fill(redColor, greenColor, blueColor);
       rect(20,50, 120, 120);
       
       
       
   } 
    
    
    
    
   else{    
       textSize(16);
       fill(255);
       text('Waiting to connect...', 20, 40);
   }
    
    
    

    

}
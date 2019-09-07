const serviceUuid = "19b10010-e8f2-537e-4f6c-d104768a1214";


let isConnected = false;
let myBLE;


let senseValues = [];

// let accelerometer;
// let gyroscope;
 let proximity;

 let temperature;
// let pressure;
// let humidity;

// let light;
// let color;


function setup() {

  myBLE = new p5ble();

  createCanvas(windowWidth, windowHeight);
  background(0);
 
  // Select 'Connect' button
  const connectButton = select('#connect');
  connectButton.mousePressed(connectToBle);
  
}

function connectToBle() {
        // Connect to a device by passing the service UUID
        myBLE.connect(serviceUuid, receivedCharacteristics);
}


function receivedCharacteristics(error, characteristics) {
  if (error) console.log('error: ', error);
  console.log(characteristics[1]);
  
  for (let i = 0; i < characteristics.length; i++) {
      myBLE.startNotifications(characteristics[i], function(value){
        handleSensorData(value, i);
      });
    }    
}

function handleSensorData(value, index) {
        senseValues[index] = value;
}

function draw() {
     background(0);


     proximity = senseValues[0];
     temperature = senseValues[1];
    console.log('proximity: ', proximity, "temperature", temperature);
    

}
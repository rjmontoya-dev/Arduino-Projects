
#include <AltSoftSerial.h>
#include <SoftwareSerial.h>
// Sensor pins
#define sensorPower 7
#define sensorPin A0

// Value for storing water level
int val = 0;
AltSoftSerial mod;
SoftwareSerial espSerial(4, 5);
unsigned long level;
void setup() {
   Serial.begin(115200);
  espSerial.begin(115200);
  mod.begin(9600);
  // Set D7 as an OUTPUT
  pinMode(sensorPower, OUTPUT);
  
  // Set to LOW so no power flows through the sensor
  digitalWrite(sensorPower, LOW);
   
}

void loop() {
  //get the reading from the function below and print it
  level = readSensor();
   int samp = 10.00; 
   float actual_val = map(level,186,0,0,100);
  Serial.println(String(level)+","+ String(samp));
  
  delay(2000);
}

//This is a function used to get the reading
int readSensor() {
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  delay(10);              // wait 10 milliseconds
  val = analogRead(sensorPin);    // Read the analog value form sensor
  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
  return val;             // send current reading
}

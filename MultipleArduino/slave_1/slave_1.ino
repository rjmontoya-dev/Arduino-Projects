#include <Wire.h>

void setup() {
  Wire.begin(8);       // Initialize I2C library as slave with address 8
  Wire.onRequest(requestEvent); // Register the requestEvent function
  Serial.begin(9600);  // Start serial communication for debugging (optional)
}

void loop() {
  // Do whatever you need to do in the loop
}

void requestEvent() {
  float one = 1.00;
  float two = 1.11;
  String data = String(one)+","+String(two);
//  Serial.println(data);
  Wire.write(data.c_str()); // Send data to the master
   
}

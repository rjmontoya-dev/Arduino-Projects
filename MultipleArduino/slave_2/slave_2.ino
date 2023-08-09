#include <Wire.h>

void setup() {
  Wire.begin(9);       // Initialize I2C library as slave with address 9
  Wire.onRequest(requestEvent); // Register the requestEvent function
  Serial.begin(9600);  // Start serial communication for debugging (optional)
}

void loop() {
  // Do whatever you need to do in the loop
}

void requestEvent() {
  float one = 2.00;
  float two = 2.22;
  String data = String(one)+","+String(two);
//  Serial.println(data);
  Wire.write(data.c_str()); // Send data to the master
}

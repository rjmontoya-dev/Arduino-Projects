//Author: Roger Montoya Jr.
//Connections
//Connect all A4 and A5 pins with each other.


#include <Wire.h>

const int BUFFER_SIZE = 10; // Define a larger buffer size to hold the data packet

void setup() {
  Wire.begin();       // Initialize I2C library as master
  Serial.begin(9600); // Start serial communication for debugging (optional)
}

void loop() {
//  handleSlaveData(8, "Slave Arduino 1: "); // Handle requests from Slave Arduino 1 (address 8)
//  handleSlaveData(9, "Slave Arduino 2: "); // Handle requests from Slave Arduino 2 (address 9)
//  handleSlaveData(10, "Slave Arduino 3: "); // Handle requests from Slave Arduino 3 (address 10)
  Serial.println(handleSlaveData(8, "Slave Arduino 1: "));
  Serial.println(handleSlaveData(9, "Slave Arduino 1: "));
  Serial.println(handleSlaveData(10, "Slave Arduino 1: "));
  // You can add more slave Arduinos by calling handleSlaveData() with the appropriate address
  // handleSlaveData(slaveAddress, "Slave Arduino x: ");
  
  delay(1000);  // Wait for 1 second before handling the next data request
}

String handleSlaveData(int slaveAddress, const char* prefix) {
  Wire.requestFrom(slaveAddress, BUFFER_SIZE); // Request data from the slave with the given address

  char receivedData[BUFFER_SIZE];
  int dataIndex = 0;
  bool dataStarted = false;

  while (Wire.available()) {
    char data = Wire.read(); // Read the data received from the slave

    // Check if data packet has started (ignore any initial non-numeric characters)
    if (isdigit(data) || data == '-' || data == '.') {
      dataStarted = true;
    }

    if (dataStarted) {
      receivedData[dataIndex++] = data; // Store the received character in the buffer

      // Check if the data packet is complete (contains a delimiter ";")
      if (data == ';' || dataIndex >= sizeof(receivedData) - 1) {
        receivedData[dataIndex] = '\0'; // Null-terminate the string
        break;
      }
    }
  }

  // Print the received data packet with the provided prefix
//  Serial.print(prefix);
//  Serial.println(receivedData);
return receivedData;
}

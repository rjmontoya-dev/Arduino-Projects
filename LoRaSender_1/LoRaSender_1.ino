//PIN CONNECTIONS
//3.3v - 3.3v
//Gnd - Gnd
//En/Nss - D10
//G0/DIO0 - D2
//SCK - D13
//MISO  - D12
//MOSI - D11
//RST - D9

#include <SPI.h>
#include <LoRa.h>
  
int randNumber = 0 ;
void setup() {
  
  Serial.begin(9600);
 
//LORA----------------------------
  while (!Serial);
  Serial.println("LoRa Sender");
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
 //LORA----------------------------
}

void loop() {
  randNumber = random(10,20);
  Serial.print("Sending packet: ");
  Serial.println(randNumber);
  
  // send packet
  LoRa.beginPacket();  
  LoRa.print(randNumber);   
  LoRa.endPacket(); 
  delay(4000);
}

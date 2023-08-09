//PIN CONNECTIONS OF LORA
//3.3v - 3.3v
//Gnd - Gnd
//En/Nss - D10
//G0/DIO0 - D2
//SCK - D13
//MISO  - D12
//MOSI - D11
//RST - D9

//Tipping bucket wire
//black - G
//white - 3


#include <SPI.h>
#include <LoRa.h>
  
const byte interruptPin = 3;
const int interval = 500;
volatile unsigned long tiptime = millis();

float rainrate= 0.00;
int counter = 0;
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

//TIPPING BUCKET
 // Set up our digital pin as an interrupt
  pinMode(interruptPin, INPUT_PULLUP);
 
}

void loop() {
  attachInterrupt(digitalPinToInterrupt(interruptPin), count, RISING);
  Serial.println(rainrate);
  LoRa.beginPacket();  
  LoRa.print(rainrate);   
  LoRa.endPacket(); 
  delay(5000);
}
void count() {
  // Grab the current ms count for common calculations
  unsigned long curtime = millis();
  
  // Make sure we don't record bounces
  if ((curtime - tiptime) < interval) {
    return;
  }

  // How long since the last tip?
  unsigned long tipcount = curtime - tiptime;
  tiptime = curtime;
  
  // Calculate mm/hr from period between cup tips
    rainrate = 914400.0 / tipcount;
  
  Serial.print("Cup tip: ");
  Serial.print(tipcount);
  Serial.println("ms");  

  Serial.print("Sec: ");
  Serial.print(tipcount/1000);
  Serial.println("sec"); 
   
  
  Serial.print("Rain rate: ");
  Serial.print(rainrate);
  Serial.println("mm/hr");  

//  Serial.println(digitalRead(interruptPin));

  Serial.print("Sending packet: ");
  Serial.println(rainrate);
  
  // send packet
  LoRa.beginPacket();  
  LoRa.print(rainrate);   
  LoRa.endPacket(); 
  rainrate = 0.00;
}

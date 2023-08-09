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

//DHT
#include "DHT.h"
#define DHTPIN 3     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321 
DHT dht(DHTPIN, DHTTYPE);
// LCD
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


int counter = 0;

void setup() {
  
  Serial.begin(9600);

//  DHT AND LCD ------------------
  lcd.init();                      // initialize the lcd 
  lcd.init(); 
  lcd.backlight(); 
  dht.begin();
//  DHT AND LCD ------------------

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

  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)  ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  } 
  lcd.setCursor(0,0);
  lcd.print("Sending packets:");
  lcd.setCursor(0,1);
  lcd.print("H:");x1x
  lcd.print(h);
  lcd.print(" T:");
  lcd.print(t);
  Serial.print("Sending packet: h:");
  Serial.print(h);
  Serial.print(" t:");
  Serial.println(t);
  // send packet
  LoRa.beginPacket();  
  LoRa.print("...");
  LoRa.print(h); 
  LoRa.print(",");
  LoRa.print(t);   
  LoRa.print(",");
  LoRa.endPacket();  

  delay(3000);
}

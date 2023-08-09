
//Light Intensity
#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;
//SCL - A5
//SDA - A4
//vcc - 5v
//Light Intensity

// LCD
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
 // LCD

#include <SPI.h>
#include <LoRa.h>

String receivedData = "0.00";


void setup() {
  Serial.begin(115200);
  
//Light intensity
  Wire.begin(); 
  lightMeter.begin();
  Serial.println(F("BH1750 Test begin"));
//Light intensity

//   LCD ------------------
  lcd.init();                      // initialize the lcd 
  lcd.init(); 
  lcd.backlight();  
//   LCD ------------------


  while (!Serial);

  Serial.println("LoRa Receiver");

  if(!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
     lcd.setCursor(0,0);
    lcd.print("LoRa Failed!...");
    while (1);
  }else{ 
    Serial.println("Initializing LoRa Receiver...");
    lcd.setCursor(0,0);
    lcd.print("Initialize LoRa..."); 
  }
}
void loop() {
  float lux = lightMeter.readLightLevel();
//  Serial.print(" Light: ");
//  Serial.print(lux);
//  Serial.println(" lx");

  Serial.print(String(lux)+","+String(receivedData));//for testing only
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
//    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) 
    {
      receivedData = LoRa.readString();
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("RainGauge");
    lcd.setCursor(0,1);
    lcd.print(receivedData+"mm"); 
    // print RSSI of packet
//    Serial.print(receivedData + "' with RSSI ");
//    Serial.println(LoRa.packetRssi());

    lcd.setCursor(10,0);
    lcd.print("Light");
    lcd.setCursor(9,1);
    lcd.print(lux);
    lcd.print("lx"); 
//    Serial.print(String(lux)+","+String(receivedData));
  }
  delay(1000);
}

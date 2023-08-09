#include <String.h>
#include <SoftwareSerial.h>
SoftwareSerial espSerial(0,1);
//Lora
#include <SPI.h>
#include <LoRa.h>
// LCD
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
 
String humid,temp,s1;
String inString = "";     
int counter = 0;
void setup() {
  Serial.begin(115200);
//  espSerial.begin(115200);
// //   LCD ------------------
//  lcd.init();                      // initialize the lcd 
//  lcd.init(); 
//  lcd.backlight();  
////   LCD ------------------
//
//  while (!Serial); 
//  
//  if (!LoRa.begin(433E6)) {
//    Serial.println("Starting LoRa failed!");
//    lcd.setCursor(0,0);
//    lcd.print("LoRa Failed!...");
//    while (1);
//  }
//  else{ 
//    Serial.println("Initializing LoRa Receiver...");
//    lcd.setCursor(0,0);
//    lcd.print("Initialize LoRa..."); 
//  }
} 


void loop() {
   counter = 7;
    Serial.println(String(counter)+","+String(counter));
//  // try to parse packet
//  int packetSize = LoRa.parsePacket(); 
////  Serial.print(packetSize);
//  
//  if (packetSize) {  
//   while (LoRa.available())
//    {  
//      inString = LoRa.readString();  
//    }
//    humid = inString.substring(3,8);
//    temp = inString.substring(9,14);
//    s1 = inString.substring(0,3);
//
//    lcd.setCursor(0,0);
//    lcd.print("RECEIVED PACKETS");
//
//     if(checkValues(humid)){
//        humid = "0.00";
//        temp = "0.00"; 
//     } 
// 
//    lcd.setCursor(0,1);
//    lcd.print("H"); 
//    lcd.print(humid);
//    lcd.setCursor(7,1);
//    lcd.print("T"); 
//    lcd.print(temp);
//    lcd.setCursor(13,1);
//   
//    if(s1 == "...")
//      return;
//    
//    lcd.setCursor(14,1);
//    lcd.print(s1);  
    
//  } 
//  Serial.print("Receiving Packets [ humid: "+humid + " temp: "+temp + " s1: "+s1 +" ]");
//  Serial.println(LoRa.packetRssi());
   
  delay(2000); //not ideal to have delaya
}
//boolean checkValues(String data){
//  if(data != '\0')
//    return false;
//  else 
//    return true;
//}

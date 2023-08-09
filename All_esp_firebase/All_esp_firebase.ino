//AUTHOR: ROGER MONTOYA JR.
//date : July 21,2023
// Esp8266 , Firebase , Time, 

//LCD i2c
#include <LiquidCrystal_I2C.h>
#include <Wire.h>  
LiquidCrystal_I2C lcd(0x27, 20,4); 



#include <Arduino.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <WiFiUdp.h>
#include <NTPClient.h>//lib for time
#include <FirebaseESP8266.h>//lib for firebase
#include <TimeLib.h>

#include <ArduinoJson.h>

#define FIREBASE_HOST "weather-station-c541d-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "AIzaSyBo6-vuNMwKGoDHC7LALDB1tWlQJ9AdGZk"

FirebaseData firebaseData;
ESP8266WebServer server(80);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "sg.pool.ntp.org", 3600, 60000);

//VARIABLES
String readSerial;
String parentPath = "";
String databasePath= ""; 

 
  float air_temp = 0.0;
  float humidity = 0.0;
  float wind = 0.0;
  float lux = 0.0;
  int soil_temp = 0;
  float soil_moisture = 0.0;

  bool res;
  WiFiManager wm;
  
void setup() {
    Serial.begin(115200);
    Wire.begin();
    lcd.init();  
  lcd.backlight();
   
     
    timeClient.begin();
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    
    
    res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        lcd.setCursor(0,0);
        lcd.print("Faile to Connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
         lcd.setCursor(0,0);
        lcd.print("Net. Established");
    }

    databasePath = "/station1/data/";

}

void loop() {
  res = wm.autoConnect("AutoConnectAP","password");
  if(!res){
    //lcd print Error Wifi Connection
    Serial.println("Wifi not connected!");
  }else{
    //lcd print Wifi Connected

    if(Serial.available()){
    readSerial = Serial.readString(); 
    String dataString = Serial.readStringUntil('\n'); // Read the data from Serial until '\n'
    readSerialConversion(dataString); 
   
    Serial.print("air_temp: ");
    Serial.println(air_temp);
    Serial.print("humidity: ");
    Serial.println(humidity);
    Serial.print("wind: ");
    Serial.println(wind);
    Serial.print("lux: ");
    Serial.println(lux);
    Serial.print("soil_temp: ");
    Serial.println(soil_temp);
    Serial.print("soil_moisture: ");
    Serial.println(soil_moisture);

    if(isnan(air_temp) && isnan(humidity) && isnan(wind) && isnan(lux) && isnan(soil_temp) && isnan(soil_moisture) ){
      lcd.setCursor(0,1);
      lcd.print("Data Undefined.");
    }else{
      parentPath = databasePath+String(getTime());
      insertToFirebase(parentPath+"/air_temp/",String(air_temp),"string");
      insertToFirebase(parentPath+"/humidity/",String(humidity),"string");
      insertToFirebase(parentPath+"/wind_speed/",String(wind),"string");
      insertToFirebase(parentPath+"/lux/",String(lux),"string");
      insertToFirebase(parentPath+"/soil_temp/",String(soil_temp),"string");
      insertToFirebase(parentPath+"/soil_moisture/",String(soil_moisture),"string");
      lcd.setCursor(0,1);
      lcd.print("Data Sent.");
    }
    }else{
      //Lcd print
      Serial.println("Lost Connection!");
      lcd.setCursor(0,1);
      lcd.print("Data !Sent.");
    }
  }

 
  delay(60000);
}
//Inserting to firebase
void insertToFirebase(String path, String value,String dataType ){
   start:
  if(dataType == "string"){
    if(Firebase.setString(firebaseData, path, value)){
      Serial.println(" < Set Successfully:");
    } else {
      goto start;
    }
  }  
}
unsigned long getTime() {
  timeClient.update();
  unsigned long now = timeClient.getEpochTime();
  return now;
}
void readSerialConversion(String dataString){
    // Tokenize the string using strtok function
    char charArray[dataString.length() + 1]; // Create a character array for strtok
    dataString.toCharArray(charArray, sizeof(charArray)); // Convert String to char array

    char* token = strtok(charArray, ",");
    if (token != NULL) {
      air_temp = atof(token); // Convert the token to a floating-point number
      token = strtok(NULL, ",");
    }
    if (token != NULL) {
      humidity = atof(token);
      token = strtok(NULL, ",");
    }
    if (token != NULL) {
      wind = atof(token);
      token = strtok(NULL, ",");
    }
    if (token != NULL) {
      lux = atof(token);
      token = strtok(NULL, ",");
    }
    if (token != NULL) {
      soil_temp = atoi(token); // Convert the token to an integer
      token = strtok(NULL, ",");
    }
    if (token != NULL) {
      soil_moisture = atof(token);
    }

}

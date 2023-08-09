//AUTHOR: ROGER MONTOYA JR.
//date : July 21,2023
// Esp8266 , Firebase , Time, 

#include <Arduino.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <WiFiUdp.h>
#include <NTPClient.h>//lib for time
#include <FirebaseESP8266.h>//lib for firebase
#include <TimeLib.h>

#define FIREBASE_HOST "weather-station-c541d-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "AIzaSyBo6-vuNMwKGoDHC7LALDB1tWlQJ9AdGZk"
 
 
FirebaseData firebaseData;
ESP8266WebServer server(80);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

//VARIABLES
String readSerial,readSerial2;
String parentPath = "";
String databasePath= "";


int counter = 0;
int timestamp;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    
    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wm;
    timeClient.begin();
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    
    bool res;
    res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }

    databasePath = "/station1/data/sensors";
}
void loop() {
    // put your main code here, to run repeatedly:  
     Serial.println(getDateFormatted());
     Serial.println(getTimeFormatted());
     if(Serial.available()){
        readSerial = Serial.readString();
//        Serial.println(Serial.readString());
      }
      else{
        readSerial = "Esp not connected!!";
      }
       
      Serial.println(readSerial);
      Serial.println(readSerial2);
      counter++;
      timestamp = getTime();

      parentPath = databasePath + "/wind_speed/"+String(getDateFormatted());
      insertToFirebase(parentPath+"/value/",String(counter),"string");
      insertToFirebase(parentPath+"/timestamps/",String(getTimeFormatted()),"string");
 
      delay(5000);
}

//Inserting to firebase
void insertToFirebase(String path, String value, String dataType){
  start:
  if(dataType == "string"){
    if(Firebase.setString(firebaseData, path, value)){
      Serial.println(" < Set Successfully:");
    } else {
      goto start;
    }
  } else {
    if(Firebase.setFloat(firebaseData, path, value.toInt())){
      Serial.println(" < Set Successfully:");
    } else {
      goto start;
    }
  }
}
String getDateFormatted() {
  timeClient.update(); // Assuming you are using a valid NTP client like the NTPClient library
  unsigned long now = timeClient.getEpochTime();
  char buffer[30];
  sprintf(buffer, "%04d-%02d-%02d", year(now), month(now), day(now));
  return String(buffer);
} 
String getTimeFormatted(){
  timeClient.update(); // Assuming you are using a valid NTP client like the NTPClient library
  unsigned long now = timeClient.getEpochTime();
  char buffer[30];
  sprintf(buffer, "%02d:%02d %s", hourFormat12(now), minute(now), isAM(now) ? "AM" : "PM");
  
  return String(buffer);
}
// Function that gets current epoch time
unsigned long getTime() {
  timeClient.update();
  unsigned long now = timeClient.getEpochTime();
  return now;
}

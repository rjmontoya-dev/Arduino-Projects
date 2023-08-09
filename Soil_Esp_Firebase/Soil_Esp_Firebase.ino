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
String databasePath1= ""; 
String databasePath2= ""; 

int timestamp; 
String n_1,n_2;
float data1,data2 ;
int savingDelay = 1;

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

    databasePath1 = "/station1/data/sensors";
    databasePath2 = "/station1/data/sensors";
}
void loop() { 
  for(int i = 0; i<3;){
    if(Serial.available()){
      readSerial = Serial.readString();
      int i1 = readSerial.indexOf(',');
      int i2 = readSerial.indexOf(',', i1+1);
      n_1 = readSerial.substring(0, i1);
      n_2 = readSerial.substring(i1 + 1, i2); 
      data1 = n_1.toFloat();
      data2 = n_2.toFloat();
      Serial.println(data1);
      Serial.println(data2);
      if(savingDelay >= 5){
         parentPath = databasePath1 +"/moisture/"+ String(getTime());
         insertToFirebase(parentPath+"/value/",String(data1),"string");
         insertToFirebase(parentPath+"/timestamps/",String(getTime()),"string");
         
         parentPath = databasePath2  +"/temperature/"+ String(getTime());
         insertToFirebase(parentPath+"/value/",String(data2),"string");
         insertToFirebase(parentPath+"/timestamps/",String(getTime()),"string");
         
         savingDelay = 0;
      }
      }else{
        Serial.println("Lost Connection!");
      }
         i++;
   }
   savingDelay++;
   delay(1000);
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
 

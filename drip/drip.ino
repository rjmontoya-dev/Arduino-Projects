#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
 #include <ESP8266HTTPClient.h>

// Set these to run example.
#define FIREBASE_HOST= "https://drip-c4ff6-default-rtdb.firebaseio.com/";
#define FIREBASE_AUTH= "Qsnbl8nNOIZ1DyG2OyKRtN75Ig1n6uxv5qHFSLWG";
#define WIFI_SSID ="GlobeAtHome_E99B7";
#define WIFI_PASSWORD ="5TN9AEYJA1H";
 
String myString;
int vr = A0; // variable resistor connected 
int sdata = 0; // The variable resistor value will be stored in sdata.

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(vr ,INPUT);
  // connect to wifi.
  pinMode(D0,OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED)
      {
    Serial.print(".");
    delay(500);
      }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

   Firebase.setString("Variable/Value","fahad");
}

void loop()
{

sdata = analogRead(vr);
myString = String(sdata); 
Serial.println(myString); 
Firebase.setString("Variable/Value",myString);
delay(1000);            
}
 
//#include <Arduino.h>
//#include <ESP8266WiFi.h>
//#include <ESP8266WiFiMulti.h>
//#include <ESP8266HTTPClient.h>
//#include <SoftwareSerial.h>
//
//#include <WiFiClient.h>
//
//ESP8266WiFiMulti WiFiMulti;
//
// 
//
////Set Firebase
//#define FIREBASE_HOST = "https://drip-c4ff6-default-rtdb.firebaseio.com/";
//#define FIREBASE_AUTH = "Qsnbl8nNOIZ1DyG2OyKRtN75Ig1n6uxv5qHFSLWG";
//#define WIFI_SSID = "GlobeAtHome_E99B7";
//#define WIFI_PASS = "5TN9AEYJA1H";
//
// 
//void setup() {
//
//  Serial.begin(115200);
//  // Serial.setDebugOutput(true);
//
//  Serial.println();
//  Serial.println();
//  Serial.println();
//
//  WIFI.begin(WIFI_SSID,WIFI_PASS);
//   Serial.printf("connecting");
//
//   while(WIFI.status() != WL_CONNECTED){
//    Serial.printf(".");
//    delay(500);
//   }
//    Serial.printf("Connected!!");
//    Serial.printf(WIFI.localIP());
//
//    Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
//}
//
//int a;
//void loop() {
//   
//  a = Firebase.getInt("drip_status/isOpen");
//   if(Firebase.failed()){
//      Serial.printf(Firebase.error());
//   }else{
//      Serial.print(a);
//   }
//  delay(10000);
//}

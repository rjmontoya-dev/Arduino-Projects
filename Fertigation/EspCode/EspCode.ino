#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <FirebaseESP8266.h>
#include <ArduinoJson.h>


#define FIREBASE_HOST "dbconnectiot-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "Hm0PHajBUe8qwhMCST8VYAASE8nlel6E1Dr61Wz6"
FirebaseData firebaseData;
ESP8266WebServer server(80);

const unsigned int BAUD_RATE = 115200;
float arrLevel[3],sum_1 = 0;
String s_1,s_2,s_3,p_1, n_1,n_2,n_3,n_4,n_5,n_6,n_7;
String readSerial;
void setup() {
  // put your setup code here, to run once:
 Serial.begin(BAUD_RATE);
 WiFiManager wifiManager;

  wifiManager.autoConnect("fertigation");
  Serial.println("Connected.....");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);


//    tryAgain:
//  if(Firebase.getString(firebaseData, "IDRecord/Water Level ID/")) {
//    if(firebaseData.dataType() == "string") {
//      waterLvlID = firebaseData.stringData().toInt();
//    } else {
//      Serial.println(firebaseData.errorReason());
//      goto tryAgain;
//    }
//  }
}

void loop() {
  // put your main code here, to run repeatedly:
// if(Serial.available()){
//    readSerial = Serial.readString();
//    Serial.println(readSerial);
// }
for(int i = 0; i < 3;){
   if(Serial.available()){
    readSerial = Serial.readString();
     int i1 = readSerial.indexOf(','); 
      int i2 = readSerial.indexOf(',', i1+1);
      int i3 = readSerial.indexOf(',', i2+1);
      int i4 = readSerial.indexOf(',', i3+1);
      int i5 = readSerial.indexOf(',', i4+1);
      int i6 = readSerial.indexOf(',', i5+1);
      int i7 = readSerial.indexOf(',', i6+1);
      int i8 = readSerial.indexOf(',', i7+1);
      int i9 = readSerial.indexOf(',', i8+1);
      int i10 = readSerial.indexOf(',', i9+1);

     p_1 = readSerial.substring(0, i1); 
     s_1 = readSerial.substring(i1 + 1, i2);
     s_2 = readSerial.substring(i2 + 1, i3);
    
     s_3 = readSerial.substring(i3 + 1, i4);
     n_1 = readSerial.substring(i4 + 1, i5);
     n_2 = readSerial.substring(i5 + 1,i6);  
     n_3 = readSerial.substring(i6 + 1,i7); 
     n_4 = readSerial.substring(i7 + 1,i8); 
     n_5 = readSerial.substring(i8 + 1,i9); 
     n_6 = readSerial.substring(i9 + 1,i10); 
     n_7 = readSerial.substring(i10 + 1); 
     
//     n_1 = readSerial.substring(0, i1); 
//     n_2 = readSerial.substring(i1 + 1, i2);
//     n_3 = readSerial.substring(i2 + 1, i3);
//    
//     n_4 = readSerial.substring(i3 + 1, i4);
//     n_5 = readSerial.substring(i4 + 1, i5);
//     n_6 = readSerial.substring(i5 + 1,i6);  
//     s_1 = readSerial.substring(i6 + 1,i7); 
//     s_2 = readSerial.substring(i7 + 1,i8); 
//     s_3 = readSerial.substring(i8 + 1,i9); 
//     p_1 = readSerial.substring(i9 + 1,i10); 
//     n_7 = readSerial.substring(i10 + 1); 
//      arrLevel[i] = n_1.toFloat(); 
       i++;  
    }
  }
  Serial.println(p_1+","+s_1+","+s_2+","+s_3+","+n_1+","+n_2+","+n_3+","+n_4+","+n_5+","+n_6);
//   insertToFirebase("station1/waterLevel/"+String(waterLvlID)+"/value/",String(n_1),"string");
  insertToFirebase("PlanTECH/temp/",String(n_1),"string");
  insertToFirebase("PlanTECH/humid/",String(n_2),"string");
  insertToFirebase("PlanTECH/moist/",String(n_3),"string");
  insertToFirebase("PlanTECH/nitrogen/",String(n_4),"string");
  insertToFirebase("PlanTECH/Phosphorus/",String(n_5),"string");
  insertToFirebase("PlanTECH/potassium/",String(n_6),"string"); 
  insertToFirebase("TankStatus/solenoid1/",String(s_1),"string");
  insertToFirebase("TankStatus/solenoid2/",String(s_2),"string");
  insertToFirebase("TankStatus/solenoid3/",String(s_3),"string");
  insertToFirebase("TankStatus/pump/",String(p_1),"string");
//  insertToFirebase("PlanTECH/tank/",String(n_7),"string"); 

  
  delay(20000);
}
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
String getData(String DataType, String path){
  if(DataType =="int"){
    return String(GetIntData(path));
  }else{
    return String(GetStringData(path));
  }
}

int GetIntData(String path){
  if(Firebase.getInt(firebaseData, path)){
    if(firebaseData.dataType()=="int"){
      return firebaseData.intData();
    }
  }else{
    return  0;
  }
}

String GetStringData(String path){
  if(Firebase.getString(firebaseData, path)){
    if(firebaseData.dataType()=="string"){
      return  String(firebaseData.stringData());
    }
  }else{
    return "error";
  }
}
void insertString(String path, String value){
  if(Firebase.setString(firebaseData, path, value)){
    Serial.println("Data Sent Successfully");
  }else{
    Serial.println(firebaseData.errorReason()); 
  }
}

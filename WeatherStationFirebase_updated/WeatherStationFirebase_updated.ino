#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <FirebaseESP8266.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
#define FIREBASE_HOST "weather-station-c541d-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "7fVGYjeFHvd1A0fqmhviDAZmm95X2I9tKqHmP4oo"
FirebaseData firebaseData;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org");
ESP8266WebServer server(80);

String days[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", 
"Thursday", "Friday", "Saturday"};
String months[12] = {"January", "February", "March", "April", "May", 
"June", "July", "August", "September", "October", "November", "December"};

String dataVal = "10";
const unsigned int BAUD_RATE = 115200;

int waterLvlID;


float arrLevel[3],sum_1 = 0;
String n_1;
String readSerial;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUD_RATE);
  WiFiManager wifiManager;

  // Serial.println("Disconnected from Wi-Fi, trying to connect...");
  // WiFi.disconnect();
 
  wifiManager.autoConnect("weather station");
  Serial.println("Connected.....");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  timeClient.begin();
  timeClient.setTimeOffset(28800);

    tryAgain:
  if(Firebase.getString(firebaseData, "IDRecord/Water Level ID/")) {
    if(firebaseData.dataType() == "string") {
      waterLvlID = firebaseData.stringData().toInt();
    } else {
      Serial.println(firebaseData.errorReason());
      goto tryAgain;
    }
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < 3;){
   if(Serial.available()){
    readSerial = Serial.readString();
     int i1 = readSerial.indexOf(','); 
     n_1 = readSerial.substring(0, i1); 
      arrLevel[i] = n_1.toFloat(); 
       i++;  
    }
  }
  Serial.print(n_1);

    //wifi
  waterLvlID++;
  insertString("station1/waterLevelRecords/", String(waterLvlID));
  insertToFirebase("station1/waterLevel/"+String(waterLvlID)+"/value/",String(n_1),"string");
  
 
  
//  insertToFirebase("Water Level/"+ String(waterLvlID)+"/Date/", String(getToday()),"string");
//  insertToFirebase("Water Level/"+ String(waterLvlID)+"/Day/", String(getDayName()),"string");
//  insertToFirebase("Water Level/"+ String(waterLvlID)+"/Hour/", String(getHour()),"string");
//  insertToFirebase("Water Level/"+ String(waterLvlID)+"/Minute/", String(getMinute()),"string");
//  insertToFirebase("Water Level/"+ String(waterLvlID)+"/Month/", String(getMonthName()),"string");
//  insertToFirebase("Water Level/"+ String(waterLvlID)+"/Percentage/", String(wtrLvl),"string");
//  insertToFirebase("Water Level/"+ String(waterLvlID)+"/Year/", String(getYear()),"string");
  delay(10000);
}
// FUNCTIONS --------------------------------------------

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

String dateFormat(String Day, String Month, String Year) {
  return Month + " " + Day + " " + Year;
}

String getToday() {
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  struct tm * ptm = gmtime((time_t *) & epochTime);
  int monthday = ptm -> tm_mday;
  return String(monthday);
}

String getHour() {
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  struct tm * ptm = gmtime((time_t *) & epochTime);
  int currentHour = timeClient.getHours();
  return String(currentHour);
}

String getMinute() {
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  struct tm * ptm = gmtime((time_t *) & epochTime);
  int currentMinute = timeClient.getMinutes();
  return String(currentMinute);
}

String getSecond() {
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  struct tm * ptm = gmtime((time_t *) & epochTime);
  int currentSecond = timeClient.getSeconds();
  return String(currentSecond);
}

String getDayName() {
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  struct tm * ptm = gmtime((time_t *) & epochTime);
  String weekDay = days[timeClient.getDay()];
  return String(weekDay);
}

String getMonthName() {
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  struct tm * ptm = gmtime((time_t *) & epochTime);
  int currentMonth = ptm -> tm_mon + 1;
  String currentMonthName =  months[currentMonth - 1] ;
  return String(currentMonthName);
}

String getYear() {
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  struct tm * ptm = gmtime((time_t *) & epochTime);
  int currentYear = ptm -> tm_year + 1900;
  return String(currentYear);
}

//void insertInt(String path, int value){
//  if(Firebase.push(firebaseData, path, value)){
//    Serial.println("Data Sent Successfully");
//  }else{
//    Serial.println(firebaseData.errorReason()); 
//  }
//}
//
void insertString(String path, String value){
  if(Firebase.setString(firebaseData, path, value)){
    Serial.println("Data Sent Successfully");
  }else{
    Serial.println(firebaseData.errorReason()); 
  }
}

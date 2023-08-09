
#include <SoftwareSerial.h>
 
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>                         
#define FIREBASE_HOST "weather-station-c541d-default-rtdb.firebaseio.com"
//Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "7fVGYjeFHvd1A0fqmhviDAZmm95X2I9tKqHmP4oo" 
//Qsnbl8nNOIZ1DyG2OyKRtN75Ig1n6uxv5qHFSLWG
//drip-c4ff6-default-rtdb.firebaseio.com
//Your Firebase Database Secret goes here
     

#define WIFI_SSID "dong"                                              
#define WIFI_PASSWORD "123456789" 



// Declare the Firebase Data object in the global scope
FirebaseData firebaseData;
float arrLevel[3],arrHumid[3],sum_1 = 0,ave_1 = 0,sum_2 = 0,ave_2 = 0;
String n_1,n_2;
String readSerial;
void setup() {
 
     // Open serial communications and wait for port to open:
Serial.begin(115200);
     // Select the same baud rate if you want to see the data on Serial Monitor
  Serial.println("Serial communication started\n\n");           
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                    
  //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
 
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());//print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);// connect to firebase
  Firebase.reconnectWiFi(true);
  
 delay(1000);
}

void loop() {
 
  if(Serial.available()){
    Serial.print(Serial.read());
  }
  // put your main code here, to run repeatedly:
  for(int i = 0; i < 3;){
   if(Serial.available()){
    readSerial = Serial.readString();
     int i1 = readSerial.indexOf(',');
     int i2 = readSerial.indexOf(',', i1+1);
     n_1 = readSerial.substring(0, i1);
       n_2 = readSerial.substring(i1 + 1, i2);
      arrLevel[i] = n_1.toFloat();
       arrHumid  [i] = n_2.toFloat();
       i++;  
    }
  }
   for(int i = 0; i < 3; i++){
    sum_1 += arrLevel[i];
  }  
   for(int i = 0; i < 3; i++){
    sum_2 += arrHumid[i];
  }  
  ave_1 = sum_1/3;
   ave_2 = sum_2/3;
  Serial.println(String(n_1));
    Serial.println("Water_Level: " + String(n_2));
  char waterLevel_String[7];
   dtostrf(ave_1,-4,2,waterLevel_String);
  
   Firebase.setString(firebaseData,"station1/waterLevel",String(n_1));
   sum_1 = 0;
   ave_1 = 0;
   delay(2000);
}

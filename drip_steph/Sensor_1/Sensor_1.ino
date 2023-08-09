
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>                         
#define FIREBASE_HOST "e-brinjal-c778e-default-rtdb.firebaseio.com"
//Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "S7s5vyhz7r5H3VWGIzonTjFMRtErD0gFUOPD396x" 
//Your Firebase Database Secret goes here
//#define WIFI_SSID "GlobeAtHome_E99B7"                                              
//#define WIFI_PASSWORD "1234567890"        

#define WIFI_SSID "Drip-irrigation"                                              
#define WIFI_PASSWORD "drip_irrigation"
//Password of your wifi network 
// Declare the Firebase Data object in the global scope
FirebaseData firebaseData;
// Declare global variable to store value
 

int sensorPin = A0; 
int sensorValue;  
int limit = 550; 
String sensor_Status = ""; 
 
 
void setup() {
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
 
//   String wifi_isConnected = "true";
//  Firebase.setString(firebaseData,"drip/Sensor_1/wifi_isConnected","true");
  delay(1000);
}
void loop() 
{ 
  
 sensorValue = analogRead(sensorPin); 
 if (sensorValue<limit) {
  sensor_Status = "wet";
  Serial.println("Sensor Status : Wet");
 }
 else {
  sensor_Status = "dry";
 Serial.println("Sensor Status : Dry");
 }

  if(sensor_Status == "wet"){ 
  Firebase.setString(firebaseData,"drip/Sensor_1/sensor_Status","0"); 
  }
  else if (sensor_Status =="dry"){ 
    Firebase.setString(firebaseData,"drip/Sensor_1/sensor_Status","1");
  }
 

  delay(1000);
}
 

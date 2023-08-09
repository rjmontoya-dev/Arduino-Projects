
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>                         
#define FIREBASE_HOST "drip-irrigation-7b423-default-rtdb.firebaseio.com"
//Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "Aejq9HIINXbaZM7J8tsCGlQ29LOTMioDkssDD7xc" 
//Your Firebase Database Secret goes here
//#define WIFI_SSID "GlobeAtHome_E99B7"                                              
//#define WIFI_PASSWORD "5TN9AEYJA1H"        

#define WIFI_SSID "GlobeAtHome_E99B7"                                              
#define WIFI_PASSWORD "5TN9AEYJA1H" 
//Password of your wifi network 
// Declare the Firebase Data object in the global scope
FirebaseData firebaseData;
// Declare global variable to store value
int val=0;



int solenoidPin = D2; //This is the output pin on the Arduino we are using
int sensorPin = A0; 
int sensorValue;  
int limit = 550; 
String sensor_Status = "";
String drip_isOpen = "false";
String wifi_isConnected = "false";


void setup() {
  Serial.begin(115200);      

// put your setup code here, to run once:
pinMode(solenoidPin, OUTPUT); //Sets the pin as an output
 pinMode(13, OUTPUT);
                               
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
void loop() 
{ 
  
  
 sensorValue = analogRead(sensorPin); 
 if (sensorValue<limit) {
  digitalWrite(13, HIGH); 
  sensor_Status = "wet";
  
  Serial.println("Sensor Status : Wet");
 }
 else {
 digitalWrite(13, LOW); 
  sensor_Status = "dry";
 Serial.println("Sensor Status : Dry");
 }

  if(sensor_Status == "wet"){ 
    digitalWrite(solenoidPin, LOW);
    drip_isOpen = false;
    Serial.println("IRRIGATION  OFF");
   }
  else if (sensor_Status =="dry"){
      digitalWrite(solenoidPin, HIGH);
      drip_isOpen = true;
      
      Serial.println(" IRRIGATION Starting!!");
    }

  

  Firebase.setString(firebaseData,"drip/Sensor_1/sensorValue",sensorValue);

 
//  Firebase.setString(firebaseData,"drip/Sensor_1/wifi_isConnected",wifi_isConnected);

    
  delay(1000);
  
// val = random(-50, -150);
//// Firebase Error Handling And Writing Data At Specifed Path**********************************************
//if (Firebase.pushInt(firebaseData, "/Storingdata", val))
//{ 
//  // On successful Write operation, function returns 1  
//               Serial.println("Storingdata Uploaded Successfully");
//               Serial.print("Val = ");
//               Serial.println(val);
//               Serial.println("\n");
//               delay(10000);
//     }
//else {        
//    Serial.println(firebaseData.errorReason());
//  }
//
//// monitoring 
//
//if (Firebase.setInt(firebaseData, "/Monitoringdata", val))//  to set only one value
//{ 
//  // On successful Write operation, function returns 1  
//               Serial.println("Monitoringdata Uploaded Successfully");
//               Serial.print("Val = ");
//               Serial.println(val);
//               Serial.println("\n");
//               delay(10000);
//     }
//else {        
//    Serial.println(firebaseData.errorReason());
//  }
}

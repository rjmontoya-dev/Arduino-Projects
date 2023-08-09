 
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>                         
#define FIREBASE_HOST "drip-c4ff6-default-rtdb.firebaseio.com"
//Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "Qsnbl8nNOIZ1DyG2OyKRtN75Ig1n6uxv5qHFSLWG" 
//Your Firebase Database Secret goes here
//#define WIFI_SSID "GlobeAtHome_E99B7"                                              
//#define WIFI_PASSWORD "1234567890"   
     
#include <addons/RTDBHelper.h>
#include <addons/TokenHelper.h>


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

 int result = 0;




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
 
int GetIntData(String path){
  if(Firebase.getInt(firebaseData, path)){
    if(firebaseData.dataType()=="int"){
      return  firebaseData.intData();
    }
  }else{
    return  0;
  }
}

void loop() 
{ 
// Firebase.setFloat(firebaseData,"number",78.0);
//

Serial.println(GetStringData("drip/-NExcBSyHF-5bnbcXuSq/isOpen/"));
// value = Firebase.getFloat(firebaseData,"number");
// Serial.println(Firebase.getFloat(firebaseData,"/drip"));
 

//  Firebase.setString(firebaseData,"drip/Sensor_1/wifi_isConnected",wifi_isConnected);

    
  delay(1000);
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


#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>                         
#define FIREBASE_HOST "drip-irrigation-62b3a-default-rtdb.firebaseio.com"
//Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "6VmosmihJY76fPR7Wv83ZJgqQbBonb8E6wnCh8T1" 
//Qsnbl8nNOIZ1DyG2OyKRtN75Ig1n6uxv5qHFSLWG
//drip-c4ff6-default-rtdb.firebaseio.com
//Your Firebase Database Secret goes here
     

#define WIFI_SSID "Drip-irrigation"                                              
#define WIFI_PASSWORD "drip_irrigation" 
//Password of your wifi network 

// Declare the Firebase Data object in the global scope
FirebaseData firebaseData;



int solenoidPin = D2; //This is the output pin on the Arduino we are using
 
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
  
  Firebase.setString(firebaseData,"drip/-NExcBSyHF-5bnbcXuSq/isOpen/","0");
   
  delay(1000);
} 

String sensor_1,sensor_2,sensor_3,sensor_4,sensor_5;
bool fire = false;
String isOpen_value;
void loop() 
{ 
  //Get Sensors Status on Firebase
    sensor_1  = GetStringData("drip/Sensor_1/sensor_Status/");
    
    sensor_2  = GetStringData("drip/Sensor_2/sensor_Status/");
    
    sensor_3  = GetStringData("drip/Sensor_3/sensor_Status/");
    
    sensor_4  = GetStringData("drip/Sensor_4/sensor_Status/");
    
    sensor_5  = GetStringData("drip/Sensor_5/sensor_Status/");

    Serial.println("Sensor 1" + sensor_1);
    
    Serial.println("Sensor 2" + sensor_2);
    
    Serial.println("Sensor 3" + sensor_3);
    
    Serial.println("Sensor 4" + sensor_4);
    
    Serial.println("Sensor 5" + sensor_5);

//    1 = dry 0 = wet
//    1 = true 0 = false

    if(sensor_1 == "1" && sensor_2 == "1" && sensor_3 == "1" && sensor_4 == "1"  && sensor_5 == "1" ){
       fire = true;
     }
     if(sensor_1 == "0" && sensor_2 == "0" && sensor_3 == "0" && sensor_4 == "0"  && sensor_5 == "0"){
       fire = false;
     }
    Serial.println(fire);
    
  if(fire){
    digitalWrite(solenoidPin, HIGH);  
     Firebase.setString(firebaseData,"drip/-NExcBSyHF-5bnbcXuSq/isOpen/","1");
  }
  if(!fire){
      digitalWrite(solenoidPin, LOW); 
        Firebase.setString(firebaseData,"drip/-NExcBSyHF-5bnbcXuSq/isOpen/","0");
    }

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

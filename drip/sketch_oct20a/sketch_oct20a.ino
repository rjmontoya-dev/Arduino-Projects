//https://www.grc.com/fingerprints.htm  kapag nd gumana pag save sa firebase 


#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
ESP8266WiFiMulti WiFiMulti;

#define FIREBASE_HOST "drip-c4ff6-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "Qsnbl8nNOIZ1DyG2OyKRtN75Ig1n6uxv5qHFSLWG"
#define WIFI_SSID "Dong"
#define WIFI_PASSWORD "1234567890"

 int age = 20;
void setup() {
// Open serial communications and wait for port to open:
Serial.begin(115200);
    

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("connecting");
    
    while (WiFi.status() != WL_CONNECTED){
      Serial.print(".");

      delay(500);
      }
    
// if connected na sya tutunog sya
    Serial.print("connected: ");
 
    Serial.println(WiFi.localIP());
    
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
     delay(10);
     
}

void loop() { // run over and over
    
 

        Firebase.setInt("drip/",age);
       

         

}

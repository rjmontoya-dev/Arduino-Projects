
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>                         
#define FIREBASE_HOST "e-brinjal-c778e-default-rtdb.firebaseio.com"
//Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "S7s5vyhz7r5H3VWGIzonTjFMRtErD0gFUOPD396x" 
//Your Firebase Database Secret goes here
 #include <NTPClient.h>    
#include <WiFiUdp.h>
//#define WIFI_SSID "dong"                                              
//#define WIFI_PASSWORD "12345678" 
#define WIFI_SSID "Drip-irrigation"                                              
#define WIFI_PASSWORD "drip_irrigation" 
//Password of your wifi network 

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org");
// Declare the Firebase Data object in the global scope
FirebaseData firebaseData;



int solenoidPin = D2; //This is the output pin on the Arduino we are using
 String days[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", 
"Thursday", "Friday", "Saturday"};
String months[12] = {"January", "February", "March", "April", "May", 
"June", "July", "August", "September", "October", "November", "December"};

void setup() {
  Serial.begin(115200);      

// put your setup code here, to run once:
 pinMode(solenoidPin, OUTPUT); //Sets the pin as an output
 pinMode(13, OUTPUT);
                               
  // Select the same baud rate if you want to see the data on Serial Monitor
  Serial.println("Serial communication started\n\n");           
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);  
  
  
  timeClient.begin();
  timeClient.setTimeOffset(28800);                                  
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
  
  Firebase.setString(firebaseData,"drip/drip_status/isOpen/","0");
   
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
 
    Serial.println(dateFormat(getMonthName(),getToday(),getYear(),getHour(),getMinute(),getSecond())); 
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
     Firebase.setString(firebaseData,"drip/drip_status/isOpen/","1"); 
     String dateVar = dateFormat(getMonthName(),getToday(),getYear(),getHour(),getMinute(),getSecond());
     Firebase.setString(firebaseData,"history/"+dateVar+"/drip_duration/","");
     Firebase.setString(firebaseData,"history/"+dateVar+"/sensor_status/","wet");
  }
  if(!fire){
      digitalWrite(solenoidPin, LOW); 
        Firebase.setString(firebaseData,"drip/drip_status/isOpen/","0"); 
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
String dateFormat( String Month, String Day, String Year,String getHour, String getMinute, String getSecond) {
  return Month + " " + Day + " " + Year + " "+ getHour+":"+getMinute+":"+getSecond;
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

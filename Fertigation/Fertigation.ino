//Soil Sensor
#include <AltSoftSerial.h>
#include <SoftwareSerial.h>
// RO to pin 8 & DI to pin 9 when using AltSoftSerial
#define RE 6
#define DE 7

//Data Sets
const byte nitro[] = {0x01,0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos[] =  {0x01, 0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte pota[] =  {0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0}; 
const byte soil_ph[] =   {0x01,0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0B};
const byte soil_moist[] ={0x01, 0x03, 0x00, 0x12, 0x00, 0x01, 0x24, 0x0f};
const byte temp_humid[] =     {0x01, 0x03, 0x00, 0x12, 0x00, 0x01, 0x64, 0x0e};

byte values[11];
AltSoftSerial mod;
SoftwareSerial espSerial(4, 5);

//Humidity
#include "DHT.h"
#define DHTPIN 2   
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

//Water
#define sensorPower 5
#define water_Pin A0

//Moisture
int moist_Pin = A1; 




//// Value for storing water level
int val = 0;
////Value for storing Moisture
//int sensorValue;  
//int actual_moist = 0;
unsigned long actual_moist, sensorValue, level, humidity_Value,temp_Value, pH, moisture, humidity, nitrogen, phosphorus, potassium;
  
//Pump
const int PUMP_RELAY_PIN = A5;
//Solenoid
const int SOLENOID_RELAY_PIN = A4;




//Relays 
  int relay_nitrogen_1 = 10;
  int relay_phosphorus_2 = 11;
  int relay_potassium_3 = 12;

  //Statuses
  int pump_status;
  int solenoid1_status,solenoid2_status,solenoid3_status;
void setup() {
   Serial.begin(115200);
  espSerial.begin(115200);
  mod.begin(9600);

  pinMode(PUMP_RELAY_PIN, OUTPUT);
  pinMode(SOLENOID_RELAY_PIN, OUTPUT);
  
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);

   // put RS-485 into receive mode
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
  
  //Water Sensor
  pinMode(sensorPower, OUTPUT);
  // Set to LOW so no power flows through the sensor
  digitalWrite(sensorPower, LOW);

  //Moisture Sensor
  pinMode(13, OUTPUT);
  pinMode(11,OUTPUT);


//   Relays
  pinMode(relay_nitrogen_1,OUTPUT);
  pinMode(relay_phosphorus_2,OUTPUT);
  pinMode(relay_potassium_3,OUTPUT);
  dht.begin();
}

void loop() {
   humidity_Value = dht.readHumidity();//Read Humidity
   temp_Value  = dht.readTemperature();//Read Temp
    level = readSensor();//Read Water
   sensorValue = analogRead(moist_Pin); //Read Moisture
   actual_moist = map(sensorValue,789,0,0,100);//Convert to percent
   
//   if (isnan(humidity_Value) || isnan(level) || isnan(actual_moist)) {
//    Serial.println(F("Failed to read from Sensors!!"));
//    
//    return;
//  }
//
//  Serial.print(F("Humidity: "));
//  Serial.println(humidity_Value);
//  Serial.print("Water level: ");
//  Serial.println(level);
//  Serial.print("Moisture Value : ");
//  Serial.println(actual_moist);

  
    for(int i = 0; i < 3; i++) {
     nitrogen = getNitrogen();
     delay(250);
     
     phosphorus = getPhosphorous();
     delay(250);
     
    potassium = getPotassium();
     delay(250);
     
     pH = getPh();
     delay(250);
  
         moisture = getMoisture();
    delay(250);
  
    humidity = getHumidity();
    delay(250);
      if(nitrogen == 0 || phosphorus == 0 || potassium == 0){
        pH = 0;
      }
    double getPh = pH/100.00;

    if(moisture < 100   ){
      digitalWrite(PUMP_RELAY_PIN, HIGH);
      pump_status = 1;
    }else if (moisture == 0){
      digitalWrite(PUMP_RELAY_PIN, LOW);
       pump_status = 0;
    }
    else{
          digitalWrite(PUMP_RELAY_PIN, LOW);
           pump_status = 0;
    }
    
    if(nitrogen > 50 || phosphorus > 50 || potassium > 50){
//      relay ooff
      digitalWrite(relay_nitrogen_1,LOW);
      digitalWrite(relay_phosphorus_2,LOW);
      digitalWrite(relay_potassium_3,LOW);
      solenoid1_status = 0;
      solenoid2_status = 0;
      solenoid3_status = 0;
//      Serial.println("OFF");
    }
    else if((nitrogen <= 25 && nitrogen >0) || (phosphorus<= 25 && phosphorus > 0) || (potassium<=25 && potassium >0) ){
//      relay onn
      digitalWrite(relay_nitrogen_1,HIGH);
      digitalWrite(relay_phosphorus_2,HIGH);
      digitalWrite(relay_potassium_3,HIGH);
      solenoid1_status = 1;
      solenoid2_status = 1;
      solenoid3_status = 1;
//      Serial.println("ON");
    }else{
       digitalWrite(relay_nitrogen_1,LOW);
      digitalWrite(relay_phosphorus_2,LOW);
      digitalWrite(relay_potassium_3,LOW);
       solenoid1_status = 0;
      solenoid2_status = 0;
      solenoid3_status = 0;
//       Serial.println("OFF");
    }

    //pump_status,solenoid1_status,solenoid2_status,solenoid3_status
//    String(pump_status) +","+ String(solenoid1_status) +","+  String(solenoid2_status) +","+  String(solenoid3_status) +","+
//    Serial.println(String(humidity_Value) +"," + String(level)+","+ String(actual_moist) +","+ String(getPh) + "," + String(nitrogen) + "," + String(phosphorus) + "," + String(potassium));
 Serial.println(String(pump_status) +","+ String(solenoid1_status) +","+  String(solenoid2_status) +","+  String(solenoid3_status) +","+ String(temp_Value) +","+String(humidity_Value) +","+ String(moisture) + "," + String(nitrogen) + "," + String(phosphorus) + "," + String(potassium)+ "," + String(7));

 
  
  
//    delay(1000);
    }
    
  delay(2000);
}

//This is a function used to get the reading
int readSensor() {
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  delay(10);              // wait 10 milliseconds
  val = analogRead(water_Pin);    // Read the analog value form sensor
  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
  return val;             // send current reading
}

unsigned long getHumidity(){
   // clear the receive buffer
  mod.flushInput();

  // switch RS-485 to transmit mode
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);

  // write out the message
  for (uint8_t i = 0; i < sizeof(temp_humid); i++ ) mod.write( temp_humid[i] );

  // wait for the transmission to complete
  mod.flush();
  
  // switch RS-485 to receive mode
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  // crude delay to allow response bytes to be received!
  delay(100);

  // read in the received bytes
  for (byte i = 0; i < 7; i++) {
    values[i] = mod.read();
  }
   unsigned long combined = 0;
   combined = values[3];
   combined = (combined << 8) | values[4];
   return combined;
}

 unsigned long getMoisture() {
  // clear the receive buffer
  mod.flushInput();

  // switch RS-485 to transmit mode
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);

  // write out the message
  for (uint8_t i = 0; i < sizeof(soil_moist); i++ ) mod.write( soil_moist[i] );

  // wait for the transmission to complete
  mod.flush();
  
  // switch RS-485 to receive mode
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  // crude delay to allow response bytes to be received!
  delay(100);

  // read in the received bytes
  for (byte i = 0; i < 7; i++) {
    values[i] = mod.read();
//    Serial.print(values[i], HEX);
//    Serial.print(' ');
  }

    unsigned long combined = 0;
   combined = values[3];
   combined = (combined << 8) | values[4];
   return combined;
   
//    return values[4];
}

 unsigned long getPh() {
  // clear the receive buffer
  mod.flushInput();

  // switch RS-485 to transmit mode
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);

  // write out the message
  for (uint8_t i = 0; i < sizeof(soil_ph); i++ ) mod.write( soil_ph[i] );

  // wait for the transmission to complete
  mod.flush();
  
  // switch RS-485 to receive mode
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  // crude delay to allow response bytes to be received!
  delay(100);

  // read in the received bytes
  for (byte i = 0; i < 7; i++) {
    values[i] = mod.read();
//    Serial.print(values[i], HEX);
//    Serial.print(' ');
  }
//  return values[4];

 unsigned long combined = 0;
   combined = values[3];
   combined = (combined << 8) | values[4];
   return combined;
}

 unsigned long getNitrogen() {
  // clear the receive buffer
  mod.flushInput();

  // switch RS-485 to transmit mode
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);

  // write out the message
  for (uint8_t i = 0; i < sizeof(nitro); i++ ) mod.write( nitro[i] );

  // wait for the transmission to complete
  mod.flush();
  
  // switch RS-485 to receive mode
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  // crude delay to allow r`onse bytes to be received!
  delay(100);

  // read in the received bytes
  for (byte i = 0; i < 7; i++) {
    values[i] = mod.read();
//    Serial.print(values[i], HEX);
//    Serial.print(' ');
  }
//  return values[4];
 unsigned long combined = 0;
  combined = values[3];
  combined = (combined << 8) | values[4];
  return combined;
}

 unsigned long getPhosphorous() {
  mod.flushInput();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  for (uint8_t i = 0; i < sizeof(phos); i++ ) mod.write( phos[i] );
  mod.flush();
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  delay(100);
  for (byte i = 0; i < 7; i++) {
    values[i] = mod.read();
//    Serial.print(values[i], HEX);
//    Serial.print(' ');
  }
//  return values[4];
  unsigned long combined = 0;
    combined = values[3];
    combined = (combined << 8) | values[4];
    return combined;
}

 unsigned long getPotassium() {
  mod.flushInput();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  for (uint8_t i = 0; i < sizeof(pota); i++ ) mod.write( pota[i] );
  mod.flush();
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  delay(100);
  for (byte i = 0; i < 7; i++) {
    values[i] = mod.read();
//    Serial.print(values[i], HEX);
//    Serial.print(' ');
  }
//  return values[4];
  unsigned long combined = 0;
   combined = values[3];
   combined = (combined << 8) | values[4];
   return combined;
}

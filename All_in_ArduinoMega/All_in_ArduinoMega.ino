//Roger Montoya Jr.

#include <SoftwareSerial.h>
#define BAUDRATE 115200 

//SD card
#include <SD.h>
#include <SPI.h>


//LCD i2c
#include <LiquidCrystal_I2C.h>
#include <Wire.h>  
LiquidCrystal_I2C lcd(0x27, 20,4); 

//Soil Temperature
#include <OneWire.h>
#include <DallasTemperature.h> 
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

////Light Intensity 
#include <BH1750.h>
BH1750 lightMeter;

//DHT Sensor
#include "DHT.h"  
#define DHTPIN 7   
#define DHTTYPE DHT11  
DHT dht (DHTPIN, DHTTYPE); 

const int moisture_sensor = A0; // Analog input pin that the Sensor is attached to A0
int moisture_read = 0; // value read from the Soil Moisture


//Wind Speed Sensor
int m_time = 1;
int wind_ct = 0;
float wind = 0.0;

//Data Variables---------------------------------------
float wind_val = 0.0;
float lux_val = 0.0;
float soil_temp;
float env_t_val,env_h_val;
int soil_moisture;
//-------------------------------------------------------


unsigned long startTime;
unsigned long totalElapsedTime = 0;
const unsigned long targetTime = 6000000; // 6000 seconds (6000 * 1000 milliseconds)

const int chipSelect = 53; // always on pin 10-CS pin of the microSD module
File dataFile;  // soilTempFile is the name of the file


void setup() { 

  Serial.begin(BAUDRATE);
  startTime = millis(); // Record the starting time
  Wire.begin(); 
  dht.begin();
  lightMeter.begin(); 
  lcd.init();  
  lcd.backlight();
  lcd.setBacklight(HIGH);

  if(SD.begin(chipSelect)){
    Serial.println("SD Initialized."); 
  }else{
    Serial.println("Failed");
    return;
  }
  
  dataFile = SD.open("dataFile.txt", FILE_WRITE); //opening the file in the sd card
   if (dataFile) {            // if it opens it prints text
    dataFile.println();      // beginning of the writing the data gathered
    dataFile.print("Time");
    dataFile.print(",  ");
    dataFile.print("Relative humidity, ");
    dataFile.print("Air temperature, ");
    dataFile.print("Soil Moisture, ");
    dataFile.print("Soil Temp, ");
    dataFile.print("Wind Speed, ");
    dataFile.print("Light Intensity");
    dataFile.println();
  }
  dataFile.close();

}

void loop() {
   dataFile = SD.open("dataFile.txt", FILE_WRITE); 
   getEnv();
   getWind();
   getLight(); 
   getSoilMoisture();
   getSoilTemp();
   if(dataFile){
     sendData();
     
     lcdPrint_Env(env_t_val,env_h_val);
     lcdPrint_wind_light(wind_val,lux_val);
     lcdPrint_Soil(soil_temp,soil_moisture);
     Serial.println(String(env_t_val)+","+String(env_h_val)+","+String(wind_val)+","+String(lux_val)+","+String(soil_moisture)+","+String(soil_temp));
     
     dataFile.close();
   }else{
      Serial.println("error opening file");
      lcdErrorPrint();
      delay(2000);
      setup();
    } 
    
   

//   Serial.print("Env Temp: ");
//   Serial.println(env_t_val);
//   Serial.print("Env Humid: ");
//   Serial.println(env_h_val); 
//   Serial.print("Wind Speed: ");
//   Serial.println(wind_val);
//   Serial.print("Light:  ");
//   Serial.println(lux_val);
//   Serial.print("Soil Moisture: ");
//   Serial.println(soil_moisture);
//   Serial.print("Soil Temp: ");
//   Serial.println(soil_temp);
   

   delay(1000);
}
void sendData(){
  unsigned long elapsedTime = millis() - startTime; // Calculate the elapsed time in milliseconds
    
  // Update the totalElapsedTime with the elapsed time in this iteration
  totalElapsedTime += elapsedTime;
  
  if (totalElapsedTime >= targetTime) { 
    unsigned long seconds = totalElapsedTime / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60; 
    
    if (dataFile) {
        dataFile.print(hours);
        dataFile.print(":");
        dataFile.print(minutes % 60);
        dataFile.print(":");
        dataFile.print(seconds % 60);
        dataFile.print("->  ");
        dataFile.print(env_h_val);
        dataFile.print(",    ");
        dataFile.print(env_t_val);
        dataFile.print(",    ");
         dataFile.print(soil_moisture);
        dataFile.print(",    ");
        dataFile.print(soil_temp);
        dataFile.print(",    ");
        dataFile.print(wind_val);
        dataFile.print(",    ");
        dataFile.print(lux_val); 
        dataFile.println(); 
    } 
    // Reset the startTime to start counting for the next 6000 seconds
    startTime = millis();
    totalElapsedTime = 0;

    dataFile.flush();
  }
  dataFile.close(); 
}
void getEnv(){
   env_t_val = dht.readTemperature();       //acquiring data throught rh sensor
   env_h_val = dht.readHumidity();
}
void getWind(){
  wind_ct = 0;
  attachInterrupt(1, countWind, RISING);
  delay(1000 * m_time);
  detachInterrupt(1);
  wind = (float)wind_ct / (float)m_time * 2.4;
  wind_val = wind / 3.6;
}
void countWind() {
  wind_ct ++;
}
void getLight(){
  lux_val = lightMeter.readLightLevel();
}
void getSoilMoisture(){
  moisture_read = analogRead(moisture_sensor);
  soil_moisture = map(moisture_read, 1013, 0, 0, 100);
}
void getSoilTemp(){
  sensors.requestTemperatures(); // Send the command to get temperatures
  soil_temp = sensors.getTempCByIndex(0);
}
void lcdPrint_Env(float temp,float humid){
  lcd.setCursor(0,1);
  lcd.print("Tmp:");
  lcd.print(temp,2);
  lcd.setCursor(0,2);
  lcd.print("Hmd:");
  lcd.print(humid,2);
}
void lcdPrint_wind_light(float wind, float light){
  lcd.setCursor(0,3);
  lcd.print("Wnd:");
  lcd.print(wind,2);
  lcd.setCursor(10,1); 
  lcd.print("Lux:");
  lcd.print(light,2);
}
void lcdPrint_Soil(float temp, int moisture ){
    lcd.setCursor(10,2);
    lcd.print("tmp:");
    lcd.print(temp,2);
    lcd.setCursor(10,3);
    lcd.print("Mst:");
    lcd.print(moisture);
    lcd.print("%");
}
 void lcdErrorPrint(){
   lcd.setCursor(0,0);
   lcd.print("File Opening Err."); 
 }

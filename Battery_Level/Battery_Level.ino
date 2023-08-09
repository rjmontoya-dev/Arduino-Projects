//Test first the reading of Analog A0;
//And change the minVoltage and MaximumVoltage dependeing on the minimum value and max value of A0;

//10k Resistor
//Gnd 
//Vin
//A0


// LCD
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
 

const int batteryPin = A0; // Analog pin connected to the battery voltage divider
const float minVoltage = 3.5; // Minimum voltage (corresponding to analog reading of 19)
const float maxVoltage = 4.2; // Maximum voltage (corresponding to analog reading of 24)

void setup() {
  Serial.begin(9600); // Initialize serial communication for displaying the battery level
  // //   LCD ------------------
  lcd.init();                      // initialize the lcd 
  lcd.init(); 
  lcd.backlight();  
////   LCD ------------------
}

void loop() {
  float analogReading = analogRead(batteryPin); // Read the analog value from the battery pin

  
// float batteryVoltage = (analogReading * 5.0) / 1023.0; // Convert the analog value to voltage
float batteryVoltage = map(analogReading, 15, 18, minVoltage, maxVoltage); // Convert the analog value to voltage
//float batteryVoltage = analogReading * (5.16 / 1023.00) * 2;
Serial.println(analogReading);
// Estimate the battery level based on your b attery characteristics
  int batteryLevel = map(batteryVoltage, 3.5, 4.2, 0, 100); // Adjust the voltage range based on your battery's discharge curve
//  map(batteryVoltage, 0.0(minimum value battery volatage), 7.39 (maximum expected voltage), 0 (0% battery level), 100 ( 100 % battery level));
// int batteryLevel = map(analogReading, 15, 18, 0, 100); // Map the analog reading directly to the battery level range

  Serial.print("Battery Voltage: ");
  Serial.print(batteryVoltage);
  Serial.print("V, Battery Level: ");
  Serial.print(batteryLevel);
  Serial.println("%");
  lcd.setCursor(0,0);
  lcd.print("Voltage: ");
  lcd.print(batteryVoltage);
  lcd.setCursor(0,1);
  lcd.print("Level: ");
  lcd.print(batteryLevel);
  lcd.print(" %");
//  lcd.print(analogReading);
  delay(1000); 
}

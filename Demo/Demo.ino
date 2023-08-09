//Libraries for LCD screen
#include <LiquidCrystal_I2C.h>             
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define BAUDRATE 9600
#define pump A0
const int moisture_sensor = A3;
int moisture = 0;
int soil_moisture = 0;

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Soil MC");
  lcd.print(":  ");
  lcd.print(" Pump");
  pinMode (pump, OUTPUT);
    Serial.println();
    Serial.print("Soil Moisture,");
    Serial.print(",  ");
    Serial.println();
}
void loop() {
      moisture = analogRead(moisture_sensor);
      float soil_moisture = map(moisture, 690, 0, 0, 100);

      moisture = analogRead(moisture_sensor);
      soil_moisture = map(moisture, 690, 0, 0, 100);

      Serial.print(", ");
      Serial.print(soil_moisture);
      Serial.print(", ");
      Serial.println();

      lcd.setCursor(0, 1);
      lcd.print(" ");
      lcd.setCursor(0, 1);
      lcd.print(soil_moisture);
      lcd.print("%");

    if(soil_moisture <= 40){
    digitalWrite(A0, HIGH);
    lcd.setCursor(10, 1);
    lcd.print(" ON ");
  }
  else {
    digitalWrite(A0, LOW);
    lcd.setCursor(10, 1);
    lcd.print(" OFF");
  }
      delay(1000);
}

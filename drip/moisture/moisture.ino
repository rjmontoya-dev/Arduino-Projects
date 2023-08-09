int sensorPin = A0; 
int sensorValue;  
int limit = 550; 

void setup() {
 Serial.begin(115200);
 pinMode(13, OUTPUT);
}

void loop() {

 sensorValue = analogRead(sensorPin); 
 Serial.println("Analog Value : ");
 Serial.println(sensorValue);
 
 if (sensorValue<limit) {
  digitalWrite(13, HIGH); 
  Serial.println("Wet");
 }
 else {
 digitalWrite(13, LOW); 
 Serial.println("Dry");
 }
 
 delay(1000); 
}

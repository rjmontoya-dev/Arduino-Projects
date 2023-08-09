int sensorPin = A0; 
int sensorValue;  
int limit = 300;   
int actual_soil = 0;
void setup() {
 Serial.begin(9600);
 pinMode(13, OUTPUT);
   pinMode(11,OUTPUT);
}

void loop() {

 sensorValue = analogRead(sensorPin); 
actual_soil = map(sensorValue,674,0,0,100);
  
 Serial.println("Analog Value : ");
 Serial.println(actual_soil);

  if(actual_soil <= 30) {
    //High 
     digitalWrite(11,HIGH);
   
  }else{
    //Low
  digitalWrite(11,LOW);
  }

// 
// if (sensorValue<limit) {
// digitalWrite(13, HIGH); 
// }
// else {
// digitalWrite(13, LOW); 
// }
// 
 delay(1000); 
}

int solenoidPin = D2; //This is the output pin on the Arduino we are using

int sensorPin = A0; 
int sensorValue;  
int limit = 550; 

 String sensor_Status = "";
void setup() {
// put your setup code here, to run once:
pinMode(solenoidPin, OUTPUT); //Sets the pin as an output

Serial.begin(115200);
 pinMode(13, OUTPUT);

}

void loop() {


 sensorValue = analogRead(sensorPin); 
// Serial.println("Analog Value : ");
// Serial.println(sensorValue);
 
 if (sensorValue<limit) {
  digitalWrite(13, HIGH); 
  sensor_Status = "wet";
  Serial.println("Sensor Status : Wet");
 }
 else {
 digitalWrite(13, LOW); 
  sensor_Status = "dry";
 Serial.println("Sensor Status : Dry");
 }

  if(sensor_Status == "wet"){ 
    digitalWrite(solenoidPin, LOW);
     Serial.println("IRRIGATION  OFF");
   }
  else if (sensor_Status =="dry"){
      digitalWrite(solenoidPin, HIGH);
      Serial.println(" IRRIGATION Starting!!");
    }
  
delay(1000); //Wait 1 Second
}

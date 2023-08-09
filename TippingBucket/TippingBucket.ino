 
//Tipping bucket wire
//black - G
//white - 3

const byte interruptPin = 3;
const int interval = 500;
volatile unsigned long tiptime = millis(); 
 double rainrate = 0.00;
void setup() {
  
  Serial.begin(9600); 
  
  // Set up our digital pin as an interrupt
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), count, RISING);
  
}

void loop() {
  
}

void count() {
  // Grab the current ms count for common calculations
  unsigned long curtime = millis();
  
  // Make sure we don't record bounces
  if ((curtime - tiptime) < interval) {
    return;
  }

  // How long since the last tip?
  unsigned long tipcount = curtime - tiptime;
  tiptime = curtime;
  
  // Calculate mm/hr from period between cup tips
    rainrate = 914400.0 / tipcount;
  
  Serial.print("Cup tip: ");
  Serial.print(tipcount);
  Serial.println("ms");  

  Serial.print("Sec: ");
  Serial.print(tipcount/1000);
  Serial.println("sec"); 
  
  Serial.print("Cup tip: ");
  
  Serial.print("Rain rate: ");
  Serial.print(rainrate);
  Serial.println("mm/hr");  

  Serial.println(digitalRead(interruptPin));
}

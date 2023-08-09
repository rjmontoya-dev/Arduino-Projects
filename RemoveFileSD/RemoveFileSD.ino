#include <SD.h>

#define PIN_SPI_CS 4

File file;

void setup() {
  Serial.begin(9600);

  if (!SD.begin(PIN_SPI_CS)) {
    Serial.println(F("SD CARD FAILED, OR NOT PRESENT!"));
    while (1); // don't do anything more:
  }

  Serial.println(F("SD CARD INITIALIZED."));
  SD.remove("datalog.txt");// delete the file if existed
  
  
}

void loop() {
}

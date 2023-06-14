#include "Arduino.h"
#include <SoftwareSerial.h>
 
SoftwareSerial mySerial(2, 3);  // e32 TX e32 RX
 
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  
  delay(500);
  
  Serial.println("TX");  
}

int i = 0;

void loop() {
  delay(2000);  
  int n = i%2;
  mySerial.write(n);
  Serial.println(n);
  i++;
}
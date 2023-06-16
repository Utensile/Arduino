#include "Arduino.h"

#define SerialRate 9600


float i=0.0;

void setup() {
  Serial.begin(SerialRate);
}
 
void loop() {
  Serial.print(i);
  Serial.print(" ");
  Serial.println(millis());
  if(millis()%24000<3000){
    i*=1.00+float(random(0,10))/100;
    i+=random(0, 100);
  }
  else
    i*=1/(1.00+float(random(0,5))/100);
  delay(100);
}

#include "Arduino.h"

#define SerialRate 9600


float i=0.0;
float hum=50.0;
float temp=20.0;

void setup() {
  Serial.begin(SerialRate);
}
 
void loop() {
  Serial.print(int(i));
  Serial.print(" ");
  Serial.print(millis());
  Serial.print(" ");
  Serial.print(hum);
  Serial.print(" ");
  Serial.print(temp);
  Serial.print(" ");
  Serial.println(millis()*0.1);
  if(millis()%24000<3000){
    i*=1.00+float(random(0,10))/100;
    i+=random(0, 100);
    temp+=0.03;
    hum-=0.1;
  }
  else{
    i*=1/(1.00+float(random(0,5))/100);
    temp-=0.01;
    hum+=0.02;
  }
  delay(100);
}

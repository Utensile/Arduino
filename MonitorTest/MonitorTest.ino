#include "Arduino.h"
#include <SoftwareSerial.h>

#define SerialRate 9600
#define LaunchPin 4

SoftwareSerial lora(2, 3); // e32 TX e32 RX
float i=0.5;
long t=-2100;
long t2=-9999;
long t3=-9999;

void setup() {
  pinMode(LaunchPin, OUTPUT);
  pinMode(5, OUTPUT);
  Serial.begin(SerialRate);
  lora.begin(SerialRate); 
  Serial.println("Communication Begun");  
}
 
void loop() {
 if (lora.available()) {
   float in = lora.parseFloat();
   if(in!=0.00)
    Serial.println(in);
 }
 if(Serial.available()){
  int k=Serial.read();
  lora.write(k);
  k-=48;
  Serial.println(k);
  if(k==1){
    digitalWrite(LaunchPin, HIGH);
    Serial.println("Launched!");
    t=millis();
  }
  if(k==2){
    digitalWrite(5, HIGH);
    Serial.println("Parachute");
    t3=millis();
  }
 }
 if(millis()-t>=2000 && digitalRead(LaunchPin)==HIGH)
  digitalWrite(LaunchPin, LOW);
if(millis()-t3>=1000 && digitalRead(5)==HIGH)
  digitalWrite(5, LOW);

  if(millis()-t2>500){
    Serial.print(i);
    Serial.println(" m");
    i*=1.00+float(random(0,10))/100;
    t2=millis();
  }

}
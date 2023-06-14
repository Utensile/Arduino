#include "Arduino.h"
#include <SoftwareSerial.h>

#define SerialRate 9600
#define LaunchPin 7

SoftwareSerial lora(2, 3); // e32 TX e32 RX

long t=-2100;

void setup() {
  pinMode(LaunchPin, OUTPUT);
  Serial.begin(SerialRate);
  lora.begin(SerialRate); 
  Serial.println("Connection Enstablished!");  
}
 
void loop() {
 if (lora.available()) {
   float in = lora.parseFloat();
   if(in!=0.00){
    Serial.print(in);
    Serial.println(" m");
   }
 }
 if(Serial.available()){
  int k=Serial.read();
  lora.write(k);
  k-=48;
  //Serial.println(k);
  if(k==1){
    digitalWrite(LaunchPin, HIGH);
    Serial.println("Launched!");
    t=millis();
  }
 }
 if(millis()-t>=2000 && digitalRead(LaunchPin)==HIGH)
  digitalWrite(LaunchPin, LOW);
}
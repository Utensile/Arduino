#include <IRremote.h>

#define open 0x2ADE08A3
#define close 0x7F613A3B
#define plus 0x1A46B33F
#define minus 0x9F3E1063
#define repeat 0xFFFFFFFF

// Connections to A4988
const int dirPin = 3;  // Direction
const int stepPin = 4; // Step

const int IR_PIN = 8;
unsigned long key = 0;

IRrecv irrecv(IR_PIN);
decode_results results;

// Motor steps per rotation
const int STEPS_PER_REV = 200;
int s=1500;
int t=-10000;
bool active=false;

void step(int speed){
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(speed);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(speed);
}

void changeDir(){
  if(digitalRead(dirPin))
    digitalWrite(dirPin, LOW);
  else
    digitalWrite(dirPin, HIGH);
}

 
void setup() {
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  Serial.begin(9600);
  irrecv.enableIRIn();

}

void loop() {
  //Serial.println("loop");
  if (irrecv.decode(&results)){
        Serial.println(results.value, HEX);
        if(results.value!=repeat)
          key=results.value;
        switch(key){
          case close:
            digitalWrite(dirPin, LOW);
            active=true;
            t=millis();
            break;
          case open:
            digitalWrite(dirPin, HIGH);
            active=true;
            t=millis();
            break;
          case plus:
            if(s>400)
              s-=100;
            break;
          case minus:
            if(s<3000)
              s+=100;
            break;
        }
        irrecv.resume();
  }
  if(active)
    step(s);
  if(millis()-t>110)
    active=false;
}

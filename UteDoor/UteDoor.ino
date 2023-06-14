#include <IRremote.h>

#define open 0xFF22DD
#define close 0xFF02FD
#define plus 0xFFA857
#define minus 0xFFE01F
#define repeat 0xFFFFFF

// Connections to A4988
const int dirPin = 4;  // Direction
const int stepPin = 3; // Step

const int IR_PIN = 7;
unsigned long key = 0;

IRrecv irrecv(IR_PIN);
decode_results results;

// Motor steps per rotation
const int STEPS_PER_REV = 200;
int s=700;

void step(int speed){
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(speed);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(speed);
}

void setup() {
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);
}

void loop() {
  if (irrecv.decode(&results)){
        Serial.println(results.value, HEX);
        if(results.value!=repeat)
          key=results.value;
        switch(key){
          case close:
            digitalWrite(dirPin, LOW);
            step(s);
            break;
          case open:
            digitalWrite(dirPin, HIGH);
            step(s);
            break;
          case plus:
            s+=10;
            break;
          case minus:
            s-=10;
            break;
        }
        irrecv.resume();
  }
}

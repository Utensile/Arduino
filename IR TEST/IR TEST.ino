#include <IRremote.h> //Make sure to install the library
int IRpin=2;
IRrecv IR(IRpin);
decode_results cmd;
String myCom;
void setup()
{
Serial.begin(9600);
IR.enableIRIn();
}
 
void loop() {
  while (IR.decode(&cmd)==0){ 
}
Serial.println(cmd.value,HEX);
IR.resume();
 
if (cmd.value==0xFF6897){
  myCom="zero";
  Serial.println(myCom); 
}
if (cmd.value==0xFFA25D){
  myCom="pwr";
  Serial.println(myCom); 
}
 
}
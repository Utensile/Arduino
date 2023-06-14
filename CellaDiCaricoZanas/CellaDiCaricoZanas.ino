#include <HX711.h>
#include <SD.h>

HX711 scale(2, 3);
float calibration = -227;
bool state=false;
int time=0;
String n;

File file;

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  scale.set_scale();
  scale.tare();
  if (!SD.begin(7)) {
    Serial.println("SD sta facendo il gay, invia qualsiasi cosa per ignorare l'errore");   //print error message 
    while(!Serial.available()){}
    Serial.read();
  }
  Serial.println("SD initialized");   
  if(SD.exists("setupCarico.txt")){   //reset SD data
    file = SD.open("setupCarico.txt");
    n=file.readStringUntil('\n');
    file.close();
    SD.remove("setupCarico.txt");
    file = SD.open("setupCarico.txt", FILE_WRITE);
    file.print(n.toInt()+1);
  }
  else{
    file = SD.open("setupCarico.txt", FILE_WRITE);
    file.print("1");
    n="0";
  }
  file.close();
  n="dataCarico" + n+".txt";
  if(SD.exists(n))
    SD.remove(n);
  pinMode(5, OUTPUT);

}

void loop() {
  delay(10);
  scale.set_scale(calibration);
  float units=scale.get_units();
  if(units<0){
    units=0.0f;
  }
  int print = round(units);
  Serial.print(print);
  Serial.println("g");
  file = SD.open(n, FILE_WRITE);
  file.print(print);  file.println(" g");   //write data on SD
  file.close();
  delay(50);
  char temp = Serial.read();
  if(temp=='r'){
    scale.tare();  
  }
  if(temp=='s'){
    scale.tare();  
    SD.remove(n); 
    delay(300);
    digitalWrite(5, HIGH);
    time=millis();
    state=true;
  }
  if(millis()-time>2000 && state){
    digitalWrite(5, LOW);
    state=false;
  }
  }

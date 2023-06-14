#include "Arduino.h"
#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

//#define P 1032.0 // hPa
//#define T 7.2  //C
#define h0 72 //m Altitudine di partenza del razzo
#define ParachutePin 7
#define SD_CS 10

#define SerialRate 9600
#define Limit 10 //Il paracadute si apre se è <Limit> metri sotto l'altezza massima

double P0=0;
double h=0;
double k=0;
double hmax;
bool deploy=false;
String n;

long t0=0;
long t=0;
long pt=0;

SoftwareSerial lora(2, 3);  // e32 TX e32 RX
Adafruit_BMP280 bmp;
File file;

double measure(){
  double h=0;
  for(int i=0; i<10; i++)
      h+=bmp.readAltitude(P0);
    h=h/10-k;
  return h;
}

void setup() {
  pinMode(ParachutePin, OUTPUT);
  Serial.begin(SerialRate);
  lora.begin(SerialRate);
  if (!bmp.begin(0x76)) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
  if (!SD.begin(SD_CS)) {
    Serial.println("SD initialization failed!");
    while (1);
  }
  if(SD.exists("setup.txt")){   //reset SD data
    file = SD.open("setup.txt");
    n=file.readStringUntil('\n');
    file.close();
    SD.remove("setup.txt");
    file = SD.open("setup.txt", FILE_WRITE);
    file.print(n.toInt()+1);
  }
  else{
    file = SD.open("setup.txt", FILE_WRITE);
    file.print("1");
    n="0";
  }
  file.close();
  n="data" + n+".txt";
  if(SD.exists(n))
    SD.remove(n);

  const float P=bmp.readPressure()/100;
  const float T=bmp.readTemperature();

  P0=P*pow((1-((0.0065*h0)/(T+0.0065*h0+273.15))), -5.257); //Trovo P a livello del mare usando P e T e altezza di partenza

  Serial.println("TX: Setup");
  for(int i=0; i<100; i++){
    h+=bmp.readAltitude(P0);
    //delay(20);
    Serial.print("_");
  }
  Serial.println();
  Serial.println("Finished Setup");
  k=h/100;
  hmax=0;
  Serial.print("k=");
  Serial.println(k);
  Serial.print("p=");
  Serial.println(P);
  Serial.print("t=");
  Serial.println(T);
  t0=millis();
}


void loop() {
  if(lora.available()>0){
    int in = lora.parseInt();
    if(in==2){
      digitalWrite(ParachutePin, HIGH); 
      Serial.println("PARACHUTE DEPLOYED Manually");
      file = SD.open(n, FILE_WRITE);
      file.print("PARACHUTE DEPLOYED Manually at ");file.print(millis()-t0); file.println("ms");
      file.print("Maximum Altitude: ");file.print(hmax); file.println(" m");  
      file.close();  
      pt=millis(); 
      deploy=true;
    }
  }
  if(millis()-t>=500){
    h=measure();
    if(h>hmax){
      hmax=h;
      Serial.print("Hmax= ");
      Serial.println(hmax);
    }
    if(hmax-h>Limit &&!deploy){
      digitalWrite(ParachutePin, HIGH);
      Serial.println("PARACHUTE DEPLOYED Automatically");
      file = SD.open(n, FILE_WRITE);
      file.print("PARACHUTE DEPLOYED Automatically at ");file.print(millis()-t0); file.println("ms"); 
      file.print("Maximum Altitude: ");file.print(hmax); file.println(" m");  
      file.close();  
      pt=millis(); 
      deploy=true; 
    }

    lora.println(h);
    file = SD.open(n, FILE_WRITE); 
    file.print("Altezza:");  file.print(h); file.print(" m | t: ");   file.print(millis()-t0); file.println("ms");    
    file.close();
    t=millis();
  }
  if(millis()-pt>=2000 && digitalRead(ParachutePin)==HIGH)
    digitalWrite(ParachutePin, LOW);
}
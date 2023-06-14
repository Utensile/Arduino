#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HX711_ADC.h>
const int HX711_dout = 4; //mcu > HX711 dout pin
const int HX711_sck = 5; //mcu > HX711 sck pin

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);
float calibration_factor = 383.19;
float units=0.00;
int u=0;
float convert[3];
float pv=0;
const String um[]={"g", "Kg", "lb"};
bool newDataReady=false;
bool tar=0;
unsigned long t = 0;
int tartime=0;
int p[2]={7, 8};
bool sp[2]={0, 0};

LiquidCrystal_I2C lcd(0x27, 16, 2);

void center(const char str[], bool a, int k){
  size_t Size = strlen(str);
  lcd.setCursor(floor(8-Size/2)-k, a);
  lcd.print(str);
}

bool puls(int k){
  return digitalRead(p[k-1]);
}

void update(){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Peso: ");
    lcd.print(convert[u]);
    lcd.print(" ");
    lcd.print(um[u]);
    pv=convert[u];
    if(millis()-tartime<1000){
      center("Tarato", 1, 0);
      tar=true;
  }
  else if (tar){
      tar=false;
  }
}


void setup() {
  pinMode(p[0], INPUT);
  pinMode(p[1], INPUT);

  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  center("MonTech Scale", 0, 1);
  LoadCell.begin();
  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    while (1);
  }
  else {
    LoadCell.setCalFactor(393.55); // set calibration value (float)
  }
  
  lcd.setCursor(0,1);
  for(int i=0; i<16; i++){
    lcd.print("_");
    delay(100);
  }
  update();
}



void loop() {
  if (LoadCell.update()){
    newDataReady = true;
  } 
  if (newDataReady) {
    if (millis() > t + 200) {
      units = LoadCell.getData();
      long val;
      val = (long) (units * 10L); // val = 1234
      units = (float) val / 10.0; // x = 1234 / 10.0 = 123.4
      newDataReady = 0;
      t = millis();
    }
  }
  convert[0]=units;
  convert[1]=units/1000;
  convert[2]=units*0.00220462;

  if(convert[u]!=pv)
    update();



  if(millis()-tartime<1000){
      center("Tarato", 1, 0);
      tar=true;
  }
  else if (tar){
      update();
      tar=false;
  }

  int time=millis();
  if(puls(2) and !puls(1)){
    if(!sp[1])
      u=(u+1)%3;
    sp[1]=true;
  }
  else
    sp[1]=false;
    
  if(puls(1) and !puls(2)){
    if(!sp[0]){
      LoadCell.tareNoDelay();  
    }
    sp[0]=true;
  }
  else
    sp[0]=false;

  if (LoadCell.getTareStatus() == true) {
    tartime=millis();
  }

  if(puls(1) and puls(2)){
    lcd.clear();
    center("Attivata", 0, 0);
    center("Calibrazione", 1, 0);
    LoadCell.tareNoDelay();
    delay(2000);
    lcd.clear();
    lcd.print("Appoggiare 100 g");
    lcd.setCursor(0, 1);
    lcd.print("e premere tara");
    while(!puls(1) and !puls(2));
    lcd.clear();
    if(puls(2)){
      center("Calibrazione", 0, 0);
      center("Annullata", 1, 0);
      delay(2000);
      update();
      return;
    }
    center("Calibrazione...", 0, 0);
    lcd.setCursor(0, 1);
    LoadCell.refreshDataSet(); //refresh the dataset to be sure that the known mass is measured correct
    calibration_factor = LoadCell.getNewCalibration(100.0);
    for(int i=0; i<16; i++){
      lcd.print("_");
      delay(100);
    }
    lcd.clear();
    center("Successo!", 0, 0);
    lcd.setCursor(0, 1);
    lcd.print("C.F. = ");
    lcd.print(calibration_factor);
    delay(2000);
    update();
  }

  delay(10);
}
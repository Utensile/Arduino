#include <LiquidCrystal_I2C.h> // include la libreria per l'LCD
#include <Servo.h> // include la libreria per il servomotore

LiquidCrystal_I2C lcd(0x27, 16, 2); // crea un oggetto lcd
Servo myServo; // crea un oggetto servomotore

int servoPin = 8; // il pin del servomotore è l'8
int servoPos = 0; // la posizione corrente del servomotore
int potVal = 0; // il valore corrente del potenziometro

void setup() {
  lcd.init(); // inizializza l'LCD
  lcd.backlight(); // accende la retroilluminazione dell'LCD
  myServo.attach(servoPin); // collega il servomotore al pin specificato
}

void loop() {
  lcd.clear();
  potVal = analogRead(A0); // legge il valore del potenziometro
  servoPos = map(potVal, 0, 1023, 0, 180); // mappa il valore del potenziometro su un intervallo di 0-180 gradi
  myServo.write(servoPos); // imposta la posizione del servomotore in base al valore del potenziometro
  lcd.setCursor(0, 0); // imposta il cursore alla prima posizione della prima riga dell'LCD
  lcd.print("Pot: "); // scrive "Pot:" sull'LCD
  lcd.print(potVal); // scrive il valore del potenziometro sull'LCD
  lcd.setCursor(0, 1); // imposta il cursore alla prima posizione della seconda riga dell'LCD
  lcd.print("Servo: "); // scrive "Servo:" sull'LCD
  lcd.print(servoPos); // scrive la posizione del servomotore sull'LCD
   // attende 100ms prima di eseguire il loop nuovamente
   delay(15);
}

#include <dht11.h>
#include <LiquidCrystal_I2C.h>

#define DHT11PIN 8

dht11 DHT11;
LiquidCrystal_I2C lcd(0x27, 16, 2);   //LCD Object

void   setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loop()
{
  lcd.clear();
  Serial.println("MEASURING...");
  int chk = DHT11.read(DHT11PIN);

  lcd.setCursor(0, 0);
  lcd.print("Hum: ");
  lcd.print((float)DHT11.humidity, 2);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print((float)DHT11.temperature, 2);
  lcd.print(" C");

  delay(1000);

}
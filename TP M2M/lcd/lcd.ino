#include <DHT.h>
#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

#define pot A0

int p = 0;
DHT dht(pot, DHT22);

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.begin(16, 2);
  lcd.setRGB(255, 255, 255);
}

void loop() {
  float humidity = dht.readHumidity();
  float temp = dht.readTemperature();
  p = temp;
  Serial.print("H : ");
  Serial.print(humidity);
  Serial.print("       T : ");
  Serial.println(temp);
  float f = 255*(p-20.0)/(50.0-20.0);
  
  lcd.setRGB(f, 255-f, humidity / 100.0 * 255.0);

  lcd.setCursor(0, 0);
  lcd.print("T : ");
  lcd.setCursor(4, 0);
  lcd.print(temp);
  lcd.setCursor(0, 1);
  lcd.print("H : ");
  lcd.setCursor(4, 1);
  lcd.print(humidity);
  lcd.setCursor(10, 1);
  lcd.print("%");
  
  delay(1000);
}

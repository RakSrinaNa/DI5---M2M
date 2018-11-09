#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "rgb_lcd.h"

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
rgb_lcd lcd;

unsigned long delayTime;

void setup() {
    Serial.begin(9600);
    bme.begin();
    lcd.begin(16, 2);
    lcd.setRGB(255, 255, 255);
    delayTime = 1000;
}


void loop() { 

    float t = bme.readTemperature();
    Serial.print("Temperature = ");
    Serial.print(t);
    Serial.println(" *C");

    float h = bme.readHumidity();
    Serial.print("Humidity = ");
    Serial.print(h);
    Serial.println(" %");

    float p = bme.readPressure() / 100.0F;
    Serial.print("Pressure = ");
    Serial.print(p);
    Serial.println(" hPa");

    float a = bme.readAltitude(SEALEVELPRESSURE_HPA);
    Serial.print("Approx. Altitude = ");
    Serial.print(a);
    Serial.println(" m");

    Serial.println();

    float f = 255*(t-20.0)/(50.0-20.0);
  
    lcd.setRGB(f, 255-f, h / 100.0 * 255.0);

    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.setCursor(2, 0);
    lcd.print(t);
    
    lcd.setCursor(8, 0);
    lcd.print("P:");
    lcd.setCursor(10, 0);
    lcd.print(p);

    lcd.setCursor(0, 1);
    lcd.print("H:");
    lcd.setCursor(2, 1);
    lcd.print(h);
    
    lcd.setCursor(8, 1);
    lcd.print("A:");
    lcd.setCursor(10, 1);
    lcd.print(a);

    delay(delayTime);
}


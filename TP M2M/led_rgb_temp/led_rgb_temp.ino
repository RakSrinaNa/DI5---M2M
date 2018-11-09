#include <ChainableLED.h>
#include <DHT.h>

#define led1 13
#define led2 3
#define pot A0
#define To 500

int p = 0;
ChainableLED led(3, 4, 1);
DHT dht(pot, DHT22);

void setup() {
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(pot, INPUT);
  led.init();
  dht.begin();
}

void loop() {
  float humidity = dht.readHumidity();
  float temp = dht.readTemperature();
  p = temp;
  Serial.print("H : ");
  Serial.print(humidity);
  Serial.print("       T : ");
  Serial.println(temp);
  if(p > To){
    digitalWrite(led1, HIGH);
  }
  else{
    digitalWrite(led1, LOW);
  }
  float f = 255*(p-20.0)/(50.0-20.0);
  
  led.setColorRGB(0, f, 255-f, 0);
  Serial.println(f);
  delay(100);
}

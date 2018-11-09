#include <ChainableLED.h>

#define led1 13
#define led2 3
#define pot 1
#define To 500

int p = 0;
ChainableLED led(3, 4, 1);

void setup() {
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(pot, INPUT);
  led.init();
}

void loop() {
  p = analogRead(pot);
  if(p > To){
    digitalWrite(led1, HIGH);
  }
  else{
    digitalWrite(led1, LOW);
  }
  float f = p/1023.0;
  led2.setColorHSB(0, 0.5, 1, f);
  Serial.println(f);
  delay(100);
}

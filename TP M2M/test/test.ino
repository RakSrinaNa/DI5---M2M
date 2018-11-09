#include <ChainableLED.h>

#define led3 3
#define led2 2
#define led1 1
#define led0 0

void setup() {
  Serial.begin(9600);
  pinMode(led3, OUTPUT);
}

void loop() {
  analogWrite(led3, 100);
  Serial.println("a");
  delay(1000);
  analogWrite(led3, 0);
  Serial.println("e");
  delay(1000);
}

#include <Arduino.h>

#define RED_LED 15
#define BLUE_LED 16

void setup() {
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
}

void loop() {
  digitalWrite(RED_LED, HIGH);
  delay(150);
  digitalWrite(RED_LED, LOW);
  delay(150);

  digitalWrite(RED_LED, HIGH);
  delay(150);
  digitalWrite(RED_LED, LOW);
  delay(300);

  digitalWrite(BLUE_LED, HIGH);
  delay(150);
  digitalWrite(BLUE_LED, LOW);
  delay(150);

  digitalWrite(BLUE_LED, HIGH);
  delay(150);
  digitalWrite(BLUE_LED, LOW);
  delay(300);
}

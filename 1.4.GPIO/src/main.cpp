#include <Arduino.h>

#define LED1 15
#define LED2 16
#define BUTTON 21
#define BOOT_BUTTON 0

int mode = 1;

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(BOOT_BUTTON, INPUT);
}

void loop() {
  if (digitalRead(BUTTON) == LOW) {
    mode = 1;
    delay(50);
  }

  if (digitalRead(BOOT_BUTTON) == LOW) {
    mode = 2;
    delay(50);
  }

  if (mode == 1) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    delay(200);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    delay(200);
  }

  if (mode == 2) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    delay(1000);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    delay(1000);
  }
}

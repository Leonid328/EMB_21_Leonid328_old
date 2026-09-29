#include <Arduino.h>

int led1 = 15;
int led2 = 16;
int led3 = 17;

unsigned long t1 = 0;
unsigned long t2 = 0;
unsigned long t3 = 0;

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  unsigned long now = millis();

  if (now - t1 >= 200) {
    t1 = now;
    digitalWrite(led1, !digitalRead(led1));
    Serial.print(now);
    Serial.print(" LED1 ");
    Serial.println(digitalRead(led1));
  }

  if (now - t2 >= 500) {
    t2 = now;
    digitalWrite(led2, !digitalRead(led2));
    Serial.print(now);
    Serial.print(" LED2 ");
    Serial.println(digitalRead(led2));
  }

  if (now - t3 >= 1000) {
    t3 = now;
    digitalWrite(led3, !digitalRead(led3));
    Serial.print(now);
    Serial.print(" LED3 ");
    Serial.println(digitalRead(led3));
  }
}

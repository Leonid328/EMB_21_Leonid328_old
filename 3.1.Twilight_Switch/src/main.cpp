#include <Arduino.h>

#define ADC_PIN 4
#define RELAY_PIN 5

#define THRESHOLD_DARK 1200
#define THRESHOLD_LIGHT 2500

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
}

void loop() {
  int adc = analogRead(ADC_PIN);
  Serial.print("ADC: ");
  Serial.println(adc);

  if (adc < THRESHOLD_DARK) {
    digitalWrite(RELAY_PIN, HIGH);
  } else if (adc > THRESHOLD_LIGHT) {
    digitalWrite(RELAY_PIN, LOW);
  }

  delay(200);
}

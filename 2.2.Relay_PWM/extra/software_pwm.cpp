#include <Arduino.h>

constexpr uint8_t POT_PIN = 4;
constexpr uint8_t MOTOR_PIN = 5;
constexpr uint16_t ADC_MAX = 4095;
constexpr uint32_t PWM_PERIOD_US = 2000;

void setup() {
  pinMode(MOTOR_PIN, OUTPUT);
  analogReadResolution(12);
}

void loop() {
  int adcValue = analogRead(POT_PIN);
  uint32_t highTime = static_cast<uint32_t>(adcValue) * PWM_PERIOD_US / ADC_MAX;
  uint32_t lowTime = PWM_PERIOD_US - highTime;
  if (highTime > 0) {
    digitalWrite(MOTOR_PIN, HIGH);
    delayMicroseconds(highTime);
  }
  if (lowTime > 0) {
    digitalWrite(MOTOR_PIN, LOW);
    delayMicroseconds(lowTime);
  }
}

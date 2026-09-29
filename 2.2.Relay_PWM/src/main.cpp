#include <Arduino.h>

constexpr uint8_t RELAY_PIN = 5;
constexpr uint8_t CONTACT_PIN = 4;
constexpr uint32_t PAUSE_MS = 1000;
constexpr uint32_t DEBOUNCE_US = 5000;
constexpr uint8_t MEASUREMENTS = 10;

volatile bool contactEvent = false;
volatile uint32_t contactTime = 0;

void IRAM_ATTR contactISR() {
  static uint32_t lastTime = 0;
  uint32_t now = micros();
  if (now - lastTime < DEBOUNCE_US) {
    return;
  }
  lastTime = now;
  contactTime = now;
  contactEvent = true;
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(CONTACT_PIN, INPUT_PULLUP);
  digitalWrite(RELAY_PIN, LOW);
  attachInterrupt(digitalPinToInterrupt(CONTACT_PIN), contactISR, CHANGE);
}

void loop() {
  static bool done = false;
  if (done) {
    return;
  }
  done = true;

  uint64_t totalOn = 0;
  uint64_t totalOff = 0;

  for (uint8_t i = 0; i < MEASUREMENTS; i++) {
    contactEvent = false;
    uint32_t start = micros();
    digitalWrite(RELAY_PIN, HIGH);
    while (!contactEvent) {
    }
    uint32_t onTime = contactTime - start;
    totalOn += onTime;
    Serial.print("ON ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(onTime);
    Serial.println(" us");
    delay(PAUSE_MS);

    contactEvent = false;
    start = micros();
    digitalWrite(RELAY_PIN, LOW);
    while (!contactEvent) {
    }
    uint32_t offTime = contactTime - start;
    totalOff += offTime;
    Serial.print("OFF ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(offTime);
    Serial.println(" us");
    delay(PAUSE_MS);
  }

  float averageOn = static_cast<float>(totalOn) / MEASUREMENTS;
  float averageOff = static_cast<float>(totalOff) / MEASUREMENTS;
  Serial.println();
  Serial.print("Average ON: ");
  Serial.print(averageOn);
  Serial.println(" us");
  Serial.print("Average OFF: ");
  Serial.print(averageOff);
  Serial.println(" us");
}

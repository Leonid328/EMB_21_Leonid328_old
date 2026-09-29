#include <Arduino.h>

#define LDR_PIN 4

void setup() {
  Serial.begin(115200);
  pinMode(LDR_PIN, INPUT);
  analogReadResolution(12);
}

void loop() {
  int rawADC = analogRead(LDR_PIN);
  float calculatedVoltage = ((float)rawADC / 4095.0) * 3.1;
  uint32_t measuredMillivolts = analogReadMillivolts(LDR_PIN);
  float measuredVoltage = measuredMillivolts / 1000.0;
  float errorPercent = 0;

  if (measuredVoltage > 0) {
    errorPercent = abs(calculatedVoltage - measuredVoltage) / measuredVoltage * 100.0;
  }

  Serial.println("--------------------------------");
  Serial.print("RAW ADC: ");
  Serial.println(rawADC);
  Serial.print("Calculated voltage: ");
  Serial.print(calculatedVoltage, 3);
  Serial.println(" V");
  Serial.print("Measured voltage: ");
  Serial.print(measuredVoltage, 3);
  Serial.println(" V");
  Serial.print("Measured millivolts: ");
  Serial.print(measuredMillivolts);
  Serial.println(" mV");
  Serial.print("Error: ");
  Serial.print(errorPercent, 2);
  Serial.println(" %");

  delay(100);
}

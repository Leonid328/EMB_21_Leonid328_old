#include <Arduino.h>

#define RED_PIN 15
#define YELLOW_PIN 16
#define GREEN_PIN 17

#define GREEN_MS 5000
#define GREEN_BLINK_MS 3000
#define BLINK_MS 250
#define YELLOW_MS 2000
#define RED_MS 5000
#define RED_YELLOW_MS 1500

enum State {
  ST_GREEN,
  ST_GREEN_BLINK,
  ST_YELLOW,
  ST_RED,
  ST_RED_YELLOW
};

State state = ST_GREEN;
unsigned long stateStart = 0;
unsigned long blinkLast = 0;
int greenOn = 1;

void lights(int r, int y, int g) {
  digitalWrite(RED_PIN, r);
  digitalWrite(YELLOW_PIN, y);
  digitalWrite(GREEN_PIN, g);
}

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  Serial.begin(115200);
  lights(0, 0, 1);
  stateStart = millis();
  Serial.println("GREEN");
}

void loop() {
  unsigned long now = millis();

  if (state == ST_GREEN) {
    if (now - stateStart >= GREEN_MS) {
      state = ST_GREEN_BLINK;
      stateStart = now;
      blinkLast = now;
      greenOn = 1;
      Serial.println("GREEN BLINK");
    }
  } else if (state == ST_GREEN_BLINK) {
    if (now - blinkLast >= BLINK_MS) {
      blinkLast = now;
      greenOn = !greenOn;
      lights(0, 0, greenOn);
    }
    if (now - stateStart >= GREEN_BLINK_MS) {
      state = ST_YELLOW;
      stateStart = now;
      lights(0, 1, 0);
      Serial.println("YELLOW");
    }
  } else if (state == ST_YELLOW) {
    if (now - stateStart >= YELLOW_MS) {
      state = ST_RED;
      stateStart = now;
      lights(1, 0, 0);
      Serial.println("RED");
    }
  } else if (state == ST_RED) {
    if (now - stateStart >= RED_MS) {
      state = ST_RED_YELLOW;
      stateStart = now;
      lights(1, 1, 0);
      Serial.println("RED+YELLOW");
    }
  } else if (state == ST_RED_YELLOW) {
    if (now - stateStart >= RED_YELLOW_MS) {
      state = ST_GREEN;
      stateStart = now;
      lights(0, 0, 1);
      Serial.println("GREEN");
    }
  }
}

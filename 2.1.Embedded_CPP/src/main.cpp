#include <Arduino.h>

enum class LedState : uint8_t {
  Off,
  On
};

enum class LedMode : uint8_t {
  Blinking,
  AlwaysOn,
  AlwaysOff
};

struct Config {
  static constexpr uint8_t LED_PIN = 5;
  static constexpr uint8_t BUTTON_PIN = 4;
  static constexpr uint32_t BLINK_INTERVAL_MS = 500;
  static constexpr uint32_t DEBOUNCE_MS = 50;
  static constexpr uint32_t SERIAL_BAUD = 115200;
};

class Led {
 public:
  explicit Led(uint8_t pin) : pin_(pin) {}

  void init() {
    pinMode(pin_, OUTPUT);
    set(LedState::Off);
  }

  void set(LedState state) {
    state_ = state;
    digitalWrite(pin_, state == LedState::On ? HIGH : LOW);
  }

  void toggle() {
    set(state_ == LedState::On ? LedState::Off : LedState::On);
  }

 private:
  const uint8_t pin_;
  LedState state_ = LedState::Off;
};

Led led(Config::LED_PIN);
volatile bool buttonPressed = false;
LedMode mode = LedMode::Blinking;

void IRAM_ATTR buttonISR() {
  buttonPressed = true;
}

void handleButton() {
  static uint32_t lastPressTime = 0;
  if (!buttonPressed) {
    return;
  }
  buttonPressed = false;

  const uint32_t now = millis();
  if (now - lastPressTime < Config::DEBOUNCE_MS) {
    return;
  }
  lastPressTime = now;

  switch (mode) {
    case LedMode::Blinking:
      mode = LedMode::AlwaysOn;
      led.set(LedState::On);
      break;
    case LedMode::AlwaysOn:
      mode = LedMode::AlwaysOff;
      led.set(LedState::Off);
      break;
    case LedMode::AlwaysOff:
      mode = LedMode::Blinking;
      break;
  }
}

void updateLed() {
  static uint32_t lastToggleTime = 0;
  if (mode != LedMode::Blinking) {
    return;
  }
  const uint32_t now = millis();
  if (now - lastToggleTime >= Config::BLINK_INTERVAL_MS) {
    lastToggleTime = now;
    led.toggle();
  }
}

void setup() {
  Serial.begin(Config::SERIAL_BAUD);
  led.init();
  pinMode(Config::BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(Config::BUTTON_PIN), buttonISR, FALLING);
}

void loop() {
  handleButton();
  updateLed();
}

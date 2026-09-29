#include <Arduino.h>
#include <esp_timer.h>

#define FAN_PIN 5
#define LED_PIN 2

#define PERIOD_MS 20000
#define ON_MS 5000

static esp_timer_handle_t period_timer;
static esp_timer_handle_t off_timer;
static volatile bool fan_running = false;

void fan_off(void *arg) {
  fan_running = false;
  digitalWrite(FAN_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  Serial.println("fan off");
}

void fan_on(void *arg) {
  if (fan_running) {
    return;
  }
  fan_running = true;
  digitalWrite(FAN_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH);
  Serial.println("fan on");
  esp_timer_start_once(off_timer, (uint64_t)ON_MS * 1000ULL);
}

void setup() {
  Serial.begin(115200);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(FAN_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  esp_timer_create_args_t off_args = {};
  off_args.callback = &fan_off;
  off_args.name = "fan_off";

  esp_timer_create_args_t on_args = {};
  on_args.callback = &fan_on;
  on_args.name = "fan_on";

  esp_timer_create(&off_args, &off_timer);
  esp_timer_create(&on_args, &period_timer);
  fan_on(NULL);
  esp_timer_start_periodic(period_timer, (uint64_t)PERIOD_MS * 1000ULL);
}

void loop() {
}

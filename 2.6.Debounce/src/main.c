#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"

#define LED1 GPIO_NUM_15
#define LED2 GPIO_NUM_16
#define BUTTON GPIO_NUM_21
#define BOOT_BUTTON GPIO_NUM_0
#define DEBOUNCE_US 20000

static volatile int mode = 1;
static volatile int btn_pending = 0;
static volatile int boot_pending = 0;
static esp_timer_handle_t btn_timer;
static esp_timer_handle_t boot_timer;

static void btn_cb(void *arg) {
  if (gpio_get_level(BUTTON) == 0) {
    mode = 1;
  }
  btn_pending = 0;
}

static void boot_cb(void *arg) {
  if (gpio_get_level(BOOT_BUTTON) == 0) {
    mode = 2;
  }
  boot_pending = 0;
}

static void IRAM_ATTR btn_isr(void *arg) {
  if (btn_pending) {
    return;
  }
  btn_pending = 1;
  esp_timer_start_once(btn_timer, DEBOUNCE_US);
}

static void IRAM_ATTR boot_isr(void *arg) {
  if (boot_pending) {
    return;
  }
  boot_pending = 1;
  esp_timer_start_once(boot_timer, DEBOUNCE_US);
}

void app_main(void) {
  gpio_config_t out = {
    .pin_bit_mask = (1ULL << LED1) | (1ULL << LED2),
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE
  };
  gpio_config(&out);

  gpio_config_t btn = {
    .pin_bit_mask = (1ULL << BUTTON),
    .mode = GPIO_MODE_INPUT,
    .pull_up_en = GPIO_PULLUP_ENABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_NEGEDGE
  };
  gpio_config(&btn);

  gpio_config_t boot = {
    .pin_bit_mask = (1ULL << BOOT_BUTTON),
    .mode = GPIO_MODE_INPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_NEGEDGE
  };
  gpio_config(&boot);

  const esp_timer_create_args_t btn_args = {
    .callback = &btn_cb,
    .name = "btn"
  };
  const esp_timer_create_args_t boot_args = {
    .callback = &boot_cb,
    .name = "boot"
  };
  esp_timer_create(&btn_args, &btn_timer);
  esp_timer_create(&boot_args, &boot_timer);

  gpio_install_isr_service(0);
  gpio_isr_handler_add(BUTTON, btn_isr, NULL);
  gpio_isr_handler_add(BOOT_BUTTON, boot_isr, NULL);

  while (1) {
    if (mode == 1) {
      gpio_set_level(LED1, 1);
      gpio_set_level(LED2, 1);
      vTaskDelay(pdMS_TO_TICKS(200));
      gpio_set_level(LED1, 0);
      gpio_set_level(LED2, 0);
      vTaskDelay(pdMS_TO_TICKS(200));
    } else {
      gpio_set_level(LED1, 1);
      gpio_set_level(LED2, 0);
      vTaskDelay(pdMS_TO_TICKS(1000));
      gpio_set_level(LED1, 0);
      gpio_set_level(LED2, 1);
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }
}

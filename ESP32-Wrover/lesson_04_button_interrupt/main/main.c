#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define BUTTON_PIN GPIO_NUM_0
#define LED_PIN    GPIO_NUM_2

volatile bool led_on = false;  // Shared flag toggled by ISR

// Interrupt Service Routine
static void IRAM_ATTR button_isr_handler(void* arg) {
    led_on = !led_on;  // Toggle LED state
}

void app_main(void) {
    // Configure LED pin as output
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    // Configure button pin as input with pull-up and falling-edge interrupt
    gpio_reset_pin(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_pullup_en(BUTTON_PIN);
    gpio_set_intr_type(BUTTON_PIN, GPIO_INTR_NEGEDGE);

    // Install and attach interrupt
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_PIN, button_isr_handler, NULL);

    // Main loop
    while (1) {
        gpio_set_level(LED_PIN, led_on);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
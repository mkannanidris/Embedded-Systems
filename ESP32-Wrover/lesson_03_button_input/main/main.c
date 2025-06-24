#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define BUTTON_PIN GPIO_NUM_0   // Button connected to GPIO 0
#define LED_PIN    GPIO_NUM_2   // On-board LED on GPIO 2

void app_main(void) {
    // Reset and configure pins
    gpio_reset_pin(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);

    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    while (1) {
        int button_state = gpio_get_level(BUTTON_PIN);  // Read the button state

        if (button_state == 1) {
            gpio_set_level(LED_PIN, 1);  // Turn LED ON if button is pressed
        } else {
            gpio_set_level(LED_PIN, 0);  // Turn LED OFF if not pressed
        }

        vTaskDelay(pdMS_TO_TICKS(10));  // Small delay to avoid CPU overload
    }
}
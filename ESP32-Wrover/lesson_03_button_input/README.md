# Lesson 3: Button Input to Control an LED (ESP-IDF)

In this lesson, you’ll learn how to use a digital input (a button) to control a digital output (an LED) using the ESP-IDF framework.

---

## 🧠 Objective

- Read input from a push button using `gpio_get_level()`
- Control an output (LED) in response to that input
- Understand GPIO configuration for input and output
- Introduce polling with `vTaskDelay()` to prevent CPU overload

---

## 🔌 Hardware Setup

| Component | GPIO |
|----------|------|
| Button   | 0    |
| LED      | 2    |

- Connect one side of the button to **GPIO 0** and the other side to **GND**
- Use the on-board LED connected to **GPIO 2**

> ⚠️ No external pull-up needed on GPIO 0 — it has an internal pull-up on most ESP32 boards.

---

## 📄 Code

```c
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
```
## 📝 Code Concepts

- `gpio_reset_pin(pin)`  
  Resets the specified GPIO pin to its default state. This ensures a clean setup before configuration.

- `gpio_set_direction(pin, mode)`  
  Sets the GPIO pin to behave either as an input (`GPIO_MODE_INPUT`) or output (`GPIO_MODE_OUTPUT`).

- `gpio_get_level(pin)`  
  Reads the current logic level of a GPIO pin. Returns `1` (HIGH) if voltage is detected, or `0` (LOW) if grounded.

- `gpio_set_level(pin, value)`  
  Sets the output level of a GPIO pin. Use `1` to set HIGH (turn LED ON), or `0` for LOW (turn LED OFF).

- `vTaskDelay(pdMS_TO_TICKS(ms))`  
  Introduces a delay in the task loop without blocking other system tasks. Converts milliseconds to FreeRTOS ticks.
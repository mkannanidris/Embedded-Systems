# Lesson 4: Button Interrupt to Toggle LED

In this lesson, we explore **hardware interrupts** on the ESP32 using ESP-IDF. Instead of checking the button state in a loop (polling), we let the ESP32 **react instantly** when the button is pressed using an **Interrupt Service Routine (ISR)**.

---

## 🧠 Objective

- Learn how to configure a GPIO pin to trigger an interrupt.
- Understand how to use an Interrupt Service Routine (ISR) to toggle a variable.
- Use the ISR-triggered flag to update the LED state in the main loop.

---

## 📄 Code

`main.c`

```c
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
```
## 🧩 Code Concepts

- `gpio_reset_pin(pin);`  
  Resets the given GPIO pin to its default state before configuring it.

- `gpio_set_direction(pin, mode);`  
  Sets the pin as either an input or output. In this lesson, we set:
  - The LED pin (`GPIO_NUM_2`) as `GPIO_MODE_OUTPUT`
  - The button pin (`GPIO_NUM_0`) as `GPIO_MODE_INPUT`

- `gpio_pullup_en(pin);`  
  Enables the internal pull-up resistor on the button pin. This ensures the pin reads HIGH when the button is not pressed (avoiding a floating state).

- `gpio_set_intr_type(pin, type);`  
  Defines the interrupt trigger. `GPIO_INTR_NEGEDGE` means the interrupt is triggered when the signal goes from HIGH to LOW (button press).

- `gpio_install_isr_service(0);`  
  Initializes the interrupt service system. Must be called before registering ISR functions.

- `gpio_isr_handler_add(pin, handler, arg);`  
  Registers your interrupt service routine (`handler`) to be called when the specified pin is triggered.

- `static void IRAM_ATTR button_isr_handler(void* arg);`  
  This is the actual interrupt handler that runs when the button is pressed.  
  - `IRAM_ATTR` places the function in fast internal RAM (required for ISR functions).
  - The handler toggles a `volatile` flag instead of performing slow operations like I/O.

- `volatile bool led_on;`  
  A shared flag used between the ISR and main loop. Marked `volatile` to prevent compiler optimizations that may skip re-reading its value.

- `gpio_set_level(LED_PIN, led_on);`  
  Updates the LED pin based on the value of the `led_on` flag.

- `vTaskDelay(pdMS_TO_TICKS(10));`  
  Delays the loop for 10 milliseconds to avoid hogging the CPU. This is a FreeRTOS-safe delay.
# Lesson 2: Displaying Numbers on a 7-Segment Display (ESP-IDF)

In this lesson, you will learn how to control a 7-segment display using GPIO pins on the ESP32. You’ll display numbers from 0 to 9, then flash all segments 3 times.

---

## 🧠 Objective

- Learn how to control multiple GPIOs in parallel.
- Display numeric values (0–9) on a 7-segment display.
- Practice using arrays and loops to manage repetitive GPIO operations.
- Use delays with FreeRTOS (`vTaskDelay`).

---

## 🔌 Hardware Connections

Connect the 7 GPIO pins from your ESP32 to the **segments A–G** of a common cathode 7-segment display. Match the segmentPins array to your wiring:

| Segment | GPIO |
|---------|------|
| A       | 1    |
| B       | 2    |
| C       | 3    |
| D       | 4    |
| E       | 5    |
| F       | 6    |
| G       | 7    |

> ⚠️ Adjust `segmentPins[]` in the code to match your physical connections.

---

## 📄 Code Summary

The `app_main()` function:
- Sets GPIOs for each segment as outputs.
- Loops through numbers 0 to 9, displaying each digit for 1 second.
- Blinks all segments 3 times after counting up.

---

## 📁 File: `main.c`

```c
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define NUM_SEGMENTS 7

int segmentPins[NUM_SEGMENTS] = {GPIO_NUM_1, GPIO_NUM_2, GPIO_NUM_3, GPIO_NUM_4, GPIO_NUM_5, GPIO_NUM_6, GPIO_NUM_7};

const int commonSegments[10][NUM_SEGMENTS] = {
    {1, 1, 1, 0, 1, 1, 1},  // 0
    {0, 1, 0, 0, 0, 1, 0},  // 1
    {1, 1, 0, 1, 1, 0, 1},  // 2
    {1, 1, 0, 1, 0, 1, 1},  // 3
    {0, 1, 1, 1, 0, 1, 0},  // 4
    {1, 0, 1, 1, 0, 1, 1},  // 5
    {1, 0, 1, 1, 1, 1, 1},  // 6
    {1, 1, 1, 0, 0, 1, 0},  // 7
    {1, 1, 1, 1, 1, 1, 1},  // 8
    {1, 1, 1, 1, 0, 1, 1}   // 9
};

void display_digit(int digit) {
    for (int i = 0; i < NUM_SEGMENTS; i++) {
        gpio_set_level(segmentPins[i], commonSegments[digit][i]);
    }
}

void blink_all_segments_x3() {
    const int delay_ms = 200;
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < NUM_SEGMENTS; i++) {
            gpio_set_level(segmentPins[i], 1);
        }
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
        for (int i = 0; i < NUM_SEGMENTS; i++) {
            gpio_set_level(segmentPins[i], 0);
        }
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }
}

void app_main(void) {
    for (int i = 0; i < NUM_SEGMENTS; i++) {
        gpio_reset_pin(segmentPins[i]);
        gpio_set_direction(segmentPins[i], GPIO_MODE_OUTPUT);
    }

    while (1) {
        for (int digit = 0; digit <= 9; digit++) {
            display_digit(digit);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }

        blink_all_segments_x3();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

## 📝 Code Concepts

- `segmentPins[]`  
  An array of 7 GPIO numbers connected to the A–G segments of the 7-segment display. This allows us to control each segment individually using a loop.

- `commonSegments[][]`  
  A 2D array that defines the ON/OFF pattern for each digit (0–9). Each row corresponds to one digit, and each column represents a segment (A to G).

- `display_digit(int digit)`  
  A function that loops through the `commonSegments` array for the given digit and updates the GPIO pins accordingly.

- `gpio_set_level(pin, value)`  
  Sets the output voltage on a GPIO pin to either HIGH (`1`) or LOW (`0`). Used to turn each segment ON or OFF.

- `gpio_set_direction(pin, GPIO_MODE_OUTPUT)`  
  Configures a GPIO pin as an output. All 7 segment pins must be set this way to control the display.

- `gpio_reset_pin(pin)`  
  Resets the GPIO to a known initial state before configuration. Required in ESP-IDF v5.0+.

- `vTaskDelay(pdMS_TO_TICKS(ms))`  
  Delays the task for a given number of milliseconds. Converts time in ms to FreeRTOS ticks.
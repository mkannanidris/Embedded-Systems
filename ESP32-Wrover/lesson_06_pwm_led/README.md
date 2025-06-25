# Lesson 6: PWM and Dimming an LED with ESP32 (ESP-IDF)

In this lesson, we learn how to use **PWM (Pulse Width Modulation)** to control the brightness of an LED using the ESP32’s built-in LEDC (LED Control) driver.

## 🧠 Objective

- Understand how PWM works to simulate analog output
- Learn to configure the LEDC driver in ESP-IDF
- Use `ledc_set_duty()` and `ledc_update_duty()` to control brightness
- Smoothly fade an LED in and out

## 🔌 Circuit

- Connect an LED (with a 330Ω resistor in series) to **GPIO 2**
- Or just use the on-board LED if your dev board has one on GPIO 2

## 📄 Code

`main.c`
```c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"

#define LED_PIN 2  // On-board LED

void app_main() {
    // Configure PWM timer
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 1000,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    // Configure PWM channel
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = LED_PIN,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&ledc_channel);

    while (1) {
        // Fade LED brightness up
        for (int duty = 0; duty <= 255; duty += 5) {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
            vTaskDelay(pdMS_TO_TICKS(20));
        }

        // Fade LED brightness down
        for (int duty = 255; duty >= 0; duty -= 5) {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
            vTaskDelay(pdMS_TO_TICKS(20));
        }
    }
}
```
## 💡 Code Concepts

- **PWM (Pulse Width Modulation):**
  Rapidly toggles a pin between HIGH and LOW to simulate analog voltage levels. Used to control brightness, speed, etc.

- **Duty Cycle:**
  Percentage of time the signal is HIGH in one PWM period. More duty = more brightness.

- **LEDC (LED Control) Driver:**
  ESP32 peripheral for generating PWM signals with high precision and low CPU load.

- **`ledc_timer_config_t`:**
  Structure to configure a timer for PWM — includes frequency and resolution.

- **`ledc_channel_config_t`:**
  Structure to define a channel that outputs PWM on a specific GPIO pin.

- **`ledc_set_duty()`:**
  Sets the PWM duty cycle for a given channel.

- **`ledc_update_duty()`:**
  Applies the updated duty cycle value to the hardware pin.

- **`vTaskDelay()`:**
  ESP-IDF FreeRTOS function to pause execution for a specified time in milliseconds.
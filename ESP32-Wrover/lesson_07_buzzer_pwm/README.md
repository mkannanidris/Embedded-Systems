# Lesson 7: PWM Buzzer Tone with ESP32

In this lesson, we use **Pulse Width Modulation (PWM)** to generate tones from an active buzzer using the ESP32's LEDC driver.

## 🧠 Objective

- Learn to generate sound using PWM
- Understand how frequency and duty cycle affect tone
- Use the LEDC driver in ESP-IDF to control a buzzer

## 🔌 Circuit

- Connect a **5V active buzzer**:
  - **Positive ( + )** → **GPIO 4**
  - **Negative ( – )** → **GND**

> ⚠️ You must use an **active buzzer** (with built-in oscillator). A passive buzzer will not produce tone unless you generate proper waveforms.

## 📄 Code

```c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"

#define BUZZER_GPIO 4

void app_main(void) {
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .freq_hz = 1000,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .gpio_num = BUZZER_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 512,
        .hpoint = 0,
        .intr_type = LEDC_INTR_DISABLE
    };
    ledc_channel_config(&ledc_channel);

    while (1) {
        for (int freq = 500; freq <= 2000; freq += 250) {
            ledc_set_freq(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0, freq);
            vTaskDelay(pdMS_TO_TICKS(500));
        }

        ledc_stop(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));

        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 512);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    }
}
```
## 📘 Code Concepts

- **PWM (Pulse Width Modulation):**  
  A technique to simulate analog signals using digital output by switching the signal ON and OFF rapidly with a certain duty cycle and frequency.

- **Duty Cycle:**  
  The percentage of time the signal is HIGH during one PWM cycle. A 50% duty cycle means the signal is ON for half the time and OFF for the other half.

- **Frequency:**  
  Determines the tone of the sound when driving a buzzer. Higher frequency = higher pitch.

- **LEDC Driver:**  
  The ESP-IDF LED Control (LEDC) driver allows you to generate PWM signals with precise control over duty cycle and frequency.

- **ledc_timer_config_t:**  
  Configures the timer that defines frequency and resolution of the PWM signal.

- **ledc_channel_config_t:**  
  Binds a GPIO pin to a PWM signal using a timer.

- **ledc_set_freq():**  
  Changes the frequency of the PWM signal dynamically.

- **ledc_stop():**  
  Stops the PWM signal and sets the pin to LOW.

- **vTaskDelay():**  
  A FreeRTOS function that blocks the task for a specified number of ticks, used here to wait between tones.
# Lesson 9: 🧵 Intro to FreeRTOS with Real Tasks

In this lesson, we introduce **FreeRTOS** multitasking on the ESP32 using three separate tasks that demonstrate interrupt handling, PWM control, and UART communication — all running in parallel.

---

## 🎯 Objectives

- Understand the concept of tasks in FreeRTOS
- Create and run multiple concurrent tasks on the ESP32
- Use interrupts to control task behavior
- Implement UART communication and LED PWM simultaneously

---

## 🔌 Circuit

| Component        | ESP32 Pin       |
|------------------|-----------------|
| Push Button      | GPIO 0          |
| LED              | GPIO 2          |
| UART TX (to USB-TTL RX) | GPIO 4  |
| UART RX (to USB-TTL TX) | GPIO 5  |

Make sure to use a USB-to-TTL adapter connected to GPIO 4 and 5 for UART serial communication. You can monitor the output using a serial terminal like CoolTerm or `screen` on macOS.

---

## 🧾 Code

`main.c`

```c
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "driver/uart.h"
#include "esp_log.h"

#define BUTTON_PIN GPIO_NUM_0
#define LED_PIN GPIO_NUM_2
#define TXD_PIN GPIO_NUM_4
#define RXD_PIN GPIO_NUM_5
#define UART_PORT UART_NUM_1

volatile bool led_on = false;

static void IRAM_ATTR button_isr_handler(void *arg) {
    led_on = !led_on;
}

void button_task(void *pvParameter) {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << BUTTON_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .intr_type = GPIO_INTR_NEGEDGE
    };
    gpio_config(&io_conf);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_PIN, button_isr_handler, NULL);

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void pwm_task(void *pvParameter) {
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .freq_hz = 5000,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .channel = LEDC_CHANNEL_0,
        .duty = 0,
        .gpio_num = LED_PIN,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .hpoint = 0,
        .timer_sel = LEDC_TIMER_0
    };
    ledc_channel_config(&ledc_channel);

    while (1) {
        if (led_on) {
            for (int duty = 0; duty < 256; duty += 10) {
                ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty);
                ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
                vTaskDelay(pdMS_TO_TICKS(20));
            }
        } else {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void uart_task(void *pvParameter) {
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_PORT, &uart_config);
    uart_set_pin(UART_PORT, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_PORT, 1024 * 2, 0, 0, NULL, 0);

    while (1) {
        const char *msg = led_on ? "LED is ON\n" : "LED is BLINKING\n";
        uart_write_bytes(UART_PORT, msg, strlen(msg));
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void app_main() {
    xTaskCreate(button_task, "Button Task", 2048, NULL, 10, NULL);
    xTaskCreate(pwm_task, "PWM Task", 2048, NULL, 10, NULL);
    xTaskCreate(uart_task, "UART Task", 2048, NULL, 10, NULL);
}
```
## 🧠 Code Concepts

- **FreeRTOS Tasks**:  
  Each functionality (button, LED PWM, UART) is implemented as a separate FreeRTOS task using `xTaskCreate()`, allowing concurrent execution.

- **Interrupt Service Routine (ISR)**:  
  The button press is handled via an interrupt attached using `gpio_isr_handler_add()`, which toggles a global `led_on` flag.

- **PWM (Pulse Width Modulation)**:  
  The LED is controlled using the LEDC driver to gradually increase brightness.  
  Key functions: `ledc_timer_config()`, `ledc_channel_config()`, `ledc_set_duty()`, `ledc_update_duty()`.

- **UART Communication**:  
  A UART task sends a message indicating LED state every 2 seconds.  
  Key functions: `uart_param_config()`, `uart_set_pin()`, `uart_driver_install()`, `uart_write_bytes()`.

- **Shared Variable Between Tasks/ISR**:  
  The global variable `led_on` is toggled by the ISR and read by both the PWM and UART tasks.

- **GPIO Configuration**:  
  The button GPIO is configured with pull-up and edge-detection for interrupt-based input.
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "driver/uart.h"
#include "esp_log.h"

#define BUTTON_PIN GPIO_NUM_0     // Button connected to GPIO 0
#define LED_PIN GPIO_NUM_2        // LED connected to GPIO 2
#define TXD_PIN GPIO_NUM_4        // UART TX pin
#define RXD_PIN GPIO_NUM_5        // UART RX pin
#define UART_PORT UART_NUM_1      // Using UART1

volatile bool led_on = false;     // Shared flag for LED state

// Interrupt handler for button press
static void IRAM_ATTR button_isr_handler(void *arg) {
    led_on = !led_on; // Toggle LED state
}

// Task 1: Configure button GPIO and set up interrupt
void button_task(void *pvParameter) {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << BUTTON_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .intr_type = GPIO_INTR_NEGEDGE  // Interrupt on falling edge
    };
    gpio_config(&io_conf);

    gpio_install_isr_service(0);  // Default ISR service
    gpio_isr_handler_add(BUTTON_PIN, button_isr_handler, NULL);  // Add ISR

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));  // Nothing else needed here
    }
}

// Task 2: PWM control of LED based on button flag
void pwm_task(void *pvParameter) {
    // Configure LEDC timer
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .freq_hz = 5000,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    // Configure LEDC channel
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

// Task 3: UART communication of LED state
void uart_task(void *pvParameter) {
    // UART configuration
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

// Main application
void app_main() {
    xTaskCreate(button_task, "Button Task", 2048, NULL, 10, NULL);
    xTaskCreate(pwm_task, "PWM Task", 2048, NULL, 10, NULL);
    xTaskCreate(uart_task, "UART Task", 2048, NULL, 10, NULL);
}
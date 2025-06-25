#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"

#define BUZZER_GPIO 4  // Connect buzzer to GPIO 4

void app_main(void) {
    // Configure PWM timer
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .timer_num        = LEDC_TIMER_0,
        .duty_resolution  = LEDC_TIMER_10_BIT,  // Higher resolution for better tone control
        .freq_hz          = 1000,               // Initial frequency (1 kHz)
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    // Configure PWM channel
    ledc_channel_config_t ledc_channel = {
        .gpio_num       = BUZZER_GPIO,
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = LEDC_CHANNEL_0,
        .timer_sel      = LEDC_TIMER_0,
        .duty           = 512, // 50% duty (max 1023 for 10-bit)
        .hpoint         = 0,
        .intr_type      = LEDC_INTR_DISABLE
    };
    ledc_channel_config(&ledc_channel);

    // Play tones of increasing frequency
    while (1) {
        for (int freq = 500; freq <= 2000; freq += 250) {
            ledc_set_freq(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0, freq);
            printf("Playing tone: %d Hz\n", freq);
            vTaskDelay(pdMS_TO_TICKS(500));
        }

        // Silence buzzer for a short pause
        ledc_stop(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));

        // Restart buzzer
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 512);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    }
}
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"

#define ADC_PIN ADC1_CHANNEL_6  // GPIO34

void app_main() {
    // Configure ADC width and attenuation
    adc1_config_width(ADC_WIDTH_BIT_12);  // 12-bit resolution (0–4095)
    adc1_config_channel_atten(ADC_PIN, ADC_ATTEN_DB_11);  // 0–3.3V range

    while (1) {
        int adc_reading = adc1_get_raw(ADC_PIN);  // Read raw analog value
        printf("ADC Reading: %d\n", adc_reading);  // Print to terminal

        vTaskDelay(pdMS_TO_TICKS(500));  // Wait 500ms before next read
    }
}
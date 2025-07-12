#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "dht.h"

#define DHT_GPIO GPIO_NUM_4
#define DHT_TYPE DHT_TYPE_DHT11

void app_main(void)
{
    float temperature = 0.0f, humidity = 0.0f;

    while (1) {
        esp_err_t result = dht_read_float_data(DHT_TYPE, DHT_GPIO, &humidity, &temperature);
        if (result == ESP_OK) {
            printf("Humidity: %.1f %%\n", humidity);
            printf("Temperature: %.1f °C\n", temperature);
        } else {
            printf("Failed to read from DHT sensor: %s\n", esp_err_to_name(result));
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
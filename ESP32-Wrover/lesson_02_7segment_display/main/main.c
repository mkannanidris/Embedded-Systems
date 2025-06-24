#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define NUM_SEGMENTS 7

// GPIO pins for the 7 segments (adjust as per your wiring)
int segmentPins[NUM_SEGMENTS] = {GPIO_NUM_1, GPIO_NUM_2, GPIO_NUM_3, GPIO_NUM_4, GPIO_NUM_5, GPIO_NUM_6, GPIO_NUM_7};

// Segment patterns for digits 0–9
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
    // Configure each segment pin as output
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
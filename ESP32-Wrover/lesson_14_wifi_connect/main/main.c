#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "esp_sntp.h"

#define WIFI_SSID "Your SSID"
#define WIFI_PASS "Your Password"

static const char *TAG = "wifi";
static bool wifi_connected = false;  // Flag to indicate Wi-Fi connection status

// Callback: called automatically when the ESP32 gets an IP address
static void on_wifi_connected(void* arg, esp_event_base_t event_base,
                              int32_t event_id, void* event_data)
{
    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Connected! IP Address: " IPSTR, IP2STR(&event->ip_info.ip));
        wifi_connected = true;
    }
}

// Function to initialize Wi-Fi and attempt connection
void wifi_connect()
{
    // Initialize networking stack and default event loop
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();  // Create default Wi-Fi station

    // Set Wi-Fi init configuration
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    vTaskDelay(pdMS_TO_TICKS(500));  // Allow system to settle
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Set SSID and password
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };

    // Register event handler to track IP assignment
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &on_wifi_connected, NULL));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_connect());

    ESP_LOGI(TAG, "Connecting to %s...", WIFI_SSID);
}

// Perform a simple HTTP GET request to worldtimeapi.org and print the result
// Task to fetch time
void ntp_time_task(void *pvParameters) {
    // Log the initialization of SNTP (Simple Network Time Protocol)
    ESP_LOGI("ntp", "Initializing SNTP");

    // Set SNTP to polling mode (will periodically query the server)
    sntp_setoperatingmode(SNTP_OPMODE_POLL);

    // Set the NTP server to use for time synchronization
    sntp_setservername(0, "pool.ntp.org");

    // Initialize the SNTP service
    sntp_init();

    time_t now = 0;
    struct tm timeinfo = {0};
    int retry = 0;
    const int max_retries = 10;

    // Wait until the system time is updated from NTP or retries exceed the limit
    while (timeinfo.tm_year < (2020 - 1900) && ++retry < max_retries) {
        ESP_LOGI("ntp", "Waiting for NTP time sync... (%d)", retry);
        vTaskDelay(pdMS_TO_TICKS(2000));  // Wait 2 seconds between attempts

        time(&now);                            // Get the current time (UTC)
        localtime_r(&now, &timeinfo);          // Convert time to local time struct
    }

    // If we still don't have a valid time, log an error and exit the task
    if (timeinfo.tm_year < (2020 - 1900)) {
        ESP_LOGE("ntp", "Failed to sync time over NTP");
        vTaskDelete(NULL);  // Delete the current task
        return;
    }

    // Set timezone to Eastern Time with Daylight Saving Time rules
    const char *tz = "EST5EDT,M3.2.0/2,M11.1.0/2";
    setenv("TZ", tz, 1);  // Update TZ environment variable
    tzset();              // Apply the new timezone
    ESP_LOGI("ntp", "Timezone set to: %s", tz);

    // Infinite loop: display local time every second
    while (1) {
        time(&now);                            // Get current time
        localtime_r(&now, &timeinfo);          // Convert to local time

        char buffer[64];
        strftime(buffer, sizeof(buffer), "%A, %B %d %Y %H:%M:%S %Z (%z)", &timeinfo);
        ESP_LOGI("ntp", "Current time: %s", buffer);  // Log the formatted local time

        vTaskDelay(pdMS_TO_TICKS(5000));  // Wait 5 seconds before repeating
    }
}

// Main application entry point
void app_main(void)
{
    // Initialize non-volatile storage used by Wi-Fi
    ESP_ERROR_CHECK(nvs_flash_init());

    // Start Wi-Fi connection
    wifi_connect();

    // Wait until Wi-Fi is connected
    while (!wifi_connected) {
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    // Fetch date and time task
    xTaskCreate(ntp_time_task, "ntp_time_task", 4096, NULL, 5, NULL);
}

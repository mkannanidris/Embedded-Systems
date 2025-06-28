# Lesson 14: 🌐 Wi-Fi Basics: Connecting to a Network and Get Current Time Over Wi-Fi (NTP)

In this lesson, we connect the ESP32 to Wi-Fi and use the **SNTP protocol** to retrieve the current time from the internet. The time is synced using `pool.ntp.org` and printed every 5 seconds in Eastern Time (with daylight saving support).

---

## 🎯 Objectives

- Connect the ESP32 to a Wi-Fi network using the `esp_wifi` library
- Synchronize system time using SNTP (`esp_sntp`)
- Configure and apply a timezone with daylight saving time (DST)
- Display the current local time every 5 seconds

---

## 🔌 Circuit

| Component | ESP32 Pin |
|----------|------------|
| N/A      | Wi-Fi only (no external wiring required) |

This lesson is purely software-based and requires no additional hardware beyond a working ESP32 development board and internet connection.

---

## 🧾 Code

`main.c`

```c
#define WIFI_SSID "Your SSID"
#define WIFI_PASS "Your Password"

static const char *TAG = "wifi";
static bool wifi_connected = false;  // Flag to indicate Wi-Fi connection status

// Callback: called when the ESP32 gets an IP address
static void on_wifi_connected(void* arg, esp_event_base_t event_base,
                              int32_t event_id, void* event_data)
{
    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Connected! IP Address: " IPSTR, IP2STR(&event->ip_info.ip));
        wifi_connected = true;
    }
}

// Initialize Wi-Fi and attempt connection
void wifi_connect()
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    vTaskDelay(pdMS_TO_TICKS(500));
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };

    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &on_wifi_connected, NULL));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_connect());

    ESP_LOGI(TAG, "Connecting to %s...", WIFI_SSID);
}

// NTP time task: sync time and print it every 5 seconds
void ntp_time_task(void *pvParameters) {
    ESP_LOGI("ntp", "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();

    time_t now = 0;
    struct tm timeinfo = {0};
    int retry = 0;
    const int max_retries = 10;

    while (timeinfo.tm_year < (2020 - 1900) && ++retry < max_retries) {
        ESP_LOGI("ntp", "Waiting for NTP time sync... (%d)", retry);
        vTaskDelay(pdMS_TO_TICKS(2000));
        time(&now);
        localtime_r(&now, &timeinfo);
    }

    if (timeinfo.tm_year < (2020 - 1900)) {
        ESP_LOGE("ntp", "Failed to sync time over NTP");
        vTaskDelete(NULL);
        return;
    }

    const char *tz = "EST5EDT,M3.2.0/2,M11.1.0/2";
    setenv("TZ", tz, 1);
    tzset();
    ESP_LOGI("ntp", "Timezone set to: %s", tz);

    while (1) {
        time(&now);
        localtime_r(&now, &timeinfo);

        char buffer[64];
        strftime(buffer, sizeof(buffer), "%A, %B %d %Y %H:%M:%S %Z (%z)", &timeinfo);
        ESP_LOGI("ntp", "Current time: %s", buffer);

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

// Main application entry point
void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());

    wifi_connect();

    while (!wifi_connected) {
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    xTaskCreate(ntp_time_task, "ntp_time_task", 4096, NULL, 5, NULL);
}
```
## 🧠 Code Concepts

- **Wi-Fi Initialization and Connection**  
  The ESP32 connects to a Wi-Fi network using `esp_wifi` APIs. It creates a station (STA) interface and uses event callbacks to detect when the device obtains an IP address.

- **Event Handling**  
  An event handler is registered to listen for `IP_EVENT_STA_GOT_IP`, which confirms successful connection and stores the IP address for logging.

- **SNTP Time Synchronization**  
  SNTP (Simple Network Time Protocol) is initialized using `sntp_init()`. The ESP32 polls the NTP server `pool.ntp.org` until it retrieves a valid timestamp.

- **Retry Mechanism**  
  A loop checks if the system time has been updated (by checking `tm_year`) and retries up to 10 times, waiting 2 seconds between each attempt.

- **Timezone Configuration**  
  After syncing UTC time, the code sets the timezone using a POSIX string:  
  `"EST5EDT,M3.2.0/2,M11.1.0/2"`  
  This sets Eastern Standard Time with daylight saving time starting on the 2nd Sunday of March and ending on the 1st Sunday of November.

- **Time Formatting and Output**  
  The current local time is retrieved every 5 seconds using `localtime_r()` and formatted with `strftime()` into a human-readable string with timezone info.

- **FreeRTOS Task**  
  The SNTP sync and periodic time printing run inside a FreeRTOS task (`ntp_time_task`) that is launched after Wi-Fi is successfully connected.
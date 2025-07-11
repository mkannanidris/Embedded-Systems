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

#include "esp_http_server.h"
#include "driver/gpio.h"

#define WIFI_SSID "Your SSID"
#define WIFI_PASS "Your Password"
#define LED_GPIO GPIO_NUM_2

static const char *TAG = "wifi";
static bool wifi_connected = false;
static bool led_on = false;

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

// HTTP handler: toggles LED
esp_err_t toggle_led_handler(httpd_req_t *req)
{
    led_on = !led_on;
    gpio_set_level(LED_GPIO, led_on ? 0 : 1);  // active-low
    httpd_resp_sendstr(req, led_on ? "On" : "Off");
    return ESP_OK;
}

// HTTP handler: serves main page
esp_err_t main_page_handler(httpd_req_t *req)
{
    char html[512];
    snprintf(html, sizeof(html),
        "<!DOCTYPE html><html><head><title>ESP32 Web Server</title>"
        "<script>"
        "function toggleLED() {"
        "  fetch('/toggle').then(r => r.text()).then(state => {"
        "    document.getElementById('led-state').innerText = 'LED is ' + state;"
        "  });"
        "}"
        "</script></head><body>"
        "<h2>ESP32 Web Server</h2>"
        "<p id='led-state'>LED is %s</p>"
        "<button onclick='toggleLED()'>Toggle LED</button>"
        "</body></html>",
        led_on ? "On" : "Off"
    );

    httpd_resp_send(req, html, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// Starts HTTP server
httpd_handle_t start_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;
    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t root = {
            .uri      = "/",
            .method   = HTTP_GET,
            .handler  = main_page_handler
        };
        httpd_uri_t toggle = {
            .uri      = "/toggle",
            .method   = HTTP_GET,
            .handler  = toggle_led_handler
        };
        httpd_register_uri_handler(server, &root);
        httpd_register_uri_handler(server, &toggle);
    }
    return server;
}

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());

    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_GPIO, 0);  // Start OFF

    wifi_connect();

    while (!wifi_connected) {
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    start_webserver();
}
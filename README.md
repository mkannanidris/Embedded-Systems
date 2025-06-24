# 📘 ESP32 Embedded Systems One-Spot Learning Hub

Welcome to the **ESP32 Embedded Systems One-Spot Learning Hub** — a structured, hands-on learning center for mastering embedded systems development using the ESP32 microcontroller.

Each lesson builds on the last, starting with basic GPIO and growing into advanced systems with Wi-Fi, cloud communication, real-time multitasking, and low-power design. This series is ideal for students, engineers, and hobbyists alike.

---

## 📚 Complete Progressive Lesson Index (Tentative)

| Lesson # | Title | Concepts Introduced |
|----------|-------|---------------------|
| 1 | 🔴 Blinking an LED | `gpio_set_direction()`, `gpio_set_level()`, basic GPIO output |
| 2 | 🔢 Driving a 7-Segment Display | GPIO arrays, mapping digits to segments, `gpio_set_level()` loops |
| 3 | 🔘 Button Input to Control LED | `gpio_set_direction(INPUT)`, `gpio_get_level()`, GPIO input handling |
| 4 | 🧲 Button Interrupt to Toggle LED | `gpio_set_intr_type()`, `gpio_isr_handler_add()`, `volatile`, ISRs |
| 5 | 🎛️ Potentiometer and Analog Read | `adc1_config_width()`, `adc1_config_channel_atten()`, `adc1_get_raw()` |
| 6 | 💡 PWM and Dimming an LED | `ledc_timer_config()`, `ledc_channel_config()`, `ledc_set_duty()` |
| 7 | 🔊 Controlling a Buzzer | PWM tones with `ledc`, sound frequency generation |
| 8 | 📡 UART Serial Communication | `uart_driver_install()`, `uart_write_bytes()`, `uart_read_bytes()` |
| 9 | 🧵 Intro to FreeRTOS on ESP32 | `xTaskCreate()`, task scheduling, `vTaskDelay()`, multitasking |
| 10 | 🌡️ Reading a Temperature Sensor | ADC or digital GPIO input for LM35/DHT11 with timing |
| 11 | 🧰 Using I2C: OLED Display | `i2c_param_config()`, `i2c_master_write()`, OLED init + draw |
| 12 | 🔌 Using SPI: External Devices | `spi_bus_initialize()`, `spi_device_transmit()`, SPI config |
| 13 | 🕹️ Multi-Input System Integration | Buttons, sensors, display; logic design and state machines |
| 14 | 📶 Wi-Fi Basics: Connecting to a Network | `esp_wifi_init()`, `esp_wifi_connect()`, TCP/IP stack basics |
| 15 | 🌐 Hosting a Web Page on ESP32 | `esp_http_server.h`, serving HTML, controlling GPIO via HTTP |
| 16 | 🔒 Web Authentication | Basic login/auth in ESP-IDF HTTP server |
| 17 | 📲 Sending Data to the Cloud | `esp_http_client.h`, JSON format, REST API integration |
| 18 | 💬 ESP-NOW Peer-to-Peer Communication | `esp_now_init()`, secure send/receive, MAC pairing |
| 19 | 🧠 BLE: Bluetooth Low Energy | `esp_gatts_register_callback()`, services and characteristics |
| 20 | 💤 Deep Sleep & Wake Sources | `esp_sleep_enable_gpio_wakeup()`, RTC timer, low-power modes |
| 21 | ✨ Capacitive Touch Input | `touch_pad_config()`, touch threshold, filtering |
| 22 | 💾 SPIFFS and LittleFS File System | `esp_vfs_spiffs_register()`, reading/writing to internal flash |
| 23 | 📷 ESP32-CAM Basics | Camera init, capture, streaming, `esp_http_server` + MJPEG |
| 24 | 🔄 Over-the-Air (OTA) Updates | `esp_https_ota()`, firmware partitioning, version control |
| 25 | 📦 Project: Smart Room Sensor Node | Final project combining ADC, Wi-Fi, web server, and OTA |

---

## 🧭 How to Use This Repository

Each lesson has its own folder:
/lesson-01-blink-led
/lesson-02-7segment
/lesson-03-button-input
…

Inside each folder:
- `README.md` – Full explanation, wiring instructions, and learning goals  
- `main.c` – ESP-IDF C code for the lesson  
- `CMakeLists.txt` – Build configuration for ESP-IDF  
- `sdkconfig` – ESP-IDF configuration file (can be reused or customized)  
- (optional) wiring diagrams and demo GIFs  

---

## 🚀 Let's Build Real Embedded Skills

This is more than just a blink demo — this is a full **learning path** for becoming confident in embedded programming with the ESP32.

New lessons will be added regularly.  
Contributions, suggestions, and forks are welcome!

---
# 📘 ESP32 Embedded Systems One-Spot Learning Hub

Welcome to the **ESP32 Embedded Systems One-Spot Learning Hub** — a structured, hands-on learning center for mastering embedded systems development using the ESP32 microcontroller.

Each lesson builds on the last, starting with basic GPIO and growing into advanced systems with Wi-Fi, cloud communication, real-time multitasking, and low-power design. This series is ideal for students, engineers, and hobbyists alike.

---

## 📚 Complete Progressive Lesson Index (Tentative)

| Lesson # | Title | Concepts Introduced |
|----------|-------|---------------------|
| 1 | 🔴 Blinking an LED | `pinMode()`, `digitalWrite()`, `delay()`, GPIO output basics |
| 2 | 🔢 Driving a 7-Segment Display | Arrays, multi-pin output, digit mapping, custom functions |
| 3 | 🔘 Button Input to Control LED | `digitalRead()`, `pinMode(INPUT)`, debouncing |
| 4 | 🧠 Non-Blocking Code with `millis()` | Replacing `delay()`, time tracking, multitasking |
| 5 | 🧲 External Interrupts and Edge Triggers | `attachInterrupt()`, rising/falling edges, interrupt service routines |
| 6 | 🎛️ Potentiometer and Analog Read | `analogRead()`, analog input, signal scaling |
| 7 | 💡 PWM and Dimming an LED | `analogWrite()`, pulse-width modulation, fading LEDs |
| 8 | 🔊 Controlling a Buzzer | Generating tones, `tone()`, `noTone()`, non-blocking buzzer |
| 9 | 📡 UART Serial Communication | `Serial.begin()`, `Serial.print()`, debugging, serial monitor |
| 10 | 🧵 Intro to FreeRTOS on ESP32 | Creating tasks, `xTaskCreate()`, multitasking basics |
| 11 | 🌡️ Reading a Temperature Sensor | Interfacing analog and digital sensors (e.g., LM35, DHT11), sensor libraries |
| 12 | 🧰 Using I2C: OLED Display | I2C protocol, SSD1306 driver, `Wire.h`, `Adafruit_GFX` |
| 13 | 🔌 Using SPI: External Devices | SPI basics, chip select logic, communicating with SPI peripherals |
| 14 | 🕹️ Multi-Input System Integration | Buttons + sensors + display, state machines, UX feedback |
| 15 | 📶 Wi-Fi Basics: Connecting to a Network | `WiFi.h`, SSID/password setup, IP configuration |
| 16 | 🌐 Hosting a Web Page on ESP32 | Basic web server, HTML interface, controlling GPIO remotely |
| 17 | 🔒 Web Authentication | Protecting web pages with login credentials |
| 18 | 📲 Sending Data to the Cloud | HTTP POST, APIs, JSON, integration with ThingSpeak or custom servers |
| 19 | 💬 ESP-NOW Peer-to-Peer Communication | ESP-NOW protocol, secure node-to-node communication |
| 20 | 📦 Project: Smart Room Sensor Node | Final project combining sensors, UI, Wi-Fi, OTA, and power saving |
| 21 | 🧠 BLE: Bluetooth Low Energy | BLE advertising, services, characteristics, scanning |
| 22 | 💤 Deep Sleep & Wake Sources | Power saving modes, `esp_sleep.h`, timer and GPIO wake |
| 23 | ✨ Capacitive Touch Input | ESP32 touch-sensitive GPIOs, filtering and touch threshold |
| 24 | 💾 SPIFFS and LittleFS File System | Storing HTML/config/log files, reading and writing from flash |
| 25 | 📷 ESP32-CAM Basics | Taking photos, streaming video, face detection, camera web server |
| 26 | 🔄 Over-the-Air (OTA) Updates | `ArduinoOTA`, remote firmware update, safe deployment |

---

## 🧭 How to Use This Repository

Each lesson has its own folder:
/lesson-01-blink-led
/lesson-02-7segment
/lesson-03-button-input
…

Inside each folder:
- `README.md` – Full explanation, wiring instructions, and learning goals  
- `code.ino` – ESP32 code  
- (optional) wiring diagrams and demo GIFs

---

## 🚀 Let's Build Real Embedded Skills

This is more than just a blink demo — this is a full **learning path** for becoming confident in embedded programming with the ESP32.

New lessons will be added regularly.  
Contributions, suggestions, and forks are welcome!

---
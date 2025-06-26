# 📡 Lesson 08: UART Serial Communication

In this lesson, we explore how to send and receive data over UART using the ESP32. UART (Universal Asynchronous Receiver/Transmitter) is a serial communication protocol commonly used for debugging and data transfer between microcontrollers, sensors, or a computer.

## 🧠 Objectives

- Configure a UART peripheral using the ESP-IDF
- Send and receive serial data between the ESP32 and a USB-to-TTL adapter
- Use terminal software on your computer to view the data

## 🔌 Circuit

**Connections:**

| ESP32 GPIO | USB-to-TTL Pin |
|------------|----------------|
| GPIO4 (TX) | RX             |
| GPIO5 (RX) | TX             |
| GND        | GND            |

> ⚠️ Make sure not to connect the 5V pin from the USB-TTL to your ESP32. Only use TX, RX, and GND.

## 📄 Code

```c
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#define UART_PORT UART_NUM_1
#define BUF_SIZE 1024
#define TXD_PIN (GPIO_NUM_4)
#define RXD_PIN (GPIO_NUM_5)

void app_main(void)
{
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_PORT, &uart_config);
    uart_set_pin(UART_PORT, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_PORT, BUF_SIZE * 2, 0, 0, NULL, 0);

    const char *welcome_msg = "Type something and I will echo it back:\n";
    uart_write_bytes(UART_PORT, welcome_msg, strlen(welcome_msg));

    uint8_t data[BUF_SIZE];

    while (1) {
        int len = uart_read_bytes(UART_PORT, data, BUF_SIZE, pdMS_TO_TICKS(1000));
        if (len > 0) {
            uart_write_bytes(UART_PORT, (const char *) data, len);
        }
    }
}
```
## 💡 Code Concepts

- **UART (Universal Asynchronous Receiver/Transmitter):**
  A hardware communication protocol for serial data transmission between devices.

- **`uart_config_t`**:  
  A structure used to define UART settings like baud rate, data bits, parity, stop bits, and flow control.

- **`uart_param_config()`**:  
  Applies the UART configuration settings to the specified UART port.

- **`uart_set_pin()`**:  
  Assigns the TX and RX GPIO pins for UART communication.

- **`uart_driver_install()`**:  
  Installs the UART driver and allocates the necessary buffer space.

- **`uart_write_bytes()`**:  
  Sends data over UART by writing bytes to the transmission buffer.

- **`uart_read_bytes()`**:  
  Reads incoming UART data into a buffer with a specified timeout period.

- **TX and RX Pins**:  
  TX (Transmit) sends data, RX (Receive) receives data. These must be crossed when connecting two devices (ESP32 TX → TTL RX and vice versa).
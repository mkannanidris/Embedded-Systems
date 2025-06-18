# Lesson 1: Blinking an LED with ESP32

Welcome to the first lesson 
In this lesson, we learn the most fundamental operation in microcontroller programming: **blinking an LED**.

## 🧠 Objective

- Understand how to control a digital output pin on the ESP32
- Use `digitalWrite()` and `delay()` to blink the on-board LED


## 📄 Code

```cpp
const int LED_PIN = 2; 

void setup() {
  pinMode(LED_PIN, OUTPUT); // This is the on-board LED pin on most ESP32 boards
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(500); // LED ON for 0.5 seconds
  digitalWrite(LED_PIN, LOW);
  delay(500); // LED OFF for 0.5 seconds
}
```

## 🔍 Code Explanation

- `const int LED_PIN = 2;`  
  Declares a constant variable for the LED pin. On most ESP32 boards, pin 2 is connected to the on-board LED.

- `void setup() { ... }`  
  This function runs once when the ESP32 is powered on or reset.

- `pinMode(LED_PIN, OUTPUT);`  
  Configures pin 2 to act as an output so it can send voltage to the LED.

- `void loop() { ... }`  
  This function runs continuously in a loop. It defines the behavior of your device.

- `digitalWrite(LED_PIN, HIGH);`  
  Sends a HIGH signal (3.3V) to the LED pin — turns the LED ON.

- `delay(500);`  
  Waits for 500 milliseconds (0.5 seconds) with the LED on.

- `digitalWrite(LED_PIN, LOW);`  
  Sends a LOW signal (0V) to the LED pin — turns the LED OFF.

- `delay(500);`  
  Waits for another 500 milliseconds with the LED off before repeating the loop.
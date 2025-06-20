# Lesson 3: Button Input to Control LED

Welcome to Lesson 3  
In this lesson, we connect a push-button to the ESP32 and use it to control an LED.  
We’ll read the state of the button and turn the LED on or off depending on whether the button is pressed.

---

## 🧠 Objective

- Learn how to read digital input from a button
- Use `digitalRead()` to make decisions
- Control an output (LED) based on input state

---

## 📄 Code

```cpp
const int BUTTON_PIN = 0;  // Connect button to GPIO 0
const int LED_PIN = 2;     // On-board LED on GPIO 2

void setup() {
  pinMode(BUTTON_PIN, INPUT);    // Set button pin as input
  pinMode(LED_PIN, OUTPUT);      // Set LED pin as output
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);  // Read the button state (HIGH or LOW)

  if (buttonState == HIGH) {
    digitalWrite(LED_PIN, HIGH);  // Turn LED on when button is pressed
  } else {
    digitalWrite(LED_PIN, LOW);   // Turn LED off when button is released
  }
}
```
## 🔍 Code Explanation

- `const int BUTTON_PIN = 0;`  
  Sets GPIO 0 as the input pin connected to the button. This is typically the boot button on most ESP32 boards.

- `const int LED_PIN = 2;`  
  Sets GPIO 2 as the output pin connected to the on-board LED.

- `void setup()`  
  This function runs once at startup:
  - `pinMode(BUTTON_PIN, INPUT);` sets the button pin as input.
  - `pinMode(LED_PIN, OUTPUT);` sets the LED pin as output.

- `void loop()`  
  This function runs continuously:
  - `digitalRead(BUTTON_PIN)` reads the current state of the button.
  - If the button is pressed (`HIGH`), the LED is turned on.
  - If the button is not pressed (`LOW`), the LED is turned off.
# Lesson 2: Driving a 7-Segment Display with ESP32

Welcome to the second lesson  
In this lesson, we learn how to control a **7-segment display** using the ESP32 and display digits from 0 to 9.

## 🧠 Objective

- Learn how to use an array to map GPIO pins to display segments
- Understand how to represent digits using binary patterns for segment control
- Practice looping and function usage to modularize code

## 📄 Code

```cpp
const int segmentPins[] = {1, 2, 3, 4, 5, 6, 7};

void setup() {
  // Configure all segment pins as outputs in a loop
  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
}

void loop() {
  // Count from 0 to 9
  for (int counter = 0; counter <= 9; counter++) {
    Segment7(counter);
    delay(1000);
  }
  Blink_X3();
  delay(1000);
}

void Segment7(int var) {
  // Define the common segments for each digit
  const int commonSegments[][7] = {
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

  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], commonSegments[var][i]);
  }
}

void Blink_X3(){
  int delayTime = 200;
  for(int j=1; j<=3; j +=1){
    for (int i=1; i<=7; i +=1){
      digitalWrite(i, HIGH);
    }
    delay(delayTime);
    for (int i=1; i<=7; i +=1){
      digitalWrite(i, LOW);
    }
    delay(delayTime);
  }
}
```
## 🔍 Code Explanation

- `const int segmentPins[] = {1, 2, 3, 4, 5, 6, 7};`  
  Defines an array of GPIO pins connected to the 7 segments (a to g) of the display.

- `void setup()`  
  Runs once at startup. A `for` loop sets each segment pin as an output using `pinMode()`.

- `void loop()`  
  Repeats continuously:
  - Counts from 0 to 9, displaying each digit for 1 second by calling `Segment7(counter)`.
  - After the count, all segments blink three times via the `Blink_X3()` function.
  - Waits 1 second before restarting the loop.

- `void Segment7(int var)`  
  - Contains a 2D array `commonSegments` that defines which segments should be ON (1) or OFF (0) for digits 0 to 9.
  - A `for` loop writes each bit of the selected digit’s pattern to the corresponding segment pin using `digitalWrite()`.

- `void Blink_X3()`  
  - Blinks all 7 segments ON and OFF three times.
  - Uses nested `for` loops to control the blinking:  
    - Inner loop turns all pins ON, delays, then turns them OFF.  
    - Outer loop repeats this process three times.
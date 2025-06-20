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
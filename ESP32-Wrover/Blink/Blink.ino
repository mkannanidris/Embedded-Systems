const int LED_PIN = 2; 

void setup() {
  pinMode(LED_PIN, OUTPUT); //This is the on-board LED pin on my ESP32 board
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(500); // 0.5 second time delay
  digitalWrite(LED_PIN, LOW);
  delay(500);
}
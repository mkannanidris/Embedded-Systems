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

int Team = 1;

void LiftUp() {
  Serial.print('U');
}
void LiftTiltDown() {
  if (Team == 1) {
    Serial.print('Y');
  }
  if (Team == -1) {
    Serial.print('B');
  }
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  delay(5000);
  LiftUp();
  delay(5000);
  LiftTiltDown();
}

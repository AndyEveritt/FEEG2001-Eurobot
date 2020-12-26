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


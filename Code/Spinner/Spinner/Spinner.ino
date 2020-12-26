int SpinnerPin = 10;

void setup() {
  Serial.begin(9600);
  pinMode(SpinnerPin, OUTPUT);
}

void loop() {
  Spinner(false);
  delay(1000);
  Spinner(true);
  delay(1000);

}

void Spinner(boolean on){
  int SpinnerState = 0;
  if(on == true){
    digitalWrite(SpinnerPin, HIGH);
    SpinnerState = 1;
  }
  else if(on == false){
    digitalWrite(SpinnerPin, LOW);
    SpinnerState = 0;
  }
  Serial.print(SpinnerState);
}


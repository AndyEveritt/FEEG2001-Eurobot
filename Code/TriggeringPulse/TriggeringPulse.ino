int TriggerPin = 6;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(TriggerPin, OUTPUT);
  digitalWrite(TriggerPin, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10000);
  pulse();
}

void pulse() {
  Serial.println("StartT");
  digitalWrite(TriggerPin, LOW);
  digitalWrite(TriggerPin, HIGH);
  delay(1000);
  digitalWrite(TriggerPin, LOW);
  Serial.println("EndT");
  
}


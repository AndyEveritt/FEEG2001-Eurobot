/*
2016/17 Eurobot Task

Code written by Andrew Everitt - Why is this even a group project anyway?
*/

void Spinner(boolean on){
  /*
   * Controls the spinner by switching it between an on and off state
   */
  if(on == true){
    digitalWrite(SpinnerPin, HIGH);
  }
  else if(on == false){
    digitalWrite(SpinnerPin, LOW);
  }
  delay(200);
}


void SpinnerAngle(int Angle, int Speed){
  while (Angle - ServoAngle > 0){
    ServoAngle += Speed;
    Serial.println(ServoAngle);
    ServoSpinner.write(ServoAngle);
    delay(15);
  } 
  while (Angle - ServoAngle < 0){
    ServoAngle -= Speed;
    
    Serial.println(ServoAngle);
    ServoSpinner.write(ServoAngle);
    delay(15);
  }
  ServoSpinner.write(Angle);
    Serial.println(ServoAngle);
  ServoAngle = Angle;
  delay(1000);
}

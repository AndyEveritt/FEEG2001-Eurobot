/*
2016/17 Eurobot Task

Code written by Andrew Everitt
*/

void TimeCheck(){
  /*
   * Checks time since "Start" function has run, if the code has been active for longer than duration then the motors and spinner will be stopped
   */
  unsigned long CurrentMillis = millis();
  //Serial.println(CurrentMillis);
  while (CurrentMillis - StartMillis > Duration){
    stopMotor();
  }
}


void TimeDelay(int sec){
  unsigned long CurrentMillis2 = millis();
  while(millis() - CurrentMillis2 < sec*1000){TimeCheck();}
}


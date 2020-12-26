void Claw(float angle){
  if (Team == 1){
    ServoClawR.write(angle);
  }
  if (Team == -1){
    ServoClawL.write(180 - angle);
  }
}


void ClawReset(){
  ServoClawR.write(30);
  ServoClawL.write(160);
}

void Knocker(boolean Deploy){
  if (Deploy == true){
    ServoKnocker.write(70);
    Sensors = false;
  }
  if (Deploy == false){
    ServoKnocker.write(175);
    Sensors = true;
  }
}

void KnockerStart(float angle){
  ServoKnocker.write(angle);
}

void KnockerTest(){
  Claw(90);
  Knocker(true);
  delay(2000);
  Knocker(false);
  delay(2000);
  KnockerTest();
}


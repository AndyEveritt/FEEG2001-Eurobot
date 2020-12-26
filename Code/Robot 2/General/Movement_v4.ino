/*
2016/17 Eurobot Task

Code written by Andrew Everitt
*/


void TeamColourStart(){
  /*
   * Waits for a signal from the start switch before proceeding.
   * While waiting it will detect the team switch and indicate the team with an LED
   * Sets the start time for the "TimeCheck" function
   */
  while (digitalRead(StartPin) == HIGH){                             // Waits for signal from start switch
    if (digitalRead(TeamSwitch) == LOW){                             // Detects team switch position
      Team = 1;
      digitalWrite(YellowTeam, HIGH);                                // Changes LED to represent team
      digitalWrite(BlueTeam, LOW);
    }
    else{
      Team = -1;
      digitalWrite(BlueTeam, HIGH);
      digitalWrite(YellowTeam, LOW);
    }
  }
  StartMillis = millis();                                             // Reset timer after start switch has been triggered
}


void movement (int SpeedValue, float TurnAngle, float Distance, int Acceleration, boolean VariableSpeed, boolean Correction, boolean Avoidance){                                               // This function moves the platform with a set speed and turn value for each wheel until they have moved the defined distance set in cm
                                                                                                                                                              // If VariableSpeed is true (default) then the encoders will slow as a proportion of the distance moved, otherwise speed is constant
  int oscf = 1;
  int Speed1 = 0;                                                        // Creates a speed variable (SPEED1)
  int Speed2 = 0;                                                        // Creates a turn variable (SPEED2)
  float e1 = encoder1();                                                 // Calls a function that reads value of encoder 1
  float e2 = encoder2();                                                 // Calls a function that reads value of encoder 2

  float Distance_1_CM;                                                   // Creates a distance variable for wheel 1
  float Distance_2_CM;                                                   // Creates a distance variable for wheel 2

  TurnAngle = TurnAngle*Team;                                            // Mirrors all turn directions when team changes

  if(Correction == true){                                                // setting motor speeds and distances if the movement is a correction
    oscf = 0;
    int Sign1 = Correction_dist1/abs(Correction_dist1);                  // calulates the sign of D1 to determine whether the robot under or over moved
    int Sign2 = Correction_dist2/abs(Correction_dist2);                  // calulates the sign of D2 to determine whether the robot under or over moved

    int multi = 1;                                                       // fixes issue with oscf when correcting multiple times
    if (CorrectionCount > 1) {
      multi = 0; 
    }

    Speed1 = Sign1*SpeedValue;
    Distance_1_CM = Correction_dist1;
    if (abs(Distance_1_CM) < limit){                                 // Only corrects if the difference is larger enough as it will overcorrect if the distance is small
      Distance_1_CM = 0;
    }
    
    Speed2 = Sign2*SpeedValue;
    Distance_2_CM = Correction_dist2;
    if (abs(Distance_2_CM) < limit){                                 // Only corrects if the difference is larger enough as it will overcorrect if the distance is small
      Distance_2_CM = 0;
    }
    
//    Serial.println(Distance_1_CM);
//    Serial.println(Distance_2_CM);
//    Serial.println(Speed1);
//    Serial.println(Speed2);
  }

  else if (Avoidance == true){
    int Sign1 = e1/abs(e1);
    int Sign2 = e2/abs(e2);
    Speed1 = -Sign1*SpeedValue;
    Speed2 = -Sign2*SpeedValue;
    Distance_1_CM = -e1*3/4;
    Distance_2_CM = -e2*3/4;
  }
  
  else if(TurnAngle == 0){                                                // setting motor speeds and distances for a straight line movement
    Distance_1_CM = Distance_2_CM = Distance;
    Speed1 = Speed2 = SpeedValue * Distance/abs(Distance);
//    Serial.println(Distance_1_CM);
//    Serial.println(Distance_2_CM);
//    Serial.println(Speed1);
//    Serial.println(Speed2);
  }
  
  else{                                                                     // setting motor speeds and distances for a turn on the spot
    Distance_1_CM = (WheelBaseDiameter/2) * TurnAngle * (pi/180.0);
    Distance_2_CM = (WheelBaseDiameter/2) * TurnAngle * (-pi/180.0);
    Speed1 = SpeedValue * (TurnAngle/abs(TurnAngle));
    Speed2 = SpeedValue * (-TurnAngle/abs(TurnAngle));
//    Serial.println(Distance_1_CM);
//    Serial.println(Distance_2_CM);
//    Serial.println(Speed1);
//    Serial.println(Speed2);
  }

    Serial.println(Distance_1_CM);
    Serial.println(Distance_2_CM);
    Serial.println(Speed1);
    Serial.println(Speed2);
    
  if (Avoidance == false){
    Last_Distance_1 = Distance_1_CM;                                                    // Sets the distance for wheel 1 as a global variable so it can be used by the DistCorrection function
    Last_Distance_2 = Distance_2_CM;                                                    // Sets the distance for wheel 2 as a global variable so it can be used by the DistCorrection function
    Last_Speed1 = Speed1;
    Last_Speed2 = Speed2;
  }

  OvershootDist = OvershootDist*abs(Distance_1_CM/30.0);
  
  if (Avoidance == true){
    e1 = encoder1();
    e2 = encoder2();
    while(abs(e1) > abs(Distance_1_CM) || abs(e2) > abs(Distance_2_CM)){
      if(abs(e1) <= abs(Distance_1_CM)-oscf*OvershootDist){Speed1 = 0;}                               // Stops motor 1 if it has finished its movement (does not effect motor 2)
      if(abs(e2) <= abs(Distance_2_CM)-oscf*OvershootDist){Speed2 = 0;}                               // Stops motor 2 if it has finished its movement (does not effect motor 1)
      
      Wire.beginTransmission(MD25ADDRESS);                                              // Sets the acceleration to register 1
      Wire.write(ACCELERATION);
      Wire.write(Acceleration);
      Wire.endTransmission();

      Wire.beginTransmission(MD25ADDRESS);                                              // Sets a combined motor speed value
      Wire.write(SPEED1);
      Wire.write(Speed1);
      Wire.endTransmission();
    
      Wire.beginTransmission(MD25ADDRESS);                                             //Sets a turning motor speed value
      Wire.write(SPEED2);
      Wire.write(Speed2);
      Wire.endTransmission();

      TimeCheck();
      
      e1 = encoder1();
      e2 = encoder2();
    }
  }
  
  else{
  while(abs(e1) < abs(Distance_1_CM)- oscf*OvershootDist || abs(e2) < abs(Distance_2_CM)-oscf*OvershootDist){     // while statement to check the status of the traveled distance, if either motor needs to move further the loop is active, absolutes used incase the motors are in reverse

    e1 = encoder1();                                                                  // Updates encoder 1 reading
    e2 = encoder2();                                                                  // Updates encoder 2 reading

    if(abs(e1) >= abs(Distance_1_CM)-oscf*OvershootDist){Speed1 = 0;}                               // Stops motor 1 if it has finished its movement (does not effect motor 2)
    
    else if(VariableSpeed == true && (e1/Distance_1_CM) < ss){                        // Checks whether speed needs to be varibale or constant and relative distance is below a set percentage 'ss'
      float c1 = (SpeedValue-StartSpeed)/(pow((ss), 2));
      float c3 = StartSpeed;
      float PT1 = e1/Distance_1_CM;
      Speed1 =  (c1*pow(PT1, 2) + c3) * (Speed1/abs(Speed1));                         // If speed is variable then it slowly increases Speed1 as a quadratic from StartSpeed to SpeedValue
    }
    
    else if(VariableSpeed == true && (e1/Distance_1_CM) < vsco && (e1/Distance_1_CM) > ss){
      Speed1 = Last_Speed1;
    }
    
    else if(VariableSpeed == true && (e1/Distance_1_CM) > vsco){                      // Checks whether speed needs to be varibale or constant and relative distance is above a set percentage 'vsco'
      float c1 = (FinalSpeed-SpeedValue)/(pow((vsco-1), 2));
      float c2 = c1*2*(pow(vsco, 2)-vsco)/(1-vsco);
      float c3 = FinalSpeed - c1 - c2;
      float PT1 = e1/Distance_1_CM;
      Speed1 =  (c1*pow(PT1, 2) + c2*PT1 + c3) * (Speed1/abs(Speed1));                // If speed is variable then it slowly decreases Speed1 as a quadratic from SpeedValue to FinalSpeed
    }

    if(abs(e2) >= abs(Distance_2_CM)-oscf*OvershootDist){Speed2 = 0;}                               // Stops motor 2 if it has finished its movement (does not effect motor 1)

    else if(VariableSpeed == true && (e2/Distance_2_CM) < ss){                        // Checks whether speed needs to be varibale or constant and relative distance is below a set percentage 'ss'
      float c1 = (SpeedValue-StartSpeed)/(pow((ss), 2));
      float c3 = StartSpeed;
      float PT1 = e2/Distance_2_CM;
      Speed2 =  (c1*pow(PT1, 2) + c3) * (Speed2/abs(Speed2));                         // If speed is variable then it slowly increases Speed2 as a quadratic from StartSpeed to SpeedValue
    }

    else if(VariableSpeed == true && (e2/Distance_2_CM) < vsco && (e2/Distance_2_CM) > ss){
      Speed2 = Last_Speed2;
    }
    
    else if(VariableSpeed == true && (e2/Distance_2_CM) > vsco){                      // Checks whether speed needs to be varibale or constant and relative distance is above a set percentage 'vsco'
      float c1 = (FinalSpeed-SpeedValue)/(pow((vsco-1), 2));
      float c2 = c1*2*(pow(vsco, 2)-vsco)/(1-vsco);
      float c3 = FinalSpeed - c1 - c2;
      float PT2 = e2/Distance_2_CM;
      Speed2 =  (c1*pow(PT2, 2) + c2*PT2 + c3) * (Speed2/abs(Speed2));                // If speed is variable then it slowly decreases Speed2 as a quadratic from SpeedValue to FinalSpeed
    }
    
    Wire.beginTransmission(MD25ADDRESS);                                              // Sets the acceleration to register 1
    Wire.write(ACCELERATION);
    Wire.write(Acceleration);
    Wire.endTransmission();

    Wire.beginTransmission(MD25ADDRESS);                                              // Sets a combined motor speed value
    Wire.write(SPEED1);
    Wire.write(Speed1);
    Wire.endTransmission();
    
    Wire.beginTransmission(MD25ADDRESS);                                             //Sets a turning motor speed value
    Wire.write(SPEED2);
    Wire.write(Speed2);
    Wire.endTransmission();

    e1 = encoder1();                                                                // Updates encoder 1 reading
    e2 = encoder2();                                                                // Updates encoder 2 reading

    Serial.print("e1 \t");
    Serial.print(e1);
    Serial.print("\t e2 \t");
    Serial.println(e2);
    
    TimeCheck();                                                                    // Checks time
    if (Correction == false && (e1/Distance_1_CM) > 0.1){
      DistanceCheck();                                                              // Checks distance
    }
  }
  }
  
  stopMotor();                                                                      // Ensures both motors have stopped
//  Serial.println("end");
//  Serial.print("D1 \t");
//  Serial.print(Distance_1_CM);
//  Serial.print("\t D2 \t");
//  Serial.println(Distance_2_CM);

  CorrectionCount += 1;                                                             // Counts the number of correction loops for the current movement
//  Serial.println(CorrectionCount);
  
  if(CorrectionCount <= CorrectionIteration && Avoidance == false){                                       // Corrects distance if the number of loops as not been exceeded
    DistCorrection();
  }
  CorrectionCount = 0;                                                              // Resets correction iteration
}


void DistCorrection(){                                        // Corrects any over/undermovement due to momentum, corrects each wheel independantly to do both distance and angle.
  float e1 = encoder1();                                      // Calls a function that reads value of encoder 1
  float e2 = encoder2();                                      // Calls a function that reads value of encoder 2
    Serial.print("e1 \t");
    Serial.print(e1);
    Serial.print("\t e2 \t");
    Serial.println(e2);
  
  Correction_dist1 = (Last_Distance_1 - e1);                          // calculates difference between the encoder distance and the desired distance for wheel 1
  Correction_dist2 = (Last_Distance_2 - e2);                          // calculates difference between the encoder distance and the desired distance for wheel 2

  Serial.println(CorrectionCount);
  Serial.print("C1 \t");
  Serial.print(Correction_dist1);
  Serial.print("\t C2 \t");
  Serial.println(Correction_dist2);
  
  encodeReset();

  movement(CorrectionSpeed, 0, 0, 1, false, true);
  
  encodeReset();                                                                       // Resets the encoders to 0
}


void encodeReset(){                                         // This function resets the encoder values to 0
  Wire.beginTransmission(MD25ADDRESS);
  Wire.write(CMD);
  Wire.write(0x20);                                         
  Wire.endTransmission(); 
  delay(50);
}


float encoder1(){                                           // Function to read and display value of encoder 1 as a long
  Wire.beginTransmission(MD25ADDRESS);                      // Send byte to get a reading from encoder 1
  Wire.write(ENCODERONE);
  Wire.endTransmission();

  Wire.requestFrom(MD25ADDRESS, 4);                         // Request 4 bytes from MD25
  while(Wire.available() < 4);                              // Wait for 4 bytes to arrive
  long poss1 = Wire.read();                                 // First byte for encoder 1, HH.
  poss1 <<= 8;
  poss1 += Wire.read();                                     // Second byte for encoder 1, HL
  poss1 <<= 8;
  poss1 += Wire.read();                                     // Third byte for encoder 1, LH
  poss1 <<= 8;
  poss1  +=Wire.read();                                     // Fourth byte for encoder 1, LLalue
  delay(5);                                                 // Wait for everything to make sure everything is sent
  return(poss1*(WheelDiameter*pi/360));                     // Convert encoder value to cm using wheel diameter
}


float encoder2(){                                            // Function to read and display velue of encoder 2 as a long
  Wire.beginTransmission(MD25ADDRESS);           
  Wire.write(ENCODERTWO);
  Wire.endTransmission();

  Wire.requestFrom(MD25ADDRESS, 4);                         // Request 4 bytes from MD25
  while(Wire.available() < 4);                              // Wait for 4 bytes to become available
  long poss2 = Wire.read();                                 // First byte for encoder 2, HH
  poss2 <<= 8;
  poss2 += Wire.read();                                     // Second byte for encoder 2, HL             
  poss2 <<= 8;
  poss2 += Wire.read();                                     // Third byte for encoder 2, LH             
  poss2 <<= 8;
  poss2  +=Wire.read();                                     // Fourth byte for encoder 2, LLalue
  delay(5);                                                 // Wait to make sure everything is sent
  return(poss2*(WheelDiameter*pi/360));                     // Convert encoder value to cm using wheel diameter
}


void stopMotor(){                                           // Function to stop motors
  
  Wire.beginTransmission(MD25ADDRESS);                      // Sets the acceleration to register 10 (0.65s)
  Wire.write(ACCELERATION);
  Wire.write(5);
  Wire.endTransmission();

  Wire.beginTransmission(MD25ADDRESS);                      // Stops motors motor 1 if operated in MODE 0 or 1 and Stops both motors if operated in MODE 2 or 3
  Wire.write(SPEED1);
  Wire.write(0);
  Wire.endTransmission();

  Wire.beginTransmission(MD25ADDRESS);                      // Stops motors motor 2 when operated in MODE 0 or 1 and Stops both motors while in turning sequence if operated in MODE 2 or 3
  Wire.write(SPEED2);
  Wire.write(0);
  Wire.endTransmission();
  delay(200);

  encoder1();                                                // Calls a function that reads value of encoder 1
  encoder2();                                                // Calls a function that reads value of encoder 2
}  

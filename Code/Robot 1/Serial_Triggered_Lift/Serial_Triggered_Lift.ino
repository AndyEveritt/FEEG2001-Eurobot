#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Create the motor shield object with the default address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Connect stepper motors with 768 steps per revolution - well apparently 
Adafruit_StepperMotor *myMotor1 = AFMS.getStepper(768, 1);
Adafruit_StepperMotor *myMotor2 = AFMS.getStepper(768, 2);



int incomingByte;                                 //used for incoming data from other arduino
int UpHeight = 900;                              //number of steps to just below the cargo height so balls are near but dont fall out top, will need to increase as battery dies
int RemainingHeight = 400;                        //number of steps remaining after UpHeight so that lift is now at edge of ramp, will need to increase as battery dies
int TiltSteps = 650;                              //number of steps that motor furthest from cargo bay will move up and so defines angle of tilt of lift


//Up function for lift that rapidly switches between the two motors
void Up() {
  for(int s=0; s<UpHeight; s++){
    myMotor1->step(1, BACKWARD, DOUBLE); 
    myMotor2->step(1, BACKWARD, DOUBLE);
    EndCheck();
    
  }
}


//goes up with double torque, tilts so balls fall out Yellow side and goes down again with single torque 
void YellowTiltDown() {
  for(int s=0; s<RemainingHeight; s++){
    myMotor1->step(1, BACKWARD, DOUBLE); 
    myMotor2->step(1, BACKWARD, DOUBLE); 
    EndCheck();
  }

  int i = 1;
  while (i <= 2){
    myMotor2->step(TiltSteps, BACKWARD, DOUBLE);
    delay(1000);
    EndCheck();
    myMotor2->step(TiltSteps, FORWARD, SINGLE);
    delay(100);
    Serial.println(i);
    i += 1;
    EndCheck();
  }
  
  for(int s=0; s<RemainingHeight; s++){
    myMotor1->step(1, FORWARD, SINGLE);
    myMotor2->step(1, FORWARD, SINGLE);
    EndCheck();
  }
  
  for(int s=0; s<UpHeight; s++){
    myMotor1->step(1, FORWARD, SINGLE);
    myMotor2->step(1, FORWARD, SINGLE);
    EndCheck();
  }
}


//goes up with double torque, tilts so balls fall out Blue side and goes down again with single torque 
void BlueTiltDown() {
  for(int s=0; s<RemainingHeight; s++){
    myMotor1->step(1, BACKWARD, DOUBLE); 
    myMotor2->step(1, BACKWARD, DOUBLE); 
    EndCheck();
  }

  int i = 1;
  while (i <= 2){
    myMotor1->step(TiltSteps, BACKWARD, DOUBLE);
    delay(1000);
    EndCheck();
    myMotor1->step(TiltSteps, FORWARD, SINGLE);
    delay(100);
    i += 1;
    EndCheck();
  }
  
  for(int s=0; s<RemainingHeight; s++){
    myMotor1->step(1, FORWARD, SINGLE);
    myMotor2->step(1, FORWARD, SINGLE);
    EndCheck();
  }
  
  for(int s=0; s<UpHeight; s++){
    myMotor1->step(1, FORWARD, SINGLE);
    myMotor2->step(1, FORWARD, SINGLE);
    EndCheck();
  }
}


void EndCheck(){
  if (Serial.available() > 0){
    incomingByte = Serial.read();
    if (incomingByte == 'S'){
      delay(100000);
    }
  }
}


void setup() {
  Serial.begin(9600);
  
  AFMS.begin();  // create with the default frequency 1.6KHz
  myMotor2->setSpeed(80);  // 80 rpm of motors when used on their own e.g. for tilts
}


void loop() {
  //Used for resetting to 0 position
//  myMotor1->step(100, BACKWARD, DOUBLE);
//  myMotor2->step(100, BACKWARD, DOUBLE);

// Used for calibration
//delay(5000);
//Up();
//delay(5000);
//YellowTiltDown();
//delay(100000);

//Main code
  // see if there's incoming serial data:
  if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();
    if (incomingByte == 'U') {
      Serial.println("UpRecieved");
      Up();
    }
        if (incomingByte == 'Y') {
//      Serial.println("Yellow Team");
      YellowTiltDown();
    }
    if (incomingByte == 'B') {
//      Serial.println("Blue Team");
      BlueTiltDown();
  }
}
else{
//  Serial.println("NoTransmission");
}
}


#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor1 = AFMS.getStepper(768, 1);
Adafruit_StepperMotor *myMotor2 = AFMS.getStepper(768, 2);

int incomingByte; 
int CargoHeight = 1450;
int TiltSteps = 300;


void Up() {
  for(int s=0; s<CargoHeight; s++)
{
  myMotor1->step(1, BACKWARD, DOUBLE); 
  myMotor2->step(1, BACKWARD, DOUBLE); 
  
}
}
void YellowTiltDown() {
  myMotor1->step(TiltSteps, BACKWARD, DOUBLE);
  delay(5000);
  myMotor1->step(TiltSteps, FORWARD, SINGLE);
  for(int s=0; s<CargoHeight; s++)
{
  myMotor1->step(1, FORWARD, SINGLE);
  myMotor2->step(1, FORWARD, SINGLE);
}
}

void BlueTiltDown() {
  myMotor2->step(TiltSteps, BACKWARD, DOUBLE);
  delay(5000);
  myMotor2->step(TiltSteps, FORWARD, SINGLE);
  for(int s=0; s<CargoHeight; s++)
{
  myMotor1->step(1, FORWARD, SINGLE);
  myMotor2->step(1, FORWARD, SINGLE);
}
}

void setup() {
  Serial.begin(9600);
  
  AFMS.begin();  // create with the default frequency 1.6KHz
  myMotor2->setSpeed(80);  // 80 rpm   
}

void loop() {
  //Used for resetting to 0 position
//  myMotor1->step(100, BACKWARD, DOUBLE);
//  myMotor2->step(100, BACKWARD, DOUBLE);

// Used for calibration
//delay(10000);
//for(int s=0; s<CargoHeight; s++)
//{
//  myMotor1->step(1, BACKWARD, DOUBLE); 
//  myMotor2->step(1, BACKWARD, DOUBLE); 
//}
//  myMotor2->step(TiltSteps, BACKWARD, DOUBLE);
//  delay(5000);
//  myMotor2->step(TiltSteps, FORWARD, SINGLE);
//  for(int s=0; s<CargoHeight; s++)
//{
//  myMotor1->step(1, FORWARD, SINGLE);
//  myMotor2->step(1, FORWARD, SINGLE);
//}
//delay(100000);

  // see if there's incoming serial data:
  if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();
    // if it's a capital H (ASCII 72), turn on the LED:
    if (incomingByte == 'U') {
      Serial.println("UpRecieved");
      Up();
    }
    // if it's an L (ASCII 76) turn off the LED:
    if (incomingByte == 'Y') {
      Serial.println("Yellow Team");
      YellowTiltDown();
    }
    if (incomingByte == 'B') {
      Serial.println("Blue Team");
      BlueTiltDown();
  }
}
else{
  Serial.println("NoTransmission");
}
}


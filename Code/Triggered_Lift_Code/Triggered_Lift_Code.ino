/* 
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2 
---->	http://www.adafruit.com/products/1438
*/

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor1 = AFMS.getStepper(768, 1);
Adafruit_StepperMotor *myMotor2 = AFMS.getStepper(768, 2);
int TriggeredPin = 2;

void UpDown() {
  for(int s=0; s<1700; s++)
{
  myMotor1->step(1, BACKWARD, DOUBLE); 
  myMotor2->step(1, BACKWARD, DOUBLE); 
}
  myMotor1->step(300, BACKWARD, DOUBLE);
  delay(5000);
  myMotor1->step(300, FORWARD, SINGLE);
  for(int s=0; s<1700; s++)
{
  myMotor1->step(1, FORWARD, SINGLE);
  myMotor2->step(1, FORWARD, SINGLE);
}
}



void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Stepper test!");
  pinMode(TriggeredPin, INPUT_PULLUP);
  

  AFMS.begin();  // create with the default frequency 1.6KHz

  
  myMotor2->setSpeed(80);  // 80 rpm   
}

void loop() {
//  myMotor1->step(100, BACKWARD, DOUBLE);
//  myMotor2->step(100, BACKWARD, DOUBLE);
//delay(10000);
//for(int s=0; s<1550; s++)
//{
//  myMotor1->step(1, BACKWARD, DOUBLE); 
//  myMotor2->step(1, BACKWARD, DOUBLE); 
//}
//  myMotor2->step(300, BACKWARD, DOUBLE);
//  delay(5000);
//  myMotor2->step(300, FORWARD, SINGLE);
//  for(int s=0; s<1550; s++)
//{
//  myMotor1->step(1, FORWARD, SINGLE);
//  myMotor2->step(1, FORWARD, SINGLE);
//}
//delay(100000);


  while(digitalRead(TriggeredPin) == LOW){
    Serial.println("NoT");
    delay(200);

  }
  Serial.println("TransmissionRecieved");
  UpDown();
}
//    myMotor->release();
  



// Chapter 7 - Communications
// I2C Slave
// By Cornel Amariei for Packt Publishing

// Include the required Wire library for I2C
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor2 = AFMS.getStepper(768, 2);

int x = 0;

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Stepper test!");
  AFMS.begin();  // create with the default frequency 1.6KHz

  
  myMotor2->setSpeed(80);  // 80 rpm   
  
  // Start the I2C Bus as Slave on address 9
  Wire.begin(9); 
  // Attach a function to trigger when something is received.
  Wire.onReceive(UpDown);
}

void UpDown(int bytes) {
  x = Wire.read();    // read one character from the I2C
  myMotor2->step(1900, FORWARD, DOUBLE); 
  delay(5000);
  myMotor2->step(1900,BACKWARD, SINGLE);
  delay(10000);
}
void loop() {
  delay(500);
}


/*
2016 Eurobot Task

Code written by Andrew Everitt
*/

#include <Wire.h>                                             // Calls for I2C bus library
#include <Servo.h>                                            // calls for the servo library

#define MD25ADDRESS         0x58                              // Address of the MD25
#define SPEED1              0x00                              // Byte to send speed to both motors for forward and backwards motion if operated in MODE 2 or 3 and Motor 1 Speed if in MODE 0 or 1
#define SPEED2              0x01                              // Byte to send speed for turn speed if operated in MODE 2 or 3 and Motor 2 Speed if in MODE 0 or 1
#define ENCODERONE          0x02                              // Byte to read motor encoder 1
#define ENCODERTWO          0x06                              // Byte to read motor encoder 2
#define ACCELERATION        0xE                               // Byte to define motor acceleration
#define CMD                 0x10                              // Byte to reset encoder values
#define MODE_SELECTOR       0xF                               // Byte to change between control MODES

// Inputs/Outputs
int LEDPin = 3;                                               // Sets output pin for LED
int buttonPin = 10;                                           // sets the input pin for the start button
Servo myservo;                                                // create servo object to control a servo

// Chasis Parameters & Constants
float WheelBaseDiameter = 32;                               // sets the distance between the wheels in cm
float WheelDiameter = 10.0;                                  // sets the diameter of the wheel
const float pi = 3.14159265359;                               // constant variable roughly equal to pi

// Movement variables
int Mode = 1;                                                 // MODE in which the MD25 will operate selector value
int Team = -1;                                                 // Yellow = 1, Blue = -1
float S = 40;                                                   // General movement speed
float FinalSpeed = 10;                                          // End movement speed
int DefaultAcceleration = 1;                                    // Sets default acceleration value
float TS = 40.0;                                             // Turn speed, turn speed for each wheel dependent on team colour
float ArcMulti = 2;                                           // speed multiplier for arc turns
float limit = 0.3;                                            // sets the minimum error the encoders can be different from the desired distance before the distance correction function activates
int CorrectionSpeed = 3;                                      // sets the speed the the distcorrect function uses for each wheel
float oscf = 1;                                               // sets an overshoot correction factor which reduces any movement by this many CM's
float vsco = 0.5;                                             // "variable speed cut-off" percentage of distance travelled before variable speed starts
int CorrectionIteration = 1;                                  // sets the number of times the correction code will be used.

// Other Global Variables
float Last_Distance_1 = 0;                                    // The most recent Wheel 1 travel distance variable used for the 'correction' function, updated in the movement function, set as a global variable so it can be called across functions
float Last_Distance_2 = 0;                                    // The most recent Wheel 2 travel distance variable used for the 'correction' function, updated in the movement function
float Correction_dist1 = 0;
float Correction_dist2 = 0;
int CorrectionCount = 0;


// Prototype Functions
void movement (int SpeedValue, float TurnAngle, float Distance = 0, int Acceleration = DefaultAcceleration, boolean VariableSpeed = true, boolean Correction = false);          // sets the default inputs for the movement function so that specifically 'VariableSpeed' is set to 'true'
                                                                                                                                // when 'movement' is called in the loop before it is defined

// Main code
void setup(){
  pinMode(LEDPin, OUTPUT);                                    // Sets pin 3 to output for LED
  pinMode(buttonPin, INPUT);                                  // Sets pin 4 to input for start button

  myservo.attach(9);                                          // attaches the servo on pin 9 to the servo object
  myservo.write(2);                                           // Start position set to 2 degrees
  
  Wire.begin();                                               // Begin I2C bus
  Serial.begin(9600);                                         // Begin serial
  delay(100);                                                 // Wait for everything to power up

  Wire.beginTransmission(MD25ADDRESS);                        // Set MD25 operation MODE
  Wire.write(MODE_SELECTOR);
  Wire.write(Mode);                                           
  Wire.endTransmission();

  encodeReset();                                              // Calls a function that resets the encoder values to 0 
}


// Path
void loop(){
  
  delay(200);

  encodeReset();                                              // Reset encoder values to 0

/*
  // Test route
  movement(S, 0, -80);
  movement(TS, -30);
  movement(S, 0, -50);
  movement(TS, 75);
  movement(S, 0, 135);
  movement(S, 0, -135);
  movement(TS, -75);
  movement(S,0,50);
  movement(TS,30);
  movement(S, 0, 80);
  exit(0);
*/

  // Demonstration Route
  movement(S, 0, -70);
  movement(TS, -30);
  movement(S, 0, -70);
  movement(TS, 103.58);
  movement(S, 0, 20);
  movement(TS, -35);
  movement(S, 0, 120);
  movement(TS, 6);
  movement(S, 0, -85.636);
  movement(TS, -130);
  movement(S, 0, 59.025);
  /*
  movement(TS, 55.42);
  movement(S, 0, 20.142);
  movement(TS, 30);
  movement(S, 0, 70);
  */
  exit(0);
}


void start(){
  if (digitalRead(buttonPin) == LOW){
    LEDflash();
    LEDflash();
    }
  else {start();}
}


void movement (int SpeedValue, float TurnAngle, float Distance, int Acceleration, boolean VariableSpeed, boolean Correction){                                               // This function moves the platform with a set speed and turn value for each wheel until they have moved the defined distance set in cm
                                                                                                                                                              // If VariableSpeed is true (default) then the encoders will slow as a proportion of the distance moved, otherwise speed is constant

  int Speed1 = 0;                                                         // Creates a speed variable (SPEED1)
  int Speed2 = 0;                                                          // Creates a turn variable (SPEED2)
  float e1 = encoder1();                                                 // Calls a function that reads value of encoder 1
  float e2 = encoder2();                                                 // Calls a function that reads value of encoder 2

  float Distance_1_CM;
  float Distance_2_CM;

  TurnAngle = TurnAngle*Team;

  if(Correction == true){                                                       // setting motor speeds and distances if the movement is a correction
    int Sign1 = Correction_dist1/abs(Correction_dist1);                                     // calulates the sign of D1 to determine whether the robot under or over moved
    int Sign2 = Correction_dist2/abs(Correction_dist2);                                     // calulates the sign of D2 to determine whether the robot under or over moved
    Speed1 = Sign1*SpeedValue;
    Speed2 = Sign2*SpeedValue;
    Distance_1_CM = Correction_dist1 + Sign1*oscf;
    Distance_2_CM = Correction_dist2 + Sign2*oscf;
    Serial.println(Distance_1_CM);
    Serial.println(Distance_2_CM);
    Serial.println(Speed1);
    Serial.println(Speed2);
  }
  
  else if(TurnAngle == 0){                                                // setting motor speeds and distances for a straight line movement
    Distance_1_CM = Distance_2_CM = Distance;
    Speed1 = Speed2 = SpeedValue * Distance/abs(Distance);
    Serial.println(Distance_1_CM);
    Serial.println(Distance_2_CM);
    Serial.println(Speed1);
    Serial.println(Speed2);
  }
  
  else{                                                                     // setting motor speeds and distances for a turn on the spot
    Distance_1_CM = (WheelBaseDiameter/2) * TurnAngle * (pi/180.0);
    Distance_2_CM = (WheelBaseDiameter/2) * TurnAngle * (-pi/180.0);
    Speed1 = SpeedValue * (TurnAngle/abs(TurnAngle));
    Speed2 = SpeedValue * (-TurnAngle/abs(TurnAngle));
    Serial.println(Distance_1_CM);
    Serial.println(Distance_2_CM);
    Serial.println(Speed1);
    Serial.println(Speed2);
  }
  
  Last_Distance_1 = Distance_1_CM;                                       // Sets the distance for wheel 1 as a global variable so it can be used by the DistCorrection function
  Last_Distance_2 = Distance_2_CM;                                       // Sets the distance for wheel 2 as a global variable so it can be used by the DistCorrection function
  
  while(abs(e1) < abs(Distance_1_CM)- oscf || abs(e2) < abs(Distance_2_CM)-oscf){     // If statement to check the status of the traveled distance, absolutes used incase the motors are in reverse

    e1 = encoder1();                                                     // Updates encoder 1 reading
    e2 = encoder2();                                                     // Updates encoder 2 reading

    if(abs(e1) >= abs(Distance_1_CM)-oscf){Speed1 = 0;}
    
    else if(VariableSpeed == true && (e1/Distance_1_CM) > vsco){                                           // Checks whether speed needs to be varibale or constant
      float c1 = (FinalSpeed-SpeedValue)/(pow((vsco-1), 2));
      float c2 = c1*2*(pow(vsco, 2)-vsco)/(1-vsco);
      float c3 = FinalSpeed - c1 - c2;
      float PT1 = e1/Distance_1_CM;
      Speed1 =  (c1*pow(PT1, 2) + c2*PT1 + c3) * (Speed1/abs(Speed1));
    }

    if(abs(e2) >= abs(Distance_2_CM)-oscf){Speed2 = 0;}
    
    else if(VariableSpeed == true && (e2/Distance_2_CM) > vsco){
      float c1 = (FinalSpeed-SpeedValue)/(pow((vsco-1), 2));
      float c2 = c1*2*(pow(vsco, 2)-vsco)/(1-vsco);
      float c3 = FinalSpeed - c1 - c2;
      float PT2 = e2/Distance_2_CM;
      Speed2 =  (c1*pow(PT2, 2) + c2*PT2 + c3) * (Speed2/abs(Speed2));
    }
    
    Wire.beginTransmission(MD25ADDRESS);                      // Sets the acceleration to register 1
    Wire.write(ACCELERATION);
    Wire.write(Acceleration);
    Wire.endTransmission();

    Wire.beginTransmission(MD25ADDRESS);                      // Sets a combined motor speed value
    Wire.write(SPEED1);
    Wire.write(Speed1);
    Wire.endTransmission();
    
    Wire.beginTransmission(MD25ADDRESS);                      //Sets a turning motor speed value
    Wire.write(SPEED2);
    Wire.write(Speed2);
    Wire.endTransmission();

    e1 = encoder1();                                          // Updates encoder 1 reading
    e2 = encoder2();                                          // Updates encoder 2 reading

    Serial.print("e1 \t");
    Serial.print(e1);
    Serial.print("\t e2 \t");
    Serial.println(e2);  
  }
  stopMotor();
  Serial.println("end");
  Serial.print("D1 \t");
  Serial.print(Distance_1_CM);
  Serial.print("\t D2 \t");
  Serial.println(Distance_2_CM);

  CorrectionCount += 1;
  Serial.println(CorrectionCount);
  
  if(CorrectionCount <= CorrectionIteration){
    DistCorrection();
  }
  CorrectionCount = 0;
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
  
  encodeReset();

  if (abs(Correction_dist1) >= limit || abs(Correction_dist2) >= limit){                                                     // Only corrects if the difference is larger enough as it will overcorrect if the distance is small
    movement(CorrectionSpeed, 0, 0, 1, false, true);
  }
  
  encodeReset();                                                                       // Resets the encoders to 0
}


void LEDflash(){
  digitalWrite(LEDPin, HIGH);                               // Sets the output pin for the LED to high
  delay(50);                                               // Holds LED high for a set time in ms
  digitalWrite(LEDPin, LOW);                                // sets the output pin the the LED to low
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
  Wire.write(7);
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

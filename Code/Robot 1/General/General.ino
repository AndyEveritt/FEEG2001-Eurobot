/*
2016/17 Eurobot Task

Code written by Andrew Everitt and no one else
*/

#include <Wire.h>                                             // Calls for I2C bus library
#include <Servo.h>                                            // calls for the servo library
#include <NewPing.h>                                          // calls for the ultrasonic sensor library

#define MD25ADDRESS         0x58                              // Address of the MD25
#define SPEED1              0x00                              // Byte to send speed to both motors for forward and backwards motion if operated in MODE 2 or 3 and Motor 1 Speed if in MODE 0 or 1
#define SPEED2              0x01                              // Byte to send speed for turn speed if operated in MODE 2 or 3 and Motor 2 Speed if in MODE 0 or 1
#define ENCODERONE          0x02                              // Byte to read motor encoder 1
#define ENCODERTWO          0x06                              // Byte to read motor encoder 2
#define ACCELERATION        0xE                               // Byte to define motor acceleration
#define CMD                 0x10                              // Byte to reset encoder values
#define MODE_SELECTOR       0xF                               // Byte to change between control MODES
#define MAX_DISTANCE 500                                      // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define TRIGGER_PIN1  7                                      // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN1     6                                      // Arduino pin tied to echo pin on the ultrasonic sensor.
#define TRIGGER_PIN2  12                                      // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN2     13                                      // Arduino pin tied to echo pin on the ultrasonic sensor.


Servo ServoLauncher;
Servo ServoSpinner;
NewPing sonarFront(TRIGGER_PIN1, ECHO_PIN1, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonarBack(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE);


// Inputs/Outputs
const int YellowTeam = 2;
const int BlueTeam = 3;
const int StartPin = 4;
const int TeamSwitch = 5;
const int LauncherPin = 8;
const int SpinnerArmPin = 9;
const int SpinnerPin = 10;


// Chasis Parameters & Constants
float WheelBaseDiameter = 28.7;                               // sets the distance between the wheels in cm
float WheelDiameter = 10.2;                                  // sets the diameter of the wheel
const float pi = 3.14159265359;                               // constant variable roughly equal to pi


// Movement variables
int Mode = 1;                                                 // MODE in which the MD25 will operate selector value
int Team = 1;                                                 // Yellow = 1, Blue = -1
float S = 50;                                                   // General movement speed
float StartSpeed = 30;
float FinalSpeed = 10;                                          // End movement speed
int DefaultAcceleration = 1;                                    // Sets default acceleration value
float TS = 25.0;                                             // Turn speed, turn speed for each wheel dependent on team colour
float ArcMulti = 2;                                           // speed multiplier for arc turns
float limit = 0.3;                                            // sets the minimum error the encoders can be different from the desired distance before the distance correction function activates
int CorrectionSpeed = 7;                                      // sets the speed the the distcorrect function uses for each wheel
float OvershootDist = 6;                                               // sets an overshoot correction factor which reduces any movement by this many CM's
float ss = 0.3;
float vsco = 0.6;                                             // "variable speed cut-off" percentage of distance travelled before variable speed starts
int CorrectionIteration = 2;                                  // sets the number of times the correction code will be used.
boolean Sensors = true;                                       // true = ultrasonic sensors are enabled

int MinDistanceF = 60;
int MinDistanceB = 40;

// Other Global Variables
float Last_Distance_1 = 0;                                    // The most recent Wheel 1 travel distance variable used for the 'correction' function, updated in the movement function, set as a global variable so it can be called across functions
float Last_Distance_2 = 0;                                    // The most recent Wheel 2 travel distance variable used for the 'correction' function, updated in the movement function
float Last_Speed1;
float Last_Speed2;
float Correction_dist1 = 0;
float Correction_dist2 = 0;
int CorrectionCount = 0;
unsigned long StartMillis = 0;
unsigned long AvoidMillis;
const long Duration = 90000;
int ServoAngle;


// Prototype Functions
void movement (int SpeedValue, float TurnAngle, float Distance = 0, int Acceleration = DefaultAcceleration, boolean VariableSpeed = true, boolean Correction = false, boolean Avoidance = false);                    // sets the default inputs for the movement function so that specifically 'VariableSpeed' is set to 'true'                                                                                                                            // when 'movement' is called in the loop before it is defined
void SpinnerAngle(int Angle, int Speed = 1);

void setup() {
  Wire.begin();                                               // Begin I2C bus
  Serial.begin(9600);                                         // Begin serial
  delay(100);                                                 // Wait for everything to power up

  pinMode(SpinnerPin, OUTPUT);
  pinMode(StartPin, INPUT);
  pinMode(TeamSwitch, INPUT);
  pinMode(BlueTeam, OUTPUT);
  pinMode(YellowTeam, OUTPUT);
  ServoLauncher.attach(LauncherPin);
  ServoSpinner.attach(SpinnerArmPin);

  Wire.beginTransmission(MD25ADDRESS);                        // Set MD25 operation MODE
  Wire.write(MODE_SELECTOR);
  Wire.write(Mode);                                           
  Wire.endTransmission();

/*
  pinMode(TriggerPin, OUTPUT);
  digitalWrite(TriggerPin, LOW);
*/
  LauncherReset();
  SpinnerAngle(30);                                           // Spinner Start Angle
  delay(500);
  SpinnerAngle(160);                                           // Spinner Start Angle
  encodeReset();                                              // Calls a function that resets the encoder values to 0 
}
  

void loop() {
  // Main route for robot
  SpinnerAngle(160);
  TeamColourStart();                                          // Detects the team colour using a switch. Indicates the team using 2 LEDS. Waits for signal from start switch before proceeding

  encodeReset();                                              // Reset encoder values to 0

  Route();
}

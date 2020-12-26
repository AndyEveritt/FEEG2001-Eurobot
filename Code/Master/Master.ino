// Chapter 7 - Communications
// I2C Master
// By Cornel Amariei for Packt Publishing

// Include the required Wire library for I2C
#include <Wire.h>

int x = 1;

void setup() {
  // Start the I2C Bus as Master
  Wire.begin(); 
}
void pulse() {
  Wire.beginTransmission(9); // transmit to device #9
  Wire.write(x);              // sends x 
  Wire.endTransmission();    // stop transmitting
}
void loop() {
  
 pulse();
 delay(1000);
}


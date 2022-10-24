#include "Motor.h"
#include "UltrasonicSensor.h"
#include "IRSensor.h"

// Hyperparameters
int speed = 200;
const int IR_PINS[] = {8, 9, 10, 11, 12};
int Kp = speed * 0.4;
int MAX_SPEED = 255;
int mode;
int distToError = 5;
int returningSpeed = 120;
int threshold = 10;

/*
   MODE
   mode = 0: on-line and no obstacle
   mode = 1: off-line and obstacle
   mode = 2: off-line and finish
*/

// Declare the devices
Motor motor(4, 5, 7, 6, speed, Kp);
UltrasonicSensor usLeft(A0, A1);
UltrasonicSensor usRight(A2, A3);
UltrasonicSensor usSide(A4, A5);
IRSensor irSensor(IR_PINS);


/* ============================================================================================ */

void setup()
{
  Serial.begin(9600);
  Serial.println("Setting up ...");
  Serial.println("Done setup.");
  mode = 2;
}

void mode0() {
 if (usLeft.check() || usRight.check()) { // if detecting an obstacle
    mode = 1;
    return;
  }

  int error = irSensor.getError();
  if (abs(error) == 4) {
    motor.stop();
  } else {
    motor.pControl(error);
  }
}

void mode1() {
  
}

void mode2() {
  if (irSensor.countOnes() >= 1) {
    mode = 3;
    return;
  }
  
  // following object
  float diff = usSide.getDist() - threshold;
  float error = diff / threshold;
  motor.pControl(error); // TODO: put speed and Kp to motor attributes
}

void mode3() {
  
}

void mainloop() {
//  if (mode == 0) mode0();
//  if (mode == 1) mode1();
  if (mode == 2) mode2();
//  if (mode == 3) mode3(); 
}

void myPrint(char s[], float x) {
  Serial.print(s);
  Serial.print(" ");
  Serial.println(x);
}

void loop() {
  myPrint("mode", mode);
  myPrint("left dist", usLeft.getDist());
  myPrint("right dist", usRight.getDist());
  myPrint("side dist", usSide.getDist());
  mainloop();
}

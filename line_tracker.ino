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
int speedTurn = 100;
bool onLine = false;
int error;

/*
 * mode 0: on-line + no obs in the front + no obs in the side
 * mode 1: on-line + obs in the front -> off-line + obs not in the front + obs in the side
 * mode 2: off-line + obs on the side + obs not in the front -> on-line + obs on the side + obs not in the front
 * mode 3: on-line + obs on the side + obs not in the front -> error = 0 + no obs in the front + no obs on the side
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
  mode = 0;
}

void mode0() {
 if (usLeft.check() || usRight.check()) { // if detecting an obstacle
    mode = 1;
    return;
  }

  if (abs(error) == 4) {
    motor.stop();
  } else {
    motor.pControl(error);
  }
}

void mode1() {
  if (error == 4 && !usRight.check() && !usLeft.check() && usSide.check()) {
    mode = 2;
    onLine = false;
    return;
  }
    motor.turnLeft(speed);
}

void mode2() {
  if (usLeft.check() || usRight.check()) { // if detecting an obstacle
    mode = 1; // return to mode 1 -> avoid it
    return;
  }
  if (irSensor.countOnes() >= 2){
    onLine = true;
  }
  if (onLine == true && error == 4) {
    mode = 3;
    return;
  }
  
  // following object
  float diff = usSide.getDist() - threshold;
  if (diff > 50) diff = 0;
  float error_2 = diff / threshold;
  motor.pControl(error_2); // TODO: put speed and Kp to motor attributes
}

void mode3() {
  if (abs(error) <= 1 && !usRight.check() && !usLeft.check() && !usSide.check()) {
    mode = 0;
    motor.stop();
    return;
  }
    motor.turnLeft(speed);
}

void mainloop() {
  error = irSensor.getError();
  if (mode == 0) mode0();
  if (mode == 1) mode1();
  if (mode == 2) mode2();
  if (mode == 3) mode3(); 
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

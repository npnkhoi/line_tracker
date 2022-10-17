#include "Motor.h"
#include "UltrasonicSensor.h"
#include "IRSensor.h"

// Hyperparameters
int speed = 150;
const int IR_PINS[] = {8, 9, 10, 11, 12};
int Kp = 75;
int MAX_SPEED = 255;
int mode = 0;

/*
 * MODE
 * mode = 0: on-line and no obstacle
 * mode = 1: off-line and obstacle
 * mode = 2: off-line and finish
 */

// Declare the devices
Motor motor(4, 5, 7, 6);
UltrasonicSensor usLeft(A0, A1);
UltrasonicSensor usRight(A2, A3);
UltrasonicSensor usSide(A4, A5);
IRSensor irSensor(IR_PINS);


/* ============================================================================================ */

void setup()
{
  Serial.begin(9600);
  // declare the line-tracking sensors
}

void followLine() {
  int error = irSensor.getError();
  Serial.println(error);
  int delta = Kp*error;
  Serial.println(delta);
  int speedLeft = speed - delta;
  int speedRight = speed + delta;
  if (error != 0) {
     if (error != 4)  {//inline
         motor.go(speedLeft, speedRight);
     } else {//outline
         mode = 2;
         }
     }
  else {
     motor.go(speed, speed); //go straight
     }
  delay(10);
}

void midterm() {
  if (mode == 0 && (usLeft.check() || usRight.check()) ) { //if on-line and detect obstacle
    mode = 1;
    while ((usLeft.check()) & (usRight.check())) { //assuming side sensor on the right
      motor.turnRight(speed);
    }
  } else if ((mode == 1) & (!usLeft.check()) & (!usRight.check()) & (irSensor.getError() != 4)) { 
    mode = 0;
  }
  Serial.println(mode);
  if (mode == 0) {
      followLine();
  } else if (mode == 1) { 
    while (!usLeft.check()) {
      motor.go(speed - Kp, speed + Kp);
    }
    while (usLeft.check()) {
      motor.go(speed + Kp, speed - Kp);
    }
    motor.stop();
  } 
  Serial.print("Left: ");
  Serial.println(usLeft.getDist());
  Serial.print("Right: ");
  Serial.println(usRight.getDist());
  Serial.print("Side: ");
  Serial.println(usSide.getDist());
  delay(1000);
  Serial.println("---------------");  
}

void loop()
{ 
  followLine();
  irSensor.trackLine();
  for (int i = 0; i < 5; ++ i) {
    Serial.print(irSensor.irVal[i]);
  }
  Serial.println();
}  
  

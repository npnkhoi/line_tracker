#include "Motor.h"
#include "UltrasonicSensor.h"
#include "IRSensor.h"

// Hyperparameters
int speed = 200;
const int IR_PINS[] = {8, 9, 10, 11, 12};
int Kp = 75; //thay đổi theo 
int MAX_SPEED = 255;
//int mode[] = {0,1,2};

/*
 * MODE
 * mode = 0: on-line and no obstacle
 * mode = 1: off-line and obstacle
 * mode = 2: off-line and finish
 */

// Declare the devices
Motor motor(4, 5, 7, 6);
UltrasonicSensor usLeft(A0, A1);
UltrasonicSensor usMiddle(A2, A3);
UltrasonicSensor usRight(A4, A5);
IRSensor irSensor(IR_PINS);


/* ============================================================================================ */

void setup()
{
  Serial.begin(9600);
  int mode = 0;
  // declare the line-tracking sensors
}
 
void loop()
{ 
  //Test IR middle
  /*
  if ((mode == 0) & ((usLeft.check()) || (usRight.check())) { //if on-line and detect obstacle
    mode = 1;
    while ((usLeft.check()) & (usRight.check())) { //assuming side sensor on the right
       motor.turnRight();
    }
  } else if ((mode == 1) & (!usLeft.check()) & (!usRight.check()) & (irSenSor.getError != 4)) { 
    mode = 0;
  }

  if (mode == 0) {
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

  } else if (mode == 1) { 
    while (!usLeft.check()) {
      motor.go(speed - Kp, speed + Kp);
    }
    while (usLeft,check()) {
      motor.go(speed + Kp, speed - Kp);
    }
    motor.stop();
  } */
  Serial.print("Left: ");
  Serial.println(usLeft.getDist());
  Serial.print("Middle: ");
  Serial.println(usMiddle.getDist());
  Serial.print("Right: ");
  Serial.println(usRight.getDist());
  delay(1000);
  Serial.println("---------------");
}  
  

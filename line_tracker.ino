#include "Motor.h"
#include "UltrasonicSensor.h"
#include "IRSensor.h"

// Hyperparameters
int speed = 200;
const int IR_PINS[] = {8, 9, 10, 11, 12};
<<<<<<< Updated upstream
int Kp = 75; //thay đổi theo 
=======
int Kp = speed*0.4;
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
 
void loop()
{ 
  if ((mode == 0) & ((usLeft.check()) || (usRight.check())) { //if on-line and detect obstacle
=======

void followLine() {
  int error = irSensor.getError();
  Serial.println(error);
  if (error != 0) {
     if (abs(error) != 4)  {//inline
        int delta = Kp*error;
        int speedLeft = speed + delta;
        int speedRight = speed - delta;
        Serial.print("Speed: ");
        Serial.print(speedLeft);
        Serial.print(" ");
        Serial.println(speedRight);
        motor.go(speedRight, speedLeft);
        if (delta < 0) {
          Serial.println("Turn left");
        } else {
          Serial.println("Turn right");
        }
     } else {//outline
         motor.stop();
         Serial.println("Stop");
         }
     }
  else {
     motor.go(speed, speed);
     Serial.println("Go straight"); //go straight
     }
  delay(10);
}

void avoid() {

//  Serial.print("Side: ");
//  Serial.println(usSide.getDist());
//  Serial.println("---------------"); 
  if (mode == 0 && (usLeft.check() || usRight.check()) ) { //if on-line and detect obstacle
>>>>>>> Stashed changes
    mode = 1;
    if (usLeft.check()) {
      while (usLeft.check()) { //assuming side sensor on the right
        motor.turnRight(speed);
      }
    } else {
      while (usRight.check()) {
        motor.turnLeft(speed);
      }
    } 
    motor.stop();
    delay(10000);
  } else if ((mode == 1) && (!usLeft.check()) && (!usRight.check()) && (irSensor.getError() != 4)) { 
    mode = 0;
  }
<<<<<<< Updated upstream

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
=======
  Serial.print("Mode: ");
  Serial.println(mode);
  if (mode == 0) {
      followLine();
  } 
  else if (mode == 1) { 
    while (usRight.check()) {
      Serial.println("Avoiding right");
      motor.go(speed + Kp, speed - Kp);
      Serial.print("Left: ");
      Serial.println(usLeft.getDist());
      Serial.print("Right: ");
      Serial.println(usRight.getDist());
>>>>>>> Stashed changes
    }
    while (usLeft.check()) {
      motor.go(speed - Kp, speed + Kp);
      Serial.println("Avoiding left");
      Serial.print("Left: ");
      Serial.println(usLeft.getDist());
      Serial.print("Right: ");
      Serial.println(usRight.getDist());
    }
    motor.stop();
  } 
<<<<<<< Updated upstream
  Serial.print("Left: ");
  Serial.println(usLeft.getDist());
  Serial.print("Right: ");
  Serial.println(usRight.getDist());
  Serial.print("Side: ");
  Serial.println(usSide.getDist());
  delay(1000);
  Serial.println("---------------");
=======
}

void loop() { 
  avoid();
//  followLine();
  
//  for (int i = 0; i < 5; ++ i) {
//    Serial.print(irSensor.irVal[i]);
//  }
//  Serial.println();
>>>>>>> Stashed changes
}  
  

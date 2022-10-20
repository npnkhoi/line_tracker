#include "Motor.h"
#include "UltrasonicSensor.h"
#include "IRSensor.h"

// Hyperparameters
int speed = 200;
const int IR_PINS[] = {8, 9, 10, 11, 12};
int Kp = speed*0.4;
int MAX_SPEED = 255;
int mode = 0;
int distToError = 10;
int returningSpeed = 120;
int threshold = 20 ;

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
  Serial.println("Setting up ...");
  Serial.println("Done setup.");

}

void followLine() {
  int error = irSensor.getError();
  Serial.println(error);
  if (error != 0) {
     if (abs(error) != 4)  { //inline
        motor.pControl(error, speed, Kp);
     } else {//outline
         motor.stop();
         Serial.println("Stop");
         }
     }
  else {
     motor.pControl(0, speed, Kp);
     }
  delay(10);
}

void midterm() {

//  Serial.print("Side: ");
//  Serial.println(usSide.getDist());
//  Serial.println("---------------"); 
  if (mode == 0 && (usLeft.check() || usRight.check()) ) { // if on-line and detecting an obstacle
    mode = 1;
    Serial.println("Obstacle found");
    Serial.print("distances: ");
    Serial.print(usLeft.getDist());
    Serial.print(" ");
    Serial.print(usRight.getDist());
    Serial.print(" ");
    Serial.print(usSide.getDist());
    Serial.println();

    // turning to avoid obstacle
    while ( (usLeft.check() || usRight.check()) || !usSide.check()) {
      motor.pControl(-3, speed, Kp);
    }
    motor.stop();
    Serial.print("side dist: ");
    Serial.println(usSide.getDist());
    
    motor.stop();
    
  } else if ((mode == 1) && (!usLeft.check()) && (!usRight.check()) && irSensor.countOnes() >= 3) {
    // returning to line
    Serial.println("Switching from 1 to 0");
    Serial.print("ir error: ");
    Serial.println(irSensor.getError());

    // get out of the line
    while (irSensor.getError() == 4) {
      motor.go(speed, speed);
    }
    motor.stop();
    
    while (irSensor.countOnes() < 2)  {
      Serial.println("Turning back to the line");
      motor.pControl(-3, speed, Kp); // TODO: rename to rotate, remove magic value
    }
    
    motor.stop();    // if done avoiding obstacle 
    mode = 0;
  }
  
  Serial.print("Mode: ");
  Serial.println(mode);





  
  
  if (mode == 0) {
      followLine();
  } 
  else if (mode == 1) {
    int error = min(3, (int) floor((usSide.getDist() - threshold) / distToError));
    Serial.print("turning back - error: ");
    Serial.println(error);
    motor.pControl(error, speed, Kp); // TODO: put speed and Kp to motor attributes
  } 
}

void loop() { 
  midterm();
//  followLine();
}  
  

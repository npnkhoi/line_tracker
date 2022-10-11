#include "Motor.h"
#include "UltrasonicSensor.h"
#include "IRSensor.h"

// Hyperparameters
int speed = 200;
const int IR_PINS[] = {8, 9, 10, 11, 12};
int Kp = 75; //thay đổi theo 
int MAX_SPEED = 255;

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

  // declare the line-tracking sensors
}

 
void loop()
{   
  irSensor.trackLine();

  // Test G algorithm 
//  Serial.print(irSensor.irVal[0]);
//  Serial.print(irSensor.irVal[1]);
//  Serial.print(irSensor.irVal[2]);
//  Serial.print(irSensor.irVal[3]);
//  Serial.println(irSensor.irVal[4]);
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
         motor.stop();
         }
     }
  else {
     motor.go(speed, speed); //go straight
     }
  delay(10);
}  
  
//  if (irSensor.irVal[2] == 0) {
//    if (irSensor.irVal[0] || irSensor.irVal[1]) {
//      motor.turnLeft(speed);
//      Serial.println("Turning left");
//    } else if (irSensor.irVal[3] || irSensor.irVal[4]) {
//      motor.turnRight(speed);
//      Serial.println("Turning right");
//    } else {
//      Serial.println("Completely lost");
//      motor.turnRight(speed);
//    }
//  } else {
//    motor.goStraight(speed);
//    Serial.println("Going straight");
//  }          

//  Serial.print("Left: ");
//  Serial.println(usLeft.getDist());
//  Serial.print("Middle: ");
//  Serial.println(usMiddle.getDist());
//  Serial.print("Right: ");
//  Serial.println(usRight.getDist());
//  delay(100);
  
//  Serial.println(usRight.getDist());

#include "Motor.h"
#include "UltrasonicSensor.h"
#include "IRSensor.h"

// Hyperparameters
int speed = 110;
const int IR_PINS[] = {8, 9, 10, 11, 12};

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
//  irSensor.trackLine();
//  
//  if (irSensor.irVal[2] == 0) {
//    if (irSensor.irVal[0] || irSensor.irVal[1]) {
//      motor.turnLeft(speed);
//      Serial.println("Turning left");
//    } else if (irSensor.irVal[3] || irSensor.irVal[4]) {
//      motor.turnRight(speed);
//      Serial.println("Turning right");
//    } else {
//      Serial.println("Completely lost");
//      motor.stop();
//    }
//  } else {
//    motor.goStraight(speed);
//    Serial.println("Going straight");
//  }          
//  delay(500);
  Serial.print("Left: ");
  Serial.println(usLeft.getDist());
  Serial.print("Middle: ");
  Serial.println(usMiddle.getDist());
  Serial.print("Right: ");
  Serial.println(usRight.getDist());
  delay(100);
  
//  Serial.println(usRight.getDist());
}

#include "Motor.h"
#include "UltrasonicSensor.h"
#include "IRSensor.h"

// Hyperparameters
int speed = 150;
const int IR_PINS[] = {8, 9, 10, 11, 12};

// Declare the devices
Motor motor(4, 5, 7, 6);
UltrasonicSensor usRight(A0, A1);
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
  
  if (irSensor.irVal[2] == 0) {
    if (irSensor.irVal[0] || irSensor.irVal[1]) {
      motor.turnLeft(speed);
    } else if (irSensor.irVal[3] || irSensor.irVal[4]) {
      motor.turnRight(speed);
    }
  } else {
    motor.goStraight(speed);
  }          
  
  Serial.println(usRight.getDist());
}

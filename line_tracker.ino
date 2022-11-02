#include "Motor.h"
#include "UltrasonicSensor.h"
#include "IRSensor.h"
#include "Encoder.h"
#include "Display.h"

// Hyperparameters
int speed = 150;
const int IR_PINS[] = {8, 9, 10, 11, 12};
int Kp = speed * 0.3;
int MAX_SPEED = 255;
int mode;
int returningSpeed = 120;
int threshold = 10;
int speedTurn = 100;
bool onLine = false;
int error;
const int displayPins[] = {A0, A1, 13};

/*
 * mode 0: on-line + no obs in the front + no obs in the side
 * mode 1: on-line + obs in the front -> off-line + obs not in the front + obs in the side
 * mode 2: off-line + obs on the side + obs not in the front -> on-line + obs on the side + obs not in the front
 * mode 3: on-line + obs on the side + obs not in the front -> error = 0 + no obs in the front + no obs on the side
 * mode 4: mode 0 + all IRsensors on-line
 */

// Declare the devices
Motor motor(4, 5, 7, 6, speed, Kp);
//UltrasonicSensor usLeft(A0, A1);
UltrasonicSensor usFront(A2, A3);
UltrasonicSensor usSide(A4, A5);
IRSensor irSensor(IR_PINS);
Encoder encoder(3,2);
Display screen(displayPins);


/* ============================================================================================ */

void setup()
{
    
  Serial.begin(9600);
  Serial.println("Setting up ...");
  Serial.println("Done setup.");
  mode = 0;
}

void mode0() {
 if (usFront.check()) { // if detecting an obstacle
    mode = 1;
    return;
  }
 if ((irSensor.irVal[0] == 1) && (irSensor.irVal[1] == 1) && (irSensor.irVal[2] == 1) && (irSensor.irVal[3] == 1) && (irSensor.irVal[4] == 1)) {
    mode = 4;
    return; 
  }

  if (abs(error) == 4) {
    motor.stop();
  } else {
    motor.pControl(error);
  }
}

void mode1() {
  if (!usFront.check() && usSide.check()) {
    mode = 2;
    onLine = false;
    return;
  }
//    delay(100);
    motor.turnLeft(speed);
}

void mode2() {
  if (usFront.check()) { // if detecting an obstacle
    mode = 1; // return to mode 1 -> avoid it
    return;
  }
  if (irSensor.countOnes() >= 2){
    onLine = true;
  }
  if (onLine == true && error == 4) {
//    motor.motor_left_Lui(speed);
//    motor.motor_right_Lui(speed);
//    delay(50);
    motor.stop();
    mode = 3;
    return;
  }
  
  // following object
  float diff = usSide.getDist() - threshold;
  if (diff > 50) {
//    motor.turnRight(speed);
    motor.go(speed, speed);
    return;
  } else {
    float error_2 = diff / threshold;
    motor.pControl(error_2); // TODO: put speed and Kp to motor attributes  
  }
  
}

void mode3() {
  if (abs(error) <= 3 && !usFront.check() && !usSide.check()) {
    motor.stop();
    mode = 0;
    return;
  }
    motor.turnLeft(returningSpeed);
}

void mode4() {
  motor.turnLeft(speed);
//  delay(240);
//  motor.go(255, 255);
//  delay(3600);
//  motor.turnLeft(MAX_SPEED);
//  delay(150);
//  motor.go(255,255);
//  delay(500);
//  motor.stop();
//  delay(5000);
}

void updateLight() {
  
}

void mainloop() {
  error = irSensor.getError(); // very important!!
  Serial.print("mode: ");
  Serial.println(mode);
  Serial.print("usFront");
  Serial.println(usFront.getDist());
  Serial.print("usSide");
  Serial.println(usSide.getDist());
//  mode0();
  screen.showMode(mode);
  if (mode == 0) mode0();
  else if (mode == 1) mode1();
  else if (mode == 2) mode2();
  else if (mode == 3) mode3();
  else if (mode == 4) mode4(); 
  
}

void myPrint(char *s, float x) {
  Serial.print(s);
  Serial.print(" ");
  Serial.println(x);
}

void loop() {
  mainloop();
}

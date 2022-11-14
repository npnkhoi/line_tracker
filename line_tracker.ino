#include "Motor.h"
#include "UltrasonicSensor.h"
#include "IRSensor.h"
#include "Encoder.h"
#include "Display.h"

// Hyperparameters
const int IR_PINS[] = {8, 9, 10, 11, 12};
const int displayPins[] = {A0, A1, 13};
int MAX_SPEED = 255;
int speed = 180;
const int SPEED_RATIO = 1;
const float speed_dynamic = (225/199)*1.00;

//1: old line, 2: new line
const int lim_1[] = {13,192,10,60};
const int lim_run_1[] = {0, lim_1[0], lim_1[0] + lim_1[1],lim_1[0] + lim_1[1] + lim_1[2], lim_1[0] + lim_1[1] + lim_1[2] + lim_1[3], 500};
const int lim_2[] = {9,173,5,60};
const int lim_run_2[] = {0, lim_2[0], lim_2[0] + lim_2[1],lim_2[0] + lim_2[1] + lim_1[2], lim_1[0] + lim_1[1] + lim_1[2] + lim_1[3], 500};

//const float left_dynamic_1[] = {-MAX_SPEED, MAX_SPEED,-MAX_SPEED, MAX_SPEED, 0};
//const int right_dynamic_1[] = {MAX_SPEED, MAX_SPEED,MAX_SPEED, MAX_SPEED, 0};

const float left_dynamic_1[] = {-200, 240,-200, 240, 0};
const int right_dynamic_1[] = {200, 240,200, 240, 0};

const int left_dynamic_2[] = {MAX_SPEED, MAX_SPEED,MAX_SPEED, MAX_SPEED, 0};
const int right_dynamic_2[] = {(-MAX_SPEED), MAX_SPEED,(-MAX_SPEED), MAX_SPEED, 0};

int returningSpeed = 120;
int threshold = 10;
int speedTurn = 100;
int i_mode4 = 0;
float kp_line = 60;
int mode;
bool onLine = false;
float error = 0, prevError = 0;

/*
 * mode 0: on-line + no obs in the front + no obs in the side
 * mode 1: on-line + obs in the front -> off-line + obs not in the front + obs in the side
 * mode 2: off-line + obs on the side + obs not in the front -> on-line + obs on the side + obs not in the front
 * mode 3: on-line + obs on the side + obs not in the front -> error = 0 + no obs in the front + no obs on the side
 * mode 4: mode 0 + all IRsensors on-line
 */

// Declare the devices
Motor motor(4, 5, 7, 6, speed, kp_line);
UltrasonicSensor usFront(A2, A3);
UltrasonicSensor usSide(A4, A5);
IRSensor irSensor(IR_PINS);
Encoder encoder(3,2);
Display screen(displayPins);



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
// if (irSensor.countOnes() >= 4){
 if ((irSensor.irVal[0] == 1) && (irSensor.irVal[1] == 1) && (irSensor.irVal[2] == 1) && (irSensor.irVal[3] == 1) && (irSensor.irVal[4] == 1)) {
    encoder.resetCounter();
    mode = 4;
    return; 
  }

  if (abs(error) == 4) {
    error = prevError;
    motor.pControl(error);
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
  if (onLine == true && irSensor.countOnes() <= 1) {
    motor.stop();
    mode = 3;
    return;
  }
  
  // following object
  float diff = usSide.getDist() - threshold;
  if (diff > 50) {
    motor.pControl(2.75);
//    motor.go(speed, speed);
    return;
  } else {
    float error_2 = diff / threshold;
    motor.pControl(error_2); // TODO: put speed and Kp to motor attributes 
    return; 
  }
  
}

void mode3() {
  if (abs(error) <= 1 && !usFront.check() && !usSide.check()) {
    motor.stop();
    mode = 0;
    return;
  }
    motor.turnLeft(returningSpeed);
}

void mode4() {
  if (usFront.check()) { // if detecting an obstacle
    mode = 1;
    return;
  }
  encoder.getCounter(1);
  if (encoder.rightCounter >= lim_run_1[i_mode4]) {
      motor.go(right_dynamic_1[i_mode4], left_dynamic_1[i_mode4]);
      i_mode4++;
      
  }
}

void mainloop() {
  if (error != 0) prevError = error;
  error = irSensor.getError(); // very important!!
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
//  myPrint("mode", mode);
//  myPrint("left dist", usLeft.getDist());
//  myPrint("right dist", usRight.getDist());
//  myPrint("side dist", usSide.getDist());
mainloop();
}

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
//const int lim[] = {0, 177, 300};
const int lim_new[] = {0, 112, (112 + 16), (112 + 16 +200), 500};
const float left_dynamic[] = {MAX_SPEED, -MAX_SPEED, MAX_SPEED, 0};
const int right_dynamic[] = {MAX_SPEED, MAX_SPEED, MAX_SPEED, 0};
int returningSpeed = 120;
int threshold = 10;
int speedTurn = 100;
int i_mode4 = 0;
float kp_line = 52;
float kp = 40, ki = 4, kd = 150;
// const int left_speed[] = {-speed/SPEED_RATIO, speed/SPEED_RATIO, -speed/SPEED_RATIO, speed/SPEED_RATIO, 0};
// const int right_speed[] = {speed, speed, speed, speed, 0};

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
}

void mode0() {
 if (usFront.check()) { // if detecting an obstacle
    mode = 1;
    return;
  }
 if ((irSensor.irVal[0] == 1) && (irSensor.irVal[1] == 1) && (irSensor.irVal[2] == 1) && (irSensor.irVal[3] == 1) && (irSensor.irVal[4] == 1)) {
   encoder.resetCounter();
    mode = 4;
    return; 
  }

  if (abs(error) == 4) {
    motor.stop();
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
    motor.turnLeft(speed);
    Serial.print("Front:");
    Serial.println(usFront.getDist());
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
    motor.go(speed, speed);
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
  if (encoder.rightCounter >= lim_new[i_mode4]) {
      motor.go(right_dynamic[i_mode4], left_dynamic[i_mode4]);
      i_mode4++;
  }
}

void mainloop() {
  prevError = error;
  error = irSensor.getError(); // very important!!
  screen.showMode(mode);
  if (mode == 0) mode0();
  else if (mode == 1) mode1();
  else if (mode == 2) mode2();
  else if (mode == 3) mode3();
  else if (mode == 4) mode4(); 
  
}

void loop() {
    mainloop();
}

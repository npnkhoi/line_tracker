#include "Motor.h"
#include "UltrasonicSensor.h"
#include "IRSensor.h"
#include "Encoder.h"
#include "Display.h"
#define print_(a, b) Serial.print(a); Serial.println(b);

// Hyperparameters
const int IR_PINS[] = {8, 9, 10, 11, 12};
const int displayPins[] = {A0, A1, 13};
int MAX_SPEED = 255;
int baseSpeed = 180;
const int SPEED_RATIO = 1;
const float speed_dynamic = (225/199)*1.00;
const int lim[] = {0, 400, 70, 10000};
const float left_dynamic[] = {129, 00, 0};
const float right_dynamic[] = {255, 0, 0};

const float k_line[] = {40, 4, 150};
const float k_finish[] = {400, 5000, 0};

const int returningSpeed = 120;
const int threshold = 10;
const int speedTurn = 100;
// const float kp_line = 52;
// const float kp = 1, ki = 0.1, kd = 0.1;
// const int left_speed[] = {-speed/SPEED_RATIO, speed/SPEED_RATIO, -speed/SPEED_RATIO, speed/SPEED_RATIO, 0};
// const int right_speed[] = {speed, speed, speed, speed, 0};

int mode, i_mode4;
bool onLine = false;
float error = 0, prevError = 0;

float err_line[] = {0, 0, 0};
float err_finish[] = {0, 0, 0};
float kp_line = 52;

/*
 * mode 0: on-line + no obs in the front + no obs in the side
 * mode 1: on-line + obs in the front -> off-line + obs not in the front + obs in the side
 * mode 2: off-line + obs on the side + obs not in the front -> on-line + obs on the side + obs not in the front
 * mode 3: on-line + obs on the side + obs not in the front -> error = 0 + no obs in the front + no obs on the side
 * mode 4: mode 0 + all IRsensors on-line
 */

// Declare the devices
Motor motor(4, 5, 7, 6, baseSpeed, kp_line);
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
  i_mode4 = 0;
}

void mode0() {
  // Transition 0 -> 1
  if (usFront.check()) { // if detecting an obstacle
    mode = 1;
    return;
  }
  // Transition 0 -> 4
  if (error == -4) {
    encoder.resetCounter();
    mode = 4;
    return; 
  }

  // During mode 0 ...
  // Condition: error != 4 && !usFront.check()
  

  if (abs(error) == 4) { // out, lost!!
    error = prevError; // do not allow :)
  }
  
  // error > 0: 
  float delta = motor.pid(error, 0, k_line[0], k_line[1], k_line[2], err_line[0], err_line[1], err_line[2]); // TODO: tune this
  float speedLeft = baseSpeed + delta;
  float speedRight = baseSpeed - delta;
  // print_("speed left: ", speedLeft);
  // print_("speed right: ", speedRight);
  motor.go(speedRight, speedLeft);
}

void mode1() {
  if (!usFront.check() && usSide.check()) {
    mode = 2;
    onLine = false;
    return;
  }
//    delay(100);
    motor.turnLeft(baseSpeed);
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
    motor.go(baseSpeed, baseSpeed);
    return;
  } else {
    float error_2 = diff / threshold;
    motor.pControl(error_2, kp_line); // TODO: put speed and Kp to motor attributes  
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
  // Condition: encoder.leftCounter and rightCounter have been set to 0 before switching mode from 0 -> 4
  if (irSensor.countOnes() >= 2) {
    i_mode4 = 4;
    motor.stop();
  }
  
  if (encoder.rightCounter >= lim[i_mode4]) {
    err_finish[0] = err_finish[1] = err_finish[2] = 0;
    encoder.resetCounter();

    float defaultLeftPwm = left_dynamic[i_mode4]; // this will be adjusted dynamically
    float defaultRightPwm = right_dynamic[i_mode4]; // this PWM is fixed

    float expectedRatio = defaultLeftPwm/defaultRightPwm;
    float new_I = encoder.leftCounter - encoder.rightCounter * expectedRatio;
    float error = new_I - err_finish[1];
    print_("cnt left: ", encoder.leftCounter);
    print_("cnt right: ", encoder.rightCounter); 
    print_("error finish: ", error);
    
    float delta = motor.pid(error, 0, k_finish[0], k_finish[1], k_finish[2], err_finish[0], err_finish[1], err_finish[2]);
    delta = 0;
    motor.go(defaultRightPwm, defaultLeftPwm - delta);
    i_mode4++;
  }
}

void mainloop() {
  prevError = error;
  error = irSensor.getError(); // very important!!
//  Serial.print("mode: ");
//  Serial.println(mode);
//  Serial.print("usFront");
//  Serial.println(usFront.getDist());
//  Serial.print("usSide");
//  Serial.println(usSide.getDist());
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
#include "Arduino.h"
#include "Motor.h"

#define MAX_SPEED 255 //từ 0-255
#define MIN_SPEED 0

//TO DO: FIX BUG GO(RIGHT, LEFT) - EXPECTED = GO(LEFF, RIGHT) - REAL

Motor::Motor(int in1, int in2, int in3, int in4, int _baseSpeed, float _kp) {
  _in1 = in1;
  _in2 = in2;
  _in3 = in3;
  _in4 = in4;
  baseSpeed = _baseSpeed;
  kp = _kp;
  // init the pins
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void Motor::motor_right_Dung() {
  digitalWrite(this -> _in1, LOW);
  digitalWrite(this -> _in2, LOW);
}
 
void Motor::motor_left_Dung() {
  digitalWrite(this -> _in3, LOW);
  digitalWrite(this -> _in4, LOW);
}
 
void Motor::motor_right_Lui(int speed) { //speed: từ 0 - MAX_SPEED
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);//đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
  digitalWrite(this -> _in1, HIGH);// chân này không có PWM
  analogWrite(this -> _in2, 255 - speed);
}
 
void Motor::motor_right_Tien(int speed) {
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);//đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
  digitalWrite(this -> _in1, LOW);// chân này không có PWM
  analogWrite(this -> _in2, speed);
}
 
void Motor::motor_left_Lui(int speed) { //speed: từ 0 - MAX_SPEED
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);//đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
  analogWrite(this -> _in4, speed);
  digitalWrite(this -> _in3, LOW);// chân này không có PWM
}
 
void Motor::motor_left_Tien(int speed) {
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);//đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
  analogWrite(this -> _in4, 255 - speed);
  digitalWrite(this -> _in3, HIGH);// chân này không có PWM
}

void Motor::go(int speedRight, int speedLeft) {
  if (speedRight < 0) {
    this -> motor_right_Lui(abs(speedRight));
  } else {
    this -> motor_right_Tien(speedRight);
  }
  
  if (speedLeft < 0) {
    this -> motor_left_Lui(abs(speedLeft));
  }
  else {
    this -> motor_left_Tien(speedLeft);
  }
}

void Motor::turnLeft(int speed) {
  this -> motor_right_Tien(speed);
  this -> motor_left_Lui((int)(speed));
 // delay(50);
}

void Motor::turnRight(int speed) {
  this -> motor_right_Lui(speed);
  this -> motor_left_Tien((int)(speed));
 // delay(50);
}

void Motor::pControl(float error) {
  // Proportional control
  int delta = (this -> kp) * error;
  int speedLeft = this -> baseSpeed + delta;
  int speedRight = this -> baseSpeed - delta;
  speedLeft = max(speedLeft, 0);
  speedRight = max(speedRight, 0);
  Serial.print("speeds:");
  Serial.print(speedLeft);
  Serial.print(" ");
  Serial.print(speedRight);
  Serial.print("\n");
  this -> go(speedRight, (float) speedLeft);
  
//  Serial.print("Speed: ");
//  Serial.print(speedLeft);
//  Serial.print(" ");
//  Serial.println(speedRight);
//  if (delta < 0) {
//    Serial.println("Turn left");U
//  } else {
//    Serial.println("Turn right");
//  }
}



void Motor::stop() {
  this -> motor_left_Dung();
  this -> motor_right_Dung();
}

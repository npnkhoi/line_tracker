
#ifndef Motor_h
#define Motor_h

#include "Arduino.h"

class Motor {
  public:
    Motor(int in1, int int2, int in3, int int4);
    void motor_right_Dung();
    void motor_left_Dung();
    void motor_right_Lui(int speed);
    void motor_right_Tien(int speed);
    void motor_left_Lui(int speed);
    void motor_left_Tien(int speed);
    void goStraight(int speed);
    void turnRight(int speed);
    void turnLeft(int speed);
  private:
    int _in1, _in2, _in3, _in4;
};

#endif

#ifndef UltrasonicSensor_h
#define UltrasonicSensor_h

#include "Arduino.h"

class UltrasonicSensor {
  public:
    UltrasonicSensor(int trig, int echo);
    float getDist();
  private:
    int _trig, _echo;
};

#endif

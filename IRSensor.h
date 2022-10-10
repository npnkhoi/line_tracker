#ifndef IRSensor_h
#define IRSensor_h

#include "Arduino.h"
class IRSensor{
  public:
    IRSensor(const int *_pins);
    void trackLine();
    int pins[5], irVal[5];
    int offSide(int near, int far);
    int getError();
};

#endif

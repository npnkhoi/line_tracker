#ifndef IRSensor_h
#define IRSensor_h

#include "Arduino.h"
class IRSensor{
  public:
    IRSensor(const int *_pins);
    void trackLine();
    int getErrorTest();
    int getError();
    int pins[5], irVal[5];
    int countOnes();
  private:
    int offSide(int near , int far);
};

#endif

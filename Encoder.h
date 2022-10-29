#ifndef Encoder_h
#define Encoder_h

#include "Arduino.h"

class Encoder{
  public:
    Encoder(int leftPin, int rightPin);
    void resetCounter();
    void getCounter(int time_delay);
    static void incPulseLeft(), incPulseRight();
    static int leftCounter, rightCounter;
    unsigned long present, real_time;
    
  private:
    int _leftPin, _rightPin;

};

#endif

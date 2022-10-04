#ifndef Encoder_h
#define Encoder_h

#include "Arduino.h"

class Encoder{
  public:
    Encoder(int leftPin, int rightPin);
    void resetCounter();
    int getCounter(int side);
    static void incPulseLeft(), incPulseRight();
    static int leftCounter, rightCounter;
    
  private:
    int _leftPin, _rightPin;
};

#endif

#ifndef Display_h
#define Display_h

#include "Arduino.h"

class Display {
  public:
    Display(const int _pins[3]);
    void setCode(int code);
    void showMode(int mode);
  private:
    int pins[3];
};

#endif

#include "IRSensor.h"
#include "Arduino.h"

IRSensor::IRSensor(const int *_pins) {
  for (int i = 0; i < 5; ++ i) {
    pins[i] = _pins[i];
    pinMode(pins[i], INPUT);
    irVal[i] = 0;
  }
}

void IRSensor::trackLine() {
  for (int i = 0; i < 5; ++ i) {
    this -> irVal[i] = digitalRead(this -> pins[i]);
  }
}

int IRSensor::offSide(int near , int far) {
  if ((near == 1) && (far == 0)) {
    return 1;
  }
  if ((near == 0) && (far == 1)) {
    return 2;
  }
  if ((near == 1) && (far == 1)) {
    return 3;
  }
}

int IRSensor::getError() {
  if (this -> irVal[1] == irVal[2] == irVal[3] == irVal[4] == 0) {
    return 4;
  }
  else {
     if (this -> irVal[2] == 1) {
    if (this -> irVal[1] != irVal[3]) {
      if (this -> irVal[1] == 1) {
        return -1;
      }
      else {
        return 1;
      }
    }
    else {
      return 0;
    }
  }
  else {
    if (this -> irVal[0] == 1 || irVal[1] == 1) {
      -offSide(this -> irVal[1], irVal[0]);
    }
    else {
      offSide(this -> irVal[3], irVal[4]);
    }
  }
  }
}
 

  

//  int t = this -> offSide(this -> irVal[1], irVal[0]);



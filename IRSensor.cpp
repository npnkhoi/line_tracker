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
  // update irVal from the physical signals
  for (int i = 0; i < 5; ++ i) {
    this -> irVal[i] = digitalRead(this -> pins[i]);
  }
}

float IRSensor::offSide(int near , int far) {
  // return either 1, 2, or 3
  if ((near == 1) && (far == 0)) {
    return 1;
  }
  if ((near == 0) && (far == 1)) {
    return 3;
  }
  if ((near == 1) && (far == 1)) {
    return 2;
  }
}

float IRSensor::getError() {
  /*
  return an error in range [-4, 4]
  irVal = 1 means "see white, and no light on the physical IR"
  
  Error:
    0: perfect
    4: lost
    -4: pre-finishing line
    -1 to -3: only see white on the left
    1 to 3: only see white on the right
  */
  this -> trackLine();
  if ((this -> irVal[0] == 0) && (this -> irVal[1] == 0) && (this -> irVal[2] == 0) && (this -> irVal[3] == 0) && (this -> irVal[4] == 0)) {
    return 4;
  } else if ((this -> irVal[0] == 1) && (this -> irVal[1] == 1) && (this -> irVal[2] == 1) && (this -> irVal[3] == 1) && (this -> irVal[4] == 1)) {
    return -4;
  } else {
     if (this -> irVal[2] == 1) {
        int l = (this -> irVal[0]) + (this -> irVal[1]);
        int r = (this -> irVal[3]) + (this -> irVal[4]);

        if (l == r) return 0;
        if (abs(l - r) == 2) return r - l;
        // now abs(l - r) == 1
        return r - l;
        
                       
        // if (this -> irVal[1] != this -> irVal[3]) {
        //    if (this -> irVal[1] == 1) {
        //       return -1;
        //     } else {
        //       return 1;
        //     }
        // } else {
        //    return 0;
        // }
     } else {
        if (this -> irVal[0] == 1 || this -> irVal[1] == 1) {
           float k = offSide(this -> irVal[1], this -> irVal[0]);
           return -k;
        } else {
           float k = offSide(this -> irVal[3], this -> irVal[4]);
           return k;
        }
     }
  }
}

int IRSensor::countOnes() {
  // return the number of ones in irVal[]
  this -> trackLine();
  int ret = 0;
  for (int i = 0; i < 5; ++ i) {
    ret += abs(this -> irVal[i]);
  }
  // Serial.print("count ones: ");
  // Serial.println(ret);
  return ret;
}

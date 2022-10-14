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

//int IRSensor::getError() {
//  int getError;
//  return getError;
//}

int IRSensor::getErrorTest(){
  int getErrorTest;
//  const int IR_PINS[] = {8, 9, 10, 11, 12};
//  IRSensor irSensor(IR_PINS);
  if (this -> irVal[2] == 0) {
    if (this -> irVal[0] || this -> irVal[1]) {
      getErrorTest = 1;
      Serial.println("Turning left");
    } else if (this -> irVal[3] || this -> irVal[4]) {
      getErrorTest = -1;
      Serial.println("Turning right");
    } else {
      Serial.println("Completely lost");
      getErrorTest = 4;
    }
  } else {
    getErrorTest = 0;
    Serial.println("Going straight");
  }
  return getErrorTest;      
}

int IRSensor::offSide(int near , int far) {
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

int IRSensor::getError() {
  if ((this -> irVal[0] == 0) && (this -> irVal[1] == 0) && (this -> irVal[2] == 0) && (this -> irVal[3] == 0) && (this -> irVal[4] == 0)) {
    return 4;
  } else {
     if (this -> irVal[2] == 1) {
        if (this -> irVal[1] != this -> irVal[3]) {
           if (this -> irVal[1] == 1) {
              return -1;
            } else {
              return 1;
            }
        } else {
           return 0;
        }
     } else {
        if (this -> irVal[0] == 1 || this -> irVal[1] == 1) {
           int k = offSide(this -> irVal[1], this -> irVal[0]);
           return 0 - k;
        } else {
           int k = offSide(this -> irVal[3], this -> irVal[4]);
           return k;
        }
     }
  }
}
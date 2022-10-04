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

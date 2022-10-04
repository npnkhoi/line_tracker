#include "Encoder.h"

void Encoder::incPulseLeft() {
  leftCounter ++;
}

void Encoder::incPulseRight() {
  rightCounter ++;
}

Encoder::Encoder(int leftPin, int rightPin) {
  _leftPin = leftPin;
  _rightPin = rightPin;
  pinMode(leftPin, INPUT);
  pinMode(rightPin, INPUT);

  // pulse counters
  leftCounter = 0;
  rightCounter = 0;
  
  attachInterrupt(0, Encoder::incPulseLeft, RISING);
  attachInterrupt(1, Encoder::incPulseRight, RISING);
}

//float run_time(float mm_distance) {
//  float rpm = 132;
//  float wheel_perimeter = 214.0;
//  number_of_rev = mm_distance/wheel_perimeter;
//  float runtime = number_of_rev/rpm*60000;
//  return runtime;
//}

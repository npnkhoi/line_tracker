#include "Encoder.h"

Encoder::Encoder(int leftPin, int rightPin) {
  _leftPin = leftPin;
  _rightPin = rightPin;
  pinMode(leftPin, INPUT);
  pinMode(rightPin, INPUT);

  present = 0;
  
  attachInterrupt(0, Encoder::incPulseLeft, RISING);
  attachInterrupt(1, Encoder::incPulseRight, RISING);
}

int Encoder::leftCounter = 0;
int Encoder::rightCounter = 0;

void Encoder::incPulseLeft() {
  leftCounter ++;
}

void Encoder::incPulseRight() {
  rightCounter ++;
}

void Encoder::getCounter (int time_delay) {
  real_time = millis();
  if (real_time - present >= time_delay)
  {
    present = real_time;
//    Serial.print("Time: "); Serial.println(this -> real_time);
//    Serial.print("Present: "); Serial.println(this -> present);
//    Serial.print("Left: "); Serial.println(this -> leftCounter);
//    Serial.print("Right: "); Serial.println(this -> rightCounter); 
//    Serial.print("Ratio: "); Serial.println(1.0*(this -> leftCounter)/(this -> rightCounter)); 
  }
}

void Encoder::resetCounter(){
  // Reset both counters, and present
  leftCounter = 0;
  rightCounter = 0;
  present = 0;
}

//float run_time(float mm_distance) {
//  float rpm = 132;
//  float wheel_perimeter = 214.0;
//  number_of_rev = mm_distance/wheel_perimeter;
//  float runtime = number_of_rev/rpm*60000;
//  return runtime;
//}

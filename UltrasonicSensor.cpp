#include "UltrasonicSensor.h"
#include "Arduino.h"

UltrasonicSensor::UltrasonicSensor(int trig, int echo) {
  _trig = trig;
  _echo = echo;
  
  // declare the pins
  pinMode(trig,OUTPUT);   // chân trig sẽ phát tín hiệu
  pinMode(echo,INPUT);    // chân echo sẽ nhận tín hiệu
}

float UltrasonicSensor::getDist() {
  /*
  Returns distance to the object in the range of the sensor
  Adapted from http://arduino.vn/bai-viet/233-su-dung-cam-bien-khoang-cach-hc-sr04
  */
  unsigned long duration; // biến đo thời gian
  int distance;           // biến lưu khoảng cách
  
  /* Phát xung từ chân trig */
  digitalWrite(this -> _trig, 0);   // tắt chân trig
  delayMicroseconds(2);
  digitalWrite(this -> _trig, 1);   // phát xung từ chân trig
  delayMicroseconds(5);   // xung có độ dài 5 microSeconds
  digitalWrite(this -> _trig,0);   // tắt chân trig
  
  /* Tính toán thời gian */
  // Đo độ rộng xung HIGH ở chân echo. 
  duration = pulseIn(this -> _echo, HIGH);
  // Tính khoảng cách đến vật.
  distance = float(duration/2/29.412);
  // Serial.print("get dist: ");
  // Serial.println(distance);
  return distance;
}

bool UltrasonicSensor::check() {
  float tmp = this -> getDist();
  if (tmp <= 30 && tmp != 0) {
    return true;
  } else {
    return false;
  }
}

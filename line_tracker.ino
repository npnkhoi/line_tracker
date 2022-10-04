// for motors and encoders
#include "Motor.h"
Motor motor(4, 5, 7, 6);

//#define IN1 4 //HIGH = tien, LOW = lui
//#define IN2 5 //analog: speed
//#define IN3 7 //HIGH = tien, LOW = lui
//#define IN4 6 //analog: speed

#define encoderLeft 2
#define encoderRight 3

const int trig = A0;
const int echo = A1;

int cntPulseLeft = 0;
int cntPulseRight = 0;
float rpm = 0;
float number_of_rev = 0;
float timeStart = millis();
int wait = 1000;
int speed = 150;

// for line-tracker sensor
const int pinOUT1 = 8;
const int pinOUT2 = 9;
const int pinOUT3 = 10;
const int pinOUT4 = 11;
const int pinOUT5 = 12;
int IRvalue1 = 0;
int IRvalue2 = 0;
int IRvalue3 = 0;
int IRvalue4 = 0;
int IRvalue5 = 0;

// helper functions

void incPulseLeft() {
  cntPulseLeft++;
}

void incPulseRight() {
  cntPulseRight ++;
}

void setup()
{
  Serial.begin(9600);

  // declare the encoders
  pinMode(encoderLeft, INPUT);
  pinMode(encoderRight, INPUT);
  attachInterrupt(0, incPulseLeft, RISING);
  attachInterrupt(1, incPulseRight, RISING);

  // declare the line-tracking sensors
  pinMode(pinOUT1,INPUT);
  pinMode(pinOUT2,INPUT);
  pinMode(pinOUT3,INPUT);
  pinMode(pinOUT4,INPUT);
  pinMode(pinOUT5,INPUT);

  // declare the distance sensor
  pinMode(trig,OUTPUT);   // chân trig sẽ phát tín hiệu
  pinMode(echo,INPUT);    // chân echo sẽ nhận tín hiệu
}
 


float run_time(float mm_distance) {
  float rpm = 132;
  float wheel_perimeter = 214.0;
  number_of_rev = mm_distance/wheel_perimeter;
  float runtime = number_of_rev/rpm*60000;
  return runtime;
}

void trackLine() {
  IRvalue1 = digitalRead(pinOUT1);
  IRvalue2 = digitalRead(pinOUT2);
  IRvalue3 = digitalRead(pinOUT3);
  IRvalue4 = digitalRead(pinOUT4);
  IRvalue5 = digitalRead(pinOUT5);
}

float getDist() {
  unsigned long duration; // biến đo thời gian
  int distance;           // biến lưu khoảng cách
  
  /* Phát xung từ chân trig */
  digitalWrite(trig,0);   // tắt chân trig
  delayMicroseconds(2);
  digitalWrite(trig,1);   // phát xung từ chân trig
  delayMicroseconds(5);   // xung có độ dài 5 microSeconds
  digitalWrite(trig,0);   // tắt chân trig
  
  /* Tính toán thời gian */
  // Đo độ rộng xung HIGH ở chân echo. 
  duration = pulseIn(echo,HIGH);  
  // Tính khoảng cách đến vật.
  distance = float(duration/2/29.412);
  return distance;
}
 
void loop()
{   
//  turnLeft(speed);
  trackLine();
//  Serial.print(IRvalue1);
//  Serial.print(IRvalue2);
//  Serial.print(IRvalue3);
//  Serial.print(IRvalue4);
//  Serial.println(IRvalue5);

  if (IRvalue3 == 0) {
    if (IRvalue1 || IRvalue2) {
      motor.turnLeft(speed);
    } else if (IRvalue4 || IRvalue5) {
      motor.turnRight(speed);
    }
  } else {
    motor.goStraight(speed);
  }          
  Serial.println(getDist());      
  delay(100);                                
}

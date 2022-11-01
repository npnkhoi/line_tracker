#include "Display.h"

Display::Display(const int _pins[3]) {
  for (int i = 0; i < 3; ++ i) {
    pins[i] = _pins[i];
    pinMode(this -> pins[i], OUTPUT);
  }
}

void Display::setCode(int code) {
  Serial.println(code);
  for (int i = 0; i < 3; ++i) {
//    Serial.print(this -> pins[i]);
//    Serial.print(" ");
//    Serial.print((code >> i & 1) ? "HIGH " : "LOW ");
    digitalWrite(this -> pins[i], (code >> i & 1) ? HIGH : LOW);
  }
  Serial.println();
}

void Display::showMode(int mode) {
  int code;
  if (mode < 4) code = 4 + mode;
  else code = 3;
  Serial.println(code);
  this -> setCode(code);
}

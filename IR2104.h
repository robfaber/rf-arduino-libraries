#include <Arduino.h>
#include <PWM.h>
#include "PwmDriver.h"

#ifndef IR2104_H
#define IR2104_H

class IR2104: public PwmDriver {
public:
  IR2104(uint8_t thePwmPin, uint8_t theShutDownPin) {
    pwmPin = thePwmPin;
    shutdownPin = theShutDownPin;
  }

  void setup();
  uint8_t getPWM();
  void setPWM(uint8_t pwm);

private:
  uint8_t pwmPin;
  uint8_t shutdownPin;
  uint8_t pwm;
};

void IR2104::setup() {
  pinMode(shutdownPin, OUTPUT);
  pinMode(pwmPin, OUTPUT);

  InitTimersSafe(10000);
  SetPinFrequencySafe(pwmPin, 10000);

  setPWM(0);
}

void IR2104::setPWM(uint8_t pwmValue) {
  pwm = constrain(pwmValue, 0, 254);
  if (pwm > 0) {
    digitalWrite(shutdownPin, HIGH);
    pwmWrite(pwmPin, pwm);
  }
  else {
    digitalWrite(shutdownPin, LOW);
    pwmWrite(pwmPin, pwm);
  }
}

uint8_t IR2104::getPWM() {
  return pwm;
}

#endif
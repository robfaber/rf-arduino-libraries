#include <Arduino.h>
#include <PWM.h>
#include "PwmDriver.h"

#ifndef SIMPLEDRIVER_H
#define SIMPLEDRIVER_H

class SimpleDriver: public PwmDriver {
public:
  SimpleDriver(uint8_t pwmPin) {
    pin = pwmPin;
  }
  void setup();
  void setPWM(uint8_t pwm);
  uint8_t getPWM();

private:
  uint8_t pin;
  uint8_t pwm;
};

void SimpleDriver::setup() {
  pinMode(pin, OUTPUT);
  InitTimersSafe(10000);
  SetPinFrequencySafe(pin, 10000);
  setPWM(0);
}

void SimpleDriver::setPWM(uint8_t pwmValue) {
  pwm = constrain(pwmValue, 0, 254);
  pwmWrite(pin, pwm);
}

uint8_t SimpleDriver::getPWM() {
  return pwm;
}

#endif
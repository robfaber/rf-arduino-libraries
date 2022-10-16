#include <Arduino.h>
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
  if (pin == 5 || pin == 6) {
    //Timer 0 case
    TCCR0A = _BV(WGM01) | _BV(WGM00);
    TCCR0B = _BV(CS00); // no prescaling results in 31.2kHz PWM

  }
  else if (pin == 9 || pin == 10) {
    //Timer 1 case - using 8bit fast pwm mode, 10bit also possible
    TCCR1A = _BV(WGM10);
    TCCR1B = _BV(WGM12) | _BV(CS10); // no prescaling results in 31.2kHz PWM
  }
  else if (pin == 3 || pin == 11) {
    //Timer 2 case
    TCCR2A = _BV(WGM21) | _BV(WGM20);
    TCCR2B = _BV(CS20); // no prescaling results in 31.2kHz PWM
  }
  else {
    if (Serial) {
      Serial.println("Invalid pin for PWM driver, use T0(5,6) T1(9,10) or T2(3,11)");
    }
  }

  setPWM(0);
}

void SimpleDriver::setPWM(uint8_t pwmValue) {
  pwm = constrain(pwmValue, 0, 254);
  analogWrite(pin, pwm);
}

uint8_t SimpleDriver::getPWM() {
  return pwm;
}

#endif
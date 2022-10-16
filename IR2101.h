#include <Arduino.h>
#include "PwmDriver.h"

#ifndef IR2101_H
#define IR2101_H

class IR2101: public PwmDriver {
public:
  IR2101(uint8_t pwmLoPin, uint8_t pwmHiPin) {
    loPin = pwmLoPin;
    hiPin = pwmHiPin;
  }

  void setup();
  uint8_t getPWM();
  void setPWM(uint8_t pwm);
private:
  uint8_t loPin;
  uint8_t hiPin;
  uint8_t pwm;
};

void IR2101::setup() {
  pinMode(loPin, OUTPUT);
  pinMode(hiPin, OUTPUT);

  if ((loPin == 5 && hiPin == 6) || (loPin == 6 && hiPin == 5)) {
    //Timer 0 case
    TCCR0A = _BV(WGM01) | _BV(WGM00);
    TCCR0B = _BV(CS00); // no prescaling results in 31.2kHz PWM

  }
  else if ((loPin == 9 && hiPin == 10) || (loPin == 10 && hiPin == 9)) {
    //Timer 1 case - using 8bit fast pwm mode, 10bit also possible
    TCCR1A = _BV(WGM10);
    TCCR1B = _BV(WGM12) | _BV(CS10); // no prescaling results in 31.2kHz PWM
  }
  else if ((loPin == 3 && hiPin == 11) || (loPin == 11 && hiPin == 3)) {
    //Timer 2 case
    TCCR2A = _BV(WGM21) | _BV(WGM20);
    TCCR2B = _BV(CS20); // no prescaling results in 31.2kHz PWM
  }
  else {
    if (Serial) {
      Serial.println("Invalid pin combination for PWM driver, use T0(5,6) T1(9,10) or T2(3,11)");
    }
  }


  setPWM(0);
}

void IR2101::setPWM(uint8_t pwmValue) {
  pwm = constrain(pwmValue, 0, 254);
  if (pwm > 0) {
    analogWrite(loPin, pwm);
    analogWrite(hiPin, pwm);
  }
  else {
    digitalWrite(loPin, HIGH);
    digitalWrite(hiPin, LOW);
  }
}


uint8_t IR2101::getPWM() {
  return pwm;
}

#endif
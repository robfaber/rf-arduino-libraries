#include <Arduino.h>
#ifndef PWM_DRIVER
#define PWM_DRIVER

class PwmDriver {
public:
  PwmDriver() {};
  virtual void setup();
  virtual void setPWM(uint8_t pwm);
  virtual uint8_t getPWM();
  void setDutyCycle(float dutyCycle);
  float getDutyCycle();
};

void PwmDriver::setDutyCycle(float dutyCycle) {
  float dc = constrain(dutyCycle, 0, 99);
  uint8_t pwm = (dc * 255.0 / 100.0);
  setPWM(pwm);
}

float PwmDriver::getDutyCycle() {
  return getPWM() * 100.0 / 255.0;
}

#endif
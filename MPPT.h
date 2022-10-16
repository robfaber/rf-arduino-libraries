#include <Arduino.h>
#include "PwmDriver.h"
#include "CurrentSensor.h"
#include "VoltageSensor.h"
#include "SweepListener.h"

#ifndef MPPT_H
#define MPPT_H

#define MAX_PWM 254

class MPPT {
public:
  MPPT(
    PwmDriver* pwmDriver,
    CurrentSensor* solarCurrentSensor,
    VoltageSensor* solarVoltageSensor,
    float maximumCurrent,
    SweepListener* sweepListener = nullptr) {

    driver = pwmDriver;
    currentSensor = solarCurrentSensor;
    voltageSensor = solarVoltageSensor;
    maxCurrent = maximumCurrent;
    sweepCallback = sweepListener;
  }

  void sweep(float vcc);
  void update(float Vsol, float Isol);
  float getDutyCycle();

private:
  PwmDriver* driver;
  CurrentSensor* currentSensor;
  VoltageSensor* voltageSensor;
  SweepListener* sweepCallback;
  float maxCurrent;
  uint8_t pwm = 0;

  float previousPower = 0;
  float previousSolarVoltage;
};

float MPPT::getDutyCycle() {
  return driver->getDutyCycle();
}

void MPPT::sweep(float vcc) {
  float maxPower = 0;
  float mpptPwm;
  float Vsol, Isol, Psol;
  for (pwm = 0; pwm <= MAX_PWM; pwm++) {
    driver->setPWM(pwm);
    Vsol = voltageSensor->read(vcc);
    Isol = currentSensor->read(vcc);
    Psol = Isol * Vsol;

    if (sweepCallback) {
      sweepCallback->sweepResult(pwm, Vsol, Isol, Psol);
    }
    if (Psol > maxPower) {
      maxPower = Psol;
      mpptPwm = pwm;
    }
  }
  pwm = mpptPwm;
  driver->setPWM(pwm);
}

void MPPT::update(float Vsol, float Isol) {
  float Psol = Vsol * Isol;

  if (Psol > previousPower) {
    if (Vsol > previousSolarVoltage) { //We are moving from the left towards the MPPT
      pwm--;
    }
    else if (Vsol < previousSolarVoltage) { //We are moving from the right towards the MPPT
      pwm++;
    }

  }
  else if (Psol < previousPower) {
    if (Vsol > previousSolarVoltage) { //we are moving further to the right past the MPPT
      pwm++;
    }
    else if (Vsol < previousSolarVoltage) { //We are moving further to the left away from the MPPT
      pwm--;
    }
  }
  driver->setPWM(pwm);
  previousPower = Psol;
  previousSolarVoltage = Vsol;

  // if (currentSolarVoltage < batteryVoltage) {
  //   mode = OFF; // If the voltage is lower there cannot be a charging current
  //   minPwm = 0;
  //   pwm = 0;

  // } else {
    // if (batteryVoltage > MAX_BATTERY + 0.1) {
    //   minPwm = (batteryVoltage/currentSolarVoltage) * MAX_PWM;
    //   mode = CV;    // In constant voltage mode we try to match the pwm in such a way that it matches the battery voltage
    //   pwm --;

    // } else if (batteryVoltage > MAX_BATTERY - 0.1) {
    //   minPwm = (batteryVoltage/currentSolarVoltage) * MAX_PWM;
    //   mode = CV;    // In constant voltage mode we try to match the pwm in such a way that it matches the battery voltage
    //   pwm ++;

    // } else {
      // minPwm = (batteryVoltage/currentSolarVoltage) * MAX_PWM;
      // mode = CC;
      // if (solarCurrent > MAX_CURRENT) {
      //   pwm--;
      // } else {
      //   if (currentPower < MIN_POWER) {
      //     pwmSweep(); // If we are not producing enough power let's do a sweep to find the sweet spot again
        // } else if (currentPower > previousPower) {
        //   if (currentSolarVoltage > previousSolarVoltage) { //We are moving from the left towards the MPPT
        //     pwm--;
        //   } else if (currentSolarVoltage < previousSolarVoltage) { //We are moving from the right towards the MPPT
        //     pwm++;
        //   }

        // } else if (currentPower < previousPower) {
        //   if (currentSolarVoltage > previousSolarVoltage) { //we are moving further to the right past the MPPT
        //       pwm++;
        //   } else if (currentSolarVoltage < previousSolarVoltage) { //We are moving further to the left away from the MPPT
        //       pwm--; 
        //   }
        // }
      // }
    // }
  // }

}

#endif
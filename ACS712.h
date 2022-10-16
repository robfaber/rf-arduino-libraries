#include <Arduino.h>
#include "CurrentSensor.h"

#ifndef ACS712_H
#define ACS712_H
const float ACS712_MVA = 0.185; //ACS712-5B gives 185 mV/A

class ACS712: public CurrentSensor {
public:
  ACS712(uint8_t sensePin, int numberOfSamples) {
    pin = sensePin;
    samples = numberOfSamples;
  }
  void setup();
  float read(float vcc);
  float getValue();
  float getBaseValue();
  void calibrate(float vcc);

private:
  uint8_t pin;
  int samples = 1;
  float vcc = 3.3;
  float baseValue = 2.5; //Midpoint of ACS712 is 2.5 Volt. Calibrate will determine a more accurate value
  float value;
};

void ACS712::setup() {
  pinMode(pin, INPUT);
}

float ACS712::read(float vcc) {
  int read = readAverage(pin, samples);
  //Translate value to a voltage
  float voltage = read * vcc / 1023.0;

  //We subtract the baseValue
  voltage -= baseValue;
  value = voltage / ACS712_MVA;
  return value;
}

float ACS712::getValue() {
  return value;
}
float ACS712::getBaseValue() {
  return baseValue;
}

void ACS712::calibrate(float vcc) {
  baseValue = readAverage(pin, samples) * vcc / 1023.0;
}
#endif
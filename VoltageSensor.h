#include <Arduino.h>
#include "Sensor.h"

#ifndef VOLTAGE_SENSOR
#define VOLTAGE_SENSOR

class VoltageSensor: public Sensor {
public:
  VoltageSensor(int8_t inputPin, float r1Value, float r2Value, int nrOfSamples) {
    pin = inputPin;
    r1 = r1Value;
    r2 = r2Value;
    samples = nrOfSamples;
  };

  void setup();
  float read(float vcc);
  float getValue();

private:
  uint8_t pin;
  float r1, r2;
  float factor = 0;
  int samples = 1;
  float value;
};

void VoltageSensor::setup() {
  pinMode(pin, INPUT);
  factor = (r1 + r2) / (float)r2;
}
float VoltageSensor::read(float vcc) {
  unsigned int read = readAverage(pin, samples);
  float voltage = read * vcc / 1023.0;
  value = voltage * factor;
  return value;
}
float VoltageSensor::getValue() {
  return value;
}

#endif
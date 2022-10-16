#include <Arduino.h>
#include "Sensor.h"

#ifndef VOLTAGE_SENSOR
#define VOLTAGE_SENSOR

class VoltageSensor: public Sensor {
public:
  VoltageSensor(int8_t inputPin, int nrOfSamples = 1) {
    pin = inputPin;
    r1 = 0;
    r2 = 1; // Will make the factor 1
    samples = nrOfSamples;
  };
  VoltageSensor(int8_t inputPin, float r1Value, float r2Value, int nrOfSamples = 1) {
    pin = inputPin;
    r1 = r1Value;
    r2 = r2Value;
    samples = nrOfSamples;
  };

  void setup();
  float read(float vcc);
  float value();

private:
  uint8_t pin;
  float r1, r2;
  float factor = 0;
  int samples;
  float voltage;
};

void VoltageSensor::setup() {
  pinMode(pin, INPUT);
  factor = (r1 + r2) / (float)r2;
}

float VoltageSensor::read(float vcc) {
  unsigned int read = readAverage(pin, samples);
  voltage = read * vcc * factor / 1023.0;
  return voltage;
}

float VoltageSensor::value() {
  return voltage;
}

#endif
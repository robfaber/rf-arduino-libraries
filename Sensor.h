#include <Arduino.h>
#ifndef SENSOR
#define SENSOR

class Sensor {
public:
  Sensor() {};

protected:
  unsigned int readAverage(int pin, int samples = 1);
  float readAverageVoltage(int pin, float vcc, int samples = 1);
};

unsigned int Sensor::readAverage(int pin, int samples = 1) {
  unsigned long sum = 0;
  for (int i = 0; i < samples; i++) {
    sum += analogRead(pin);
  }
  return (unsigned int)(sum / samples);
}

float Sensor::readAverageVoltage(int pin, float Vcc, int samples = 1) {
  unsigned int value = readAverage(pin, samples);
  return value * Vcc / 1023.0;
}

#endif
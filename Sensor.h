#include <Arduino.h>
#ifndef SENSOR
#define SENSOR

class Sensor {
public:
  Sensor() {};

protected:
  unsigned int readAverage(int pin, int samples);
};

unsigned int Sensor::readAverage(int pin, int samples) {
  unsigned long sum = 0;
  for (int i = 0; i < samples; i++) {
    sum += analogRead(pin);
  }
  return (unsigned int)(sum / samples);
}

#endif
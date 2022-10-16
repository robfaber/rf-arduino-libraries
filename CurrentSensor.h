#include <Arduino.h>
#include "Sensor.h"
#ifndef CURRENT_SENSOR
#define CURRENT_SENSOR

class CurrentSensor: public Sensor {
public:
  CurrentSensor() {};
  virtual float read(float vcc);
  virtual float getValue();
  void calibrate(float vcc) {}
};
#endif
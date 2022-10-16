#include <Arduino.h>
#include "CurrentSensor.h"
#ifndef SHUNT_CURRENT_SENSOR
#define SHUNT_CURRENT_SENSOR
/**
 * This setup uses a shunt to measure current
 *
 * On the low side (with inverting amplifier);
 *     Vout = - Vshunt * (R2/R1)
 * On the high (with differential amplifier);
 *     Vout = Vshunt * (R2/R1)
 *
 * We call R2/R1 the gain
 */
class ShuntCurrentSensor: public CurrentSensor {
public:
  ShuntCurrentSensor(uint8_t inputPin, float rs, float ampGain, int numberOfSamples = 1) {
    pin = inputPin;
    Rs = rs;
    gain = ampGain;
    samples = numberOfSamples;
  };

  void setup();
  float read(float vcc);
  float current();
private:
  float Rs;
  float I;
  uint8_t pin;
  int samples;
  float gain;
};

void ShuntCurrentSensor::setup() {
  pinMode(pin, INPUT);
}

float ShuntCurrentSensor::read(float Vcc) {
  float Vout = readAverageVoltage(pin, Vcc, samples); // Measured voltage
  float Vshunt = Vout / gain;        // Vin over the shunt
  I = Vshunt / Rs;                   // Current through the shunt is Vshunt/Rshunt
  return I;
}

float ShuntCurrentSensor::current() {
  return I;
}

#endif
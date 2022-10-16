#include <Arduino.h>
#include "Sensor.h"
#include "mydelay.h"

#ifndef VCC_SENSOR
#define VCC_SENSOR

class VccSensor: public Sensor {
public:
    VccSensor() {};
    float update();
    float value();
private:
    float vcc;
};

float VccSensor::update() {
    unsigned int result;

    ADMUX = (1 << REFS0) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1);
    mydelay(10); // Wait for Vref to settle

    ADCSRA |= (1 << ADSC); // Start conversion
    while (bit_is_set(ADCSRA, ADSC)); // wait until done
    result = ADC;

    // second time is a charm
    ADCSRA |= (1 << ADSC); // Start conversion
    while (bit_is_set(ADCSRA, ADSC)); // wait until done
    result = ADC;

    vcc = 1126.4 / (float)result; //1126.4 = 1.1*1024
    return vcc; // Vcc in volts
}

float VccSensor::value() {
    return vcc;
}

#endif
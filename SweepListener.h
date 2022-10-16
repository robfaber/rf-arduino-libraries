#include <Arduino.h>

#ifndef SWEEP_LIST
#define SWEEP_LIST

class SweepListener {
public:
    virtual void sweepResult(int pwm, float Vsol, float Isol, float Psol);
};

#endif
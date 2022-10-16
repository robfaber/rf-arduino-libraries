#include <Arduino.h>

#ifndef MPPT_DISPLAY
#define MPPT_DISPLAY

class MpptDisplay {
public:
    MpptDisplay() {};

public:
    virtual void setup();
    virtual void clear();
    virtual void init(float vcc, float Vsolb, float Vbatb);
    virtual void sweepUpdate(int pwm, float Vsol, float Isol, float Psol);
    virtual void update(float vcc, float Vsol, float Isol, float Vbat, float Ibat, float batPerc, int dc, float eff, int mode);
};

#endif
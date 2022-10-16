#include <Arduino.h>
#ifndef MYDELAY_H
#define MYDELAY_H


void mydelay(unsigned long ms) {
    byte value = TCCR0B & (_BV(CS00) | _BV(CS01) | _BV(CS02));
    if (value == 0) {
        return delay(ms);
    }
    else if (value == 1) {
        return delay(ms * 64);
    }
    else if (value == 2) {
        return delay(ms * 8);
    }
    else if (value == 3) {
        return delay(ms);
    }
    else if (value == 4) {
        return delay(ms / 4);
    }
    else if (value == 5) {
        return delay(ms / 16);
    }
    else {
        return delay(ms);
    }
}

unsigned long mymillis() {
    byte value = TCCR0B & (_BV(CS00) | _BV(CS01) | _BV(CS02));
    if (value == 1) {
        return millis() / 64;
    }
    else if (value == 2) {
        return millis() / 8;
    }
    else if (value == 3) {
        return millis();
    }
    else if (value == 4) {
        return millis() * 4;
    }
    else if (value == 5) {
        return millis() * 16;
    }
    else {
        return millis();
    }
}

#endif

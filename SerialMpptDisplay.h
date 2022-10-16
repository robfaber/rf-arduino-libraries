#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "MpptDisplay.h"
#include "icons.h"
#include "mydelay.h"

#ifndef SERIALMPPT_DISPLAY
#define SERIALMPPT_DISPLAY

class SerialMpptDisplay: public MpptDisplay {
public:
    SerialMpptDisplay(unsigned long baudrate) {
        rate = baudrate;
    };
    void setup();
    void clear();
    void init(float vcc, float Vsolb, float Vbatb);
    void sweepUpdate(int pwm, float Vsol, float Isol, float Psol);
    void update(float vcc, float Vsol, float Isol, float Vbat, float Ibat, float batPerc, int dc, float eff, int mode);
private:
    unsigned long rate;
    void logValue(const char* label, const float value, const int decimals, const char* unit);
};


void SerialMpptDisplay::setup() {
    Serial.begin(rate);
    Serial.println("MPPTv2 2022-10");
}

void SerialMpptDisplay::clear() {
    //NOP
}

void SerialMpptDisplay::sweepUpdate(int pwm, float Vsol, float Isol, float Psol) {
    Serial.print(pwm);
    Serial.print(", ");
    Serial.print(pwm / 2.55, 0);
    Serial.print(", ");
    Serial.print(Vsol, 1);
    Serial.print(", ");
    Serial.print(Isol, 2);
    Serial.print(", ");
    Serial.print(Psol, 1);
    Serial.println();
}

void SerialMpptDisplay::logValue(const char* label, const float value, const int decimals, const char* unit) {
    Serial.print(label);
    Serial.print(" = ");
    Serial.print(value, decimals);
    Serial.print(unit);
}

void SerialMpptDisplay::update(float vcc, float Vsol, float Isol, float Vbat, float Ibat, float batPerc, int dc, float eff, int mode) {
    logValue("Vsolar", Vsol, 1, "V, ");
    logValue("Isolar", Isol, 2, "A, ");
    logValue("Psolar", Vsol * Isol, 1, "W, ");
    logValue("Vbat", Vbat, 1, "V, ");
    logValue("Ibat", Ibat, 2, "A, ");
    logValue("Bat", batPerc, 0, "%, ");
    logValue("DC", dc, 0, "%");
    Serial.println("");
}

void SerialMpptDisplay::init(float vcc, float Vsolb, float Vbatb) {
    logValue("Vcc", vcc, 3, "V, ");
    logValue("Vbase_sol", Vsolb, 3, "V, ");
    logValue("Vbase_bat", Vbatb, 3, "V");
    Serial.println("");
}

#endif
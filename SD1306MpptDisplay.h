#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "MpptDisplay.h"
#include "icons.h"
#include "mydelay.h"

#ifndef SD1306MPPT_DISPLAY
#define SD1306MPPT_DISPLAY

//Rotated display draw lines
const int y1 = 9;
const int y2 = 23;
const int y3 = 38;
const int y4 = 53;

//Rotated display draw rectangle at the top
const int rx1 = 0;
const int ry1 = 0;

const int x1 = 5;
const int x2 = 50;
const int x3 = 95;

const int rw = 128;
const int rh = 8;

class SD1306MpptDisplay: public MpptDisplay {
public:
    SD1306MpptDisplay(Adafruit_SSD1306* ssdDisplay, uint8_t i2cAddr) {
        display = ssdDisplay;
        address = i2cAddr;
    };
    void setup();
    void clear();
    void init(float vcc, float Vsolb, float Vbatb);
    void sweepUpdate(int pwm, float Vsol, float Isol, float Psol);
    void update(float vcc, float Vsol, float Isol, float Vbat, float Ibat, float batPerc, int dc, float eff, int mode);
private:
    Adafruit_SSD1306* display;
    uint8_t address;
    int px, py1, py2, py3;
    void drawHeader(int bat, float efficiency, int mode);
};

void SD1306MpptDisplay::drawHeader(int bat, float efficiency, int mode) {
    display->setTextSize(1);
    display->setTextColor(SSD1306_WHITE);
    display->cp437(true);

    display->setCursor(x1, y1);
    display->write("SOL");
    display->drawBitmap(30, y1, solar_icon, 8, 8, SSD1306_WHITE);

    display->setCursor(x2, y1);
    display->write("BAT");
    display->drawBitmap(75, y1, bat_icons[bat], 8, 8, SSD1306_WHITE);

    display->setCursor(x3, y1);
    display->write("PWM");
    display->drawBitmap(120, y1, pwm_icon, 8, 8, SSD1306_WHITE);

    display->setCursor(95, y3);
    if (mode == 0) {
        display->write("Off");
    }
    else if (mode == 1) {
        display->write("CC");
    }
    else if (mode == 2) {
        display->write("CV");
    }
    // display->write("VCC");
    // display->drawBitmap(120, y3, vcc_icon, 8, 8, SSD1306_WHITE);

    display->fillRect(rx1, ry1, rw, rh, SSD1306_BLACK);
}


void SD1306MpptDisplay::setup() {
    while (!display->begin(SSD1306_SWITCHCAPVCC, address, false)) {
        Serial.begin(115200);
        Serial.println(F("SSD1306 allocation failed"));
        mydelay(1000);
    }

    display->clearDisplay();
    // drawHeader(0, 0, 0);
    display->display();
}

void SD1306MpptDisplay::init(float vcc, float Vsolb, float Vbatb) {
    display->setCursor(x1, y1);
    display->write("VCC");
    display->setCursor(x1, y2);
    display->print(vcc, 3);
    display->print("V");

    display->setCursor(x2, y1);
    display->write("Vsol");
    display->setCursor(x2, y2);
    display->print(Vsolb, 3);
    display->print("V");

    display->setCursor(x3, y1);
    display->write("Vbat");
    display->setCursor(x3, y2);
    display->print(Vbatb, 3);
    display->print("V");

    mydelay(3000);
}


void SD1306MpptDisplay::clear() {
    display->clearDisplay();
    px = 0;
    py1 = 60;
    py2 = 60;
    py3 = 60;
}
void SD1306MpptDisplay::sweepUpdate(int pwm, float Vsol, float Isol, float Psol) {
    if (pwm % 2 == 0) return;
    int x = pwm / 2;
    int y1 = (int)(60 - 2 * Vsol);  //Max 25V so Max 50 pixels
    // int y2 = (int)(60 - 30 * Isol); //Max 2A so Max 60 Pixels
    int y3 = (int)(60 - 2 * Psol);  //Max 30W so Max 60 Pixels
    if (pwm == 1) {
        py1 = y1;
        py2 = y2;
        py3 = y3;
    }

    display->drawLine(px, py1, x, y1, SSD1306_WHITE);
    // display->drawLine(px, py2, x, y2, SSD1306_WHITE);
    display->drawLine(px, py3, x, y3, SSD1306_WHITE);
    display->drawLine(px, 60, px, 60, SSD1306_WHITE);
    display->display();

    px = x;
    py1 = y1;
    py2 = y2;
    py3 = y3;
}

void SD1306MpptDisplay::update(float vcc, float Vsol, float Isol, float Vbat, float Ibat, float batPerc, int dc, float eff, int mode) {
    display->clearDisplay();

    int indicator = (int)(min(batPerc, 100) * 6 / 100);
    drawHeader(indicator, eff, mode);

    display->setCursor(x1, y2);
    display->print(Vsol, 1);
    display->print('V');
    display->setCursor(x1, y3);
    display->print(Isol, 2);
    display->print("A");
    display->setCursor(x1, y4);
    display->print(Vsol * Isol, 1);
    display->print("W");

    display->setCursor(x2, y2);
    display->print(Vbat, 1);
    display->print('V');
    display->setCursor(x2, y3);
    display->print(Ibat, 2);
    display->print("A");
    display->setCursor(x2, y4);
    display->print(batPerc, 0);
    display->print('%');


    display->setCursor(x3, y2);
    display->print(dc);
    display->print('%');
    display->setCursor(x3, y4);
    if (eff > 0) {
        display->print(eff, 0);
        display->print("%");
    }
    else {
        display->print(vcc, 1);
        display->print("V");
    }

    display->fillRect(rx1, ry1, rw, rh, SSD1306_BLACK);
    display->display();
}

#endif
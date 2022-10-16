#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#include <IR2101.h>
#include <MPPT.h>
#include <ACS712.h>
#include <VoltageSensor.h>
#include <VccSensor.h>
#include <SweepListener.h>
#include <SD1306MpptDisplay.h>
#include <SerialMpptDisplay.h>
#include <radio.h>

#define SERIAL_BAUD    115200
#define SOLAR_CUR_PIN  A0
#define SOLAR_VOL_PIN  A1
#define BAT_CUR_PIN    A2
#define BAT_VOLT_PIN   A3
#define AVG_READS       20
#define PWM_LO_PIN      5
#define PWM_HI_PIN      6
#define MAX_CURRENT     2.0
#define MAX_BATTERY     12.6
#define SCREEN_WIDTH      128  // OLED display width, in pixels
#define SCREEN_HEIGHT     64   // OLED display height, in pixels
#define SCREEN_ADDRESS    0x3C // 0x3C for the SSD1306 i got from Ali

VccSensor vccSensor;
// Adafruit_SSD1306 rawDisplay(SCREEN_WIDTH, SCREEN_HEIGHT);
// SD1306MpptDisplay display(&rawDisplay, SCREEN_ADDRESS);
// SerialMpptDisplay* display = new SerialMpptDisplay(SERIAL_BAUD);
MpptDisplay* display = NULL;
IR2101 driver(PWM_LO_PIN, PWM_HI_PIN);
ACS712 solarCurrentSensor(SOLAR_CUR_PIN, AVG_READS);
ACS712 batteryCurrentSensor(BAT_CUR_PIN, AVG_READS);
VoltageSensor solarVoltageSensor(SOLAR_VOL_PIN, 100000, 15000, AVG_READS);
VoltageSensor batteryVoltageSensor(BAT_VOLT_PIN, 100000, 15000, AVG_READS);

class LogSweep: public SweepListener {
  void sweepResult(int pwm, float Vsol, float Isol, float Psol) {
    if (display) display->sweepUpdate(pwm, Vsol, Isol, Psol);
  }
};

LogSweep sweepCallback;
MPPT mppt(&driver, &solarCurrentSensor, &solarVoltageSensor, MAX_CURRENT, &sweepCallback);

float vcc = 3.3;

void setup() {
  vcc = vccSensor.update();
  if (display) display->setup();
  setupRadio();

  driver.setup();
  solarCurrentSensor.setup();
  batteryCurrentSensor.setup();

  driver.setDutyCycle(0);

  solarCurrentSensor.calibrate(vcc);
  batteryCurrentSensor.calibrate(vcc);

  if (display) display->init(vcc, solarCurrentSensor.getBaseValue(), batteryCurrentSensor.getBaseValue());

  solarVoltageSensor.setup();
  batteryVoltageSensor.setup();

  if (display) display->clear();
  mppt.sweep(vcc);
}

const unsigned long logInterval = 5000;
unsigned long lastLog = 0;
unsigned long now;

double Vsol = 0;
double Isol = 0;
double Vbat = 0;
double Ibat = 0;
long count = 0;

void loop() {
  now = mymillis();
  Vsol += solarVoltageSensor.read(vcc);
  Vbat += batteryVoltageSensor.read(vcc);
  Isol += solarCurrentSensor.read(vcc);
  Ibat += batteryCurrentSensor.read(vcc);
  count++;

  if (now - lastLog > logInterval) {

    Vsol /= count;
    Vbat /= count;
    Isol /= count;
    Ibat /= count;

    float Psol = constrain(Vsol * Isol, 0, 1000);
    float Pbat = constrain(Vbat * (Ibat - 0.03), 0, 1000);
    float eff = Pbat * 100 / Psol;
    float batPerc = Vbat * 100 / MAX_BATTERY;
    uint8_t dc = mppt.getDutyCycle();
    mppt.update(Vsol, Isol);

    lastLog = now;
    broadcast(vcc, Vsol, Isol, Vbat, Ibat, dc, batPerc, eff);
    if (display) display->update(vcc, Vsol, Isol, Vbat, Ibat, batPerc, dc, eff, 1);

    Vsol = 0;
    Isol = 0;
    Vbat = 0;
    Ibat = 0;
    count = 0;
  }
}
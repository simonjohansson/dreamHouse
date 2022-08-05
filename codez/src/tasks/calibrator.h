#ifndef CALIBRATOR_TASK
#define CALIBRATOR_TASK
#include <ADS1X15.h>
#include <Arduino.h>
#include <Preferences.h>
#include <SPI.h>
#include <consts.h>

class Calibrator {
  private:
    ADS1115 adc0_;
    ADS1115 adc1_;
    Preferences *pref_;

    void maxValues(long maxPotValue[]);
    static void startCalibration(void *_this);
    void calibration();

  public:
    Calibrator(ADS1115 adc0, ADS1115 adc1, Preferences *preferences);
    void calibrate();
};

#endif
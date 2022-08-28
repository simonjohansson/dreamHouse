#include "calibrator.h"

void Calibrator::maxValues(long maxPotValue[]) {
    long now = millis();
    for (int i = 0; i < 8; i++) {
        maxPotValue[i] = 0;
    }

    // Check for 60 seconds
    while (millis() - now < 60000) {
        long newValue[8];
        newValue[0] = adc0_.readADC(0);
        newValue[1] = adc0_.readADC(1);
        newValue[2] = adc0_.readADC(2);
        newValue[3] = adc0_.readADC(3);
        newValue[4] = adc1_.readADC(0);
        newValue[5] = adc1_.readADC(1);
        newValue[6] = adc1_.readADC(2);
        newValue[7] = adc1_.readADC(3);

        for (int i = 0; i < 8; i++) {
            if (newValue[i] > maxPotValue[i]) {
                maxPotValue[i] = newValue[i];
                log_i("%d %d %d %d %d %d %d %d", maxPotValue[0], maxPotValue[1], maxPotValue[2], maxPotValue[3], maxPotValue[4], maxPotValue[5], maxPotValue[6],
                      maxPotValue[7]);
            }
        }
    }
}

void Calibrator::calibration() {
    log_i("Starting calibration of potentiometers. Please wiggle them around at their max value..");
    pref_->clear();

    long values[8];
    maxValues(values);

    for (int i = 0; i < 8; i++) {
        char potName[80];
        sprintf(potName, "pot%d", i);
        pref_->putLong(potName, values[i]);
    }

    pref_->putBool(CALIBRATION_KEY, true);
    ESP.restart();
}

void Calibrator::startCalibration(void *_this) { ((Calibrator *)_this)->calibration(); }

void Calibrator::calibrate() { xTaskCreatePinnedToCore(Calibrator::startCalibration, "", 2048, this, 1, NULL, 0); }

Calibrator::Calibrator(ADS1115 adc0, ADS1115 adc1, Preferences *preferences) {
    pref_ = preferences;
    adc0_ = adc0;
    adc1_ = adc1;
}
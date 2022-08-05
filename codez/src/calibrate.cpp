#include <calibrate.h>

Preferences pref;
ADS1115 a0(ADC0);
ADS1115 a1(ADC1);

void maxValues(long maxPotValue[]) {
    long now = millis();
    for (int i = 0; i < 8; i++) {
        maxPotValue[i] = 0;
    }

    // Check for 60 seconds
    while (millis() - now < 60000) {
        long newValue[8];
        newValue[0] = a0.readADC(0);
        newValue[1] = a0.readADC(1);
        newValue[2] = a0.readADC(2);
        newValue[3] = a0.readADC(3);
        newValue[4] = a1.readADC(0);
        newValue[5] = a1.readADC(1);
        newValue[6] = a1.readADC(2);
        newValue[7] = a1.readADC(3);

        for (int i = 0; i < 8; i++) {
            if (newValue[i] > maxPotValue[i]) {
                maxPotValue[i] = newValue[i];
                log_i("%d %d %d %d %d %d %d %d", maxPotValue[0], maxPotValue[1], maxPotValue[2], maxPotValue[3], maxPotValue[4], maxPotValue[5], maxPotValue[6],
                      maxPotValue[7]);
            }
        }
    }
}

void calibrateTask(void *pvParameters) {
    log_i("Starting calibration of potentiometers. Please wiggle them around at their max value..");

    long values[8];
    maxValues(values);

    for (int i = 0; i < 8; i++) {
        char potName[80];
        sprintf(potName, "pot%d", i);
        pref.putLong(potName, values[i]);
    }

    pref.putBool(CALIBRATION_KEY, true);
    ESP.restart();
}

void calibrate() {
    pref.begin(PREFERENCES_KEY, false);
    pref.clear();

    a0.begin();
    a0.setDataRate(7);

    a1.begin();
    a1.setDataRate(7);

    xTaskCreate(calibrateTask, "calibrateTask", 2048, NULL, 2, NULL);
}

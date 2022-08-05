#include <inputs.h>

ADS1115 adc0(ADC0);
ADS1115 adc1(ADC1);
QueueHandle_t q;
Preferences calibrationPreferences;

long pot0MaxValue, pot1MaxValue, pot2MaxValue, pot3MaxValue, pot4MaxValue, pot5MaxValue, pot6MaxValue, pot7MaxValue;

long mapValue(long value, long potMaxValue, int min, int max) {
    long v = value;
    if (value < 0) {
        v = 0;
    }
    if (value > potMaxValue) {
        v = potMaxValue;
    }

    long mapped = map(v, 0, potMaxValue + 1, min, max);
    if (mapped > max - 1) {
        return max - 1;
    }
    return mapped;
}

state readState() {
    long raw0 = adc0.readADC(0);
    long raw1 = adc0.readADC(1);
    long raw2 = adc0.readADC(2);
    long raw3 = adc0.readADC(3);
    long raw4 = adc1.readADC(0);

    long mapped0 = mapValue(raw0, pot0MaxValue, 0, 3);
    long mapped1 = mapValue(raw1, pot1MaxValue, 0, 256);
    long mapped2 = mapValue(raw2, pot2MaxValue, 0, 256);
    long mapped3 = mapValue(raw3, pot3MaxValue, 0, 256);
    long mapped4 = mapValue(raw4, pot4MaxValue, 0, 256);

    return {{static_cast<ledMode>(mapped0), mapped1, mapped2, mapped3, mapped4}, {raw0, raw1, raw2, raw3, raw4}};
}

void loop(void *pvParameters) {
    state currentState = readState();
    xQueueOverwrite(q, &currentState);

    for (;;) {
        state newState = readState();
        if (newState != currentState) {
            xQueueOverwrite(q, &newState);
            currentState = newState;
        }
    }
}

QueueHandle_t readADCs() {
    calibrationPreferences.begin(PREFERENCES_KEY);
    pot0MaxValue = calibrationPreferences.getLong("pot0");
    pot1MaxValue = calibrationPreferences.getLong("pot1");
    pot2MaxValue = calibrationPreferences.getLong("pot2");
    pot3MaxValue = calibrationPreferences.getLong("pot3");
    pot4MaxValue = calibrationPreferences.getLong("pot4");
    pot5MaxValue = calibrationPreferences.getLong("pot5");
    pot6MaxValue = calibrationPreferences.getLong("pot6");
    pot7MaxValue = calibrationPreferences.getLong("pot7");

    q = xQueueCreate(1, sizeof(struct state));

    adc0.begin();
    adc0.setDataRate(7);

    adc1.begin();
    adc1.setDataRate(7);

    xTaskCreate(loop, "loop", 2048, NULL, 2, NULL);

    return q;
}

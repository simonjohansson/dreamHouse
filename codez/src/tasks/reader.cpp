#include "reader.h"

long Reader::mapValue(long value, long potMaxValue, int min, int max) {
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

State Reader::readState() {
    long raw0 = adc0_.readADC(0);
    long raw1 = adc0_.readADC(1);
    long raw2 = adc0_.readADC(2);
    long raw3 = adc0_.readADC(3);
    long raw4 = adc1_.readADC(0);

    long mapped0 = mapValue(raw0, pot0MaxValue_, 0, 4);
    long mapped1 = mapValue(raw1, pot1MaxValue_, 0, 256);
    long mapped2 = mapValue(raw2, pot2MaxValue_, 0, 256);
    long mapped3 = mapValue(raw3, pot3MaxValue_, 0, 256);
    long mapped4 = mapValue(raw4, pot4MaxValue_, 0, 256);

    return {{static_cast<LedMode>(mapped0), mapped1, mapped2, mapped3, mapped4}, {raw0, raw1, raw2, raw3, raw4}};
}

void Reader::loop() {
    State currentState = readState();
    xQueueOverwrite(queue_, &currentState);

    for (;;) {
        State newState = readState();
        if (newState != currentState) {
            xQueueOverwrite(queue_, &newState);
            currentState = newState;
        }
    }
}

void Reader::startLoop(void *_this) { ((Reader *)_this)->loop(); }

void Reader::start() { xTaskCreate(Reader::startLoop, "readerLoop", 2048, this, 1, NULL); }

Reader::Reader(QueueHandle_t queue, ADS1115 adc0, ADS1115 adc1, Preferences *preferences) {
    queue_ = queue;
    adc0_ = adc0;
    adc1_ = adc1;

    pot0MaxValue_ = preferences->getLong("pot0");
    pot1MaxValue_ = preferences->getLong("pot1");
    pot2MaxValue_ = preferences->getLong("pot2");
    pot3MaxValue_ = preferences->getLong("pot3");
    pot4MaxValue_ = preferences->getLong("pot4");
    pot5MaxValue_ = preferences->getLong("pot5");
    pot6MaxValue_ = preferences->getLong("pot6");
    pot7MaxValue_ = preferences->getLong("pot7");
}
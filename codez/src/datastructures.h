#ifndef DATASTRUCTURES
#define DATASTRUCTURES

#include <Arduino.h>

struct rawValues {
    long pot1, pot2, pot3, pot4, pot5;

    bool operator==(const rawValues &other) const {
        return this->pot1 == other.pot1 && this->pot2 == other.pot2 && this->pot3 == other.pot3 && this->pot4 == other.pot4 && this->pot4 == other.pot5;
    }
    bool operator!=(const rawValues &other) const { return !(*this == other); }
};

enum ledMode { OFF, HSV_MODE, RGB_MODE };

struct ledState {
    ledMode mode;
    long pot1, pot2, pot3;
    long brightness;

    bool operator==(const ledState &other) const {
        return this->mode == other.mode && this->pot1 == other.pot1 && this->pot2 == other.pot2 && this->pot3 == other.pot3 &&
               this->brightness == other.brightness;
    }

    bool operator!=(const ledState &other) const { return !(*this == other); }
};

struct state {
    ledState led;
    rawValues raw;

    String printable() {
        char buffer[80];
        sprintf(buffer,
                "mode: %d (%d) pot1: %d (%d) pot2: %d (%d) pot3: %d (%d) "
                "brightness: %d (%d)",
                led.mode, raw.pot1, led.pot1, raw.pot2, led.pot2, raw.pot3, led.pot3, raw.pot4, led.brightness, raw.pot5);
        return buffer;
    }

    bool operator==(const state &other) const { return this->led == other.led; }

    bool operator!=(const state &other) const { return !(*this == other); }
};

#endif

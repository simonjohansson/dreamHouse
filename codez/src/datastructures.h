#ifndef DATASTRUCTURES
#define DATASTRUCTURES

#include <Arduino.h>

struct RawValues {
    long pot1, pot2, pot3, pot4, pot5;

    bool operator==(const RawValues &other) const {
        return this->pot1 == other.pot1 && this->pot2 == other.pot2 && this->pot3 == other.pot3 && this->pot4 == other.pot4 && this->pot4 == other.pot5;
    }
    bool operator!=(const RawValues &other) const { return !(*this == other); }
};

enum LedMode { OFF, HSV_MODE, RGB_MODE, SCAN_MODE, RANDOM_MODE, TRACE_MODE };

struct LedState {
    LedMode mode;
    long pot1, pot2, pot3;
    long brightness;

    bool operator==(const LedState &other) const {
        return this->mode == other.mode && this->pot1 == other.pot1 && this->pot2 == other.pot2 && this->pot3 == other.pot3 &&
               this->brightness == other.brightness;
    }

    bool operator!=(const LedState &other) const { return !(*this == other); }
};

struct PotState {
    LedState led;
    RawValues raw;

    String printable() {
        char buffer[80];
        sprintf(buffer,
                "mode: %d (%d) p1: %d (%d) p2: %d (%d) p3: %d (%d) "
                "bright: %d (%d)",
                led.mode, raw.pot1, led.pot1, raw.pot2, led.pot2, raw.pot3, led.pot3, raw.pot4, led.brightness, raw.pot5);
        return buffer;
    }

    bool operator==(const PotState &other) const { return this->led == other.led; }

    bool operator!=(const PotState &other) const { return !(*this == other); }
};

#endif

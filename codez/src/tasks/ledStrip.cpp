#include "ledStrip.h"

void rgb() {}

void hsv() {}

void LedStrip::loop() {
    for (;;) {
        State newState;
        if (xQueueReceive(queue_, &newState, portMAX_DELAY) == pdPASS) {
            for (int i = 0; i < 3; i++) {
                leds_[i] = CHSV(newState.led.pot1, 255, newState.led.brightness);
                FastLED.show();
            }
        }
    }
}

void LedStrip::startHSV(void *_this) { ((LedStrip *)_this)->hsv(); }
void LedStrip::startRGB(void *_this) { ((LedStrip *)_this)->rgb(); }
void LedStrip::startLoop(void *_this) { ((LedStrip *)_this)->loop(); }

void LedStrip::start() { xTaskCreate(LedStrip::startLoop, "LedStripLoop", 2048, this, 1, NULL); }

LedStrip::LedStrip(QueueHandle_t queue, CRGB leds[]) {
    queue_ = queue;
    leds_ = leds;
}
#include "ledStrip.h"

void LedStrip::loop() {
    for (;;) {
        state newState;
        if (xQueueReceive(queue_, &newState, portMAX_DELAY) == pdPASS) {
            for (int i = 0; i < 3; i++) {
                leds_[i] = CHSV(newState.led.pot1, 1, newState.led.brightness);
                FastLED.show();
            }
        }
    }
}

void LedStrip::startLoop(void *_this) { ((LedStrip *)_this)->loop(); }

void LedStrip::start() { xTaskCreate(LedStrip::startLoop, "LedStripLoop", 2048, this, 1, NULL); }

LedStrip::LedStrip(QueueHandle_t queue, CRGB leds[]) {
    queue_ = queue;
    leds_ = leds;
}
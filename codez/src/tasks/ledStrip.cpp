#include "ledStrip.h"

void LedStrip::rgb() {
    log_i("RGB Mode");
    FastLED.clear(true);
    for (;;) {
        for (int i = 0; i < NUM_LEDS; i++) {
            leds_[i] = CRGB(state_.led.pot1, state_.led.pot2, state_.led.pot3);
        }
        FastLED.setBrightness(state_.led.brightness);
        FastLED.show();
        vTaskDelay(50 / portTICK_RATE_MS);
    }
}

void LedStrip::hsv() {
    log_i("HSV Mode");
    FastLED.clear(true);
    FastLED.setBrightness(255);
    for (;;) {
        for (int i = 0; i < NUM_LEDS; i++) {
            leds_[i] = CHSV(state_.led.pot1, 255, state_.led.brightness);
        }
        FastLED.show();
        vTaskDelay(50 / portTICK_RATE_MS);
    }
}

void LedStrip::loop() {
    State oldState;
    TaskHandle_t handle = NULL;

    for (;;) {
        State newState;
        if (xQueueReceive(queue_, &newState, portMAX_DELAY) == pdPASS) {
            state_ = newState;

            if (newState.led.mode != oldState.led.mode) {
                if (handle != NULL) {
                    vTaskDelete(handle);
                    handle = NULL;
                }
                switch (newState.led.mode) {
                case OFF:
                    break;
                case HSV_MODE:
                    xTaskCreate(LedStrip::startHSV, "HSVLoop", 2048, this, 1, &handle);
                    break;
                case RGB_MODE:
                    xTaskCreate(LedStrip::startRGB, "RGBLoop", 2048, this, 1, &handle);
                    break;
                default:
                    break;
                }
                oldState = newState;
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
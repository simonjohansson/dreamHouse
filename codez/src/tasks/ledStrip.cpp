#include "ledStrip.h"

void LedStrip::random() {
    log_i("Random Mode");

    for (;;) {
        std::vector<int> black = {};
        std::vector<int> withColour = {};

        for (int i = 0; i < NUM_LEDS; i++) {
            if (leds_[i]) {
                withColour.push_back(i);
            } else {
                black.push_back(i);
            }
        }

        for (const auto &value : withColour) {
            leds_[value].fadeToBlackBy(1);
        }

        std::random_shuffle(black.begin(), black.end());
        black.resize(5 - withColour.size());
        for (const auto &value : black) {
            if (((rand() % 5) + 1) % 5 == 0) {
                leds_[value] = CRGB((rand() % 256), (rand() % 256), (rand() % 256));
            }
        }

        FastLED.show();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void LedStrip::scan() {
    log_i("Scan Mode");
    FastLED.clear(true);
    FastLED.setBrightness(255);
    long h = 0;
    long s = 255;
    bool sIncrement = false;
    for (;;) {
        for (int i = 0; i < NUM_LEDS; i++) {
            leds_[i] = CHSV(h, s, state_.led.brightness);
        }
        FastLED.show();
        h++;
        if (h > 255) {
            h = 0;
        }

        if (s == 255) {
            sIncrement = false;
        } else if (s == 200) {
            sIncrement = true;
        }

        if (sIncrement) {
            s++;
        } else {
            s--;
        }

        vTaskDelay(map(state_.led.pot1, 0, 255, 255, 0) / portTICK_RATE_MS);
    }
}

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
            leds_[i] = CHSV(state_.led.pot1, state_.led.pot2, state_.led.brightness);
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
                    FastLED.clear(true);
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
                case SCAN_MODE:
                    xTaskCreate(LedStrip::startScan, "ScanLoop", 2048, this, 1, &handle);
                    break;
                case RANDOM_MODE:
                    xTaskCreate(LedStrip::startRandom, "RandomLoop", 2048, this, 1, &handle);
                    break;
                default:
                    break;
                }
                oldState = newState;
            }
        }
    }
}

void LedStrip::startRandom(void *_this) { ((LedStrip *)_this)->random(); }
void LedStrip::startScan(void *_this) { ((LedStrip *)_this)->scan(); }
void LedStrip::startHSV(void *_this) { ((LedStrip *)_this)->hsv(); }
void LedStrip::startRGB(void *_this) { ((LedStrip *)_this)->rgb(); }
void LedStrip::startLoop(void *_this) { ((LedStrip *)_this)->loop(); }

void LedStrip::start() { xTaskCreate(LedStrip::startLoop, "LedStripLoop", 2048, this, 1, NULL); }

LedStrip::LedStrip(QueueHandle_t queue, CRGB leds[]) {
    queue_ = queue;
    leds_ = leds;
}
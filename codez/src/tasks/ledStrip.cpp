#include "ledStrip.h"

void LedStrip::trace() {
    log_i("Trace Mode");

    int i = 0;
    for (;;) {

        for (int j = 0; j <= i; j++) {
            leds_[j] = CRGB::Purple;
            for (int k = j; k >= 0; k--) {
                leds_[k].fadeToBlackBy(8);
            }
        }
        for (int j = i + 1; j < NUM_LEDS * NUM_STRIPS; j++) {
            leds_[j].fadeToBlackBy(8);
        }
        show();
        i++;
        if (i > NUM_LEDS * NUM_STRIPS) {
            i = 0;
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void LedStrip::random() {
    log_i("Random Mode");

    for (;;) {
        std::vector<int> black = {};
        std::vector<int> withColour = {};

        for (int i = 0; i < NUM_LEDS * NUM_STRIPS; i++) {
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
        black.resize(20 - withColour.size());
        for (const auto &value : black) {
            if (((esp_random() % 5) + 1) % 5 == 0) {
                leds_[value] = CRGB((esp_random() % 256), (esp_random() % 256), (esp_random() % 256));
            }
        }
        show();
        vTaskDelay(20 / portTICK_PERIOD_MS);
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
        for (int i = 0; i < NUM_LEDS * NUM_STRIPS; i++) {
            leds_[i] = CHSV(h, s, 255);
        }
        this->withLock([&]() { FastLED.show(); });
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

        vTaskDelay(20 / portTICK_RATE_MS);
    }
}

void LedStrip::rgb() {
    log_i("RGB Mode");
    FastLED.clear(true);
    for (;;) {
        for (int i = 0; i < NUM_LEDS * NUM_STRIPS; i++) {
            leds_[i] = CRGB(state_.led.pot1, state_.led.pot2, state_.led.pot3);
        }
        FastLED.setBrightness(state_.led.brightness);
        show();
        vTaskDelay(50 / portTICK_RATE_MS);
    }
}

void LedStrip::hsv() {
    log_i("HSV Mode");
    FastLED.clear(true);
    FastLED.setBrightness(255);
    for (;;) {
        for (int i = 0; i < NUM_LEDS * NUM_STRIPS; i++) {
            leds_[i] = CHSV(state_.led.pot1, state_.led.pot2, state_.led.brightness);
        }
        show();
        vTaskDelay(50 / portTICK_RATE_MS);
    }
}

void LedStrip::withLock(const std::function<void()> &f) {
    xSemaphoreTake(lock_, portMAX_DELAY);
    f();
    xSemaphoreGive(lock_);
}

void LedStrip::show() {
    this->withLock([&]() { FastLED.show(); });
}

void LedStrip::loop() {
    PotState oldState;
    TaskHandle_t handle = NULL;

    for (;;) {
        PotState newState;
        if (xQueueReceive(queue_, &newState, portMAX_DELAY) == pdPASS) {
            state_ = newState;

            if (newState.led.mode != oldState.led.mode) {
                if (handle != NULL) {
                    this->withLock([&]() {
                        vTaskDelete(handle);
                        handle = NULL;
                        FastLED.clear(true);
                    });
                }
                switch (newState.led.mode) {
                case OFF:
                    break;
                case HSV_MODE:
                    xTaskCreatePinnedToCore(LedStrip::startHSV, "HSVLoop", 2048, this, 4, &handle, 1);
                    break;
                case RGB_MODE:
                    xTaskCreatePinnedToCore(LedStrip::startRGB, "RGBLoop", 2048, this, 4, &handle, 1);
                    break;
                case SCAN_MODE:
                    xTaskCreatePinnedToCore(LedStrip::startScan, "ScanLoop", 2048, this, 4, &handle, 1);
                    break;
                case RANDOM_MODE:
                    xTaskCreatePinnedToCore(LedStrip::startRandom, "RandomLoop", 2048, this, 4, &handle, 1);
                    break;
                case TRACE_MODE:
                    xTaskCreatePinnedToCore(LedStrip::startTrace, "TraceLoop", 2048, this, 4, &handle, 1);
                    break;
                default:
                    break;
                }
                oldState = newState;
            }
        }
    }
}

void LedStrip::startTrace(void *_this) { ((LedStrip *)_this)->trace(); }
void LedStrip::startRandom(void *_this) { ((LedStrip *)_this)->random(); }
void LedStrip::startScan(void *_this) { ((LedStrip *)_this)->scan(); }
void LedStrip::startHSV(void *_this) { ((LedStrip *)_this)->hsv(); }
void LedStrip::startRGB(void *_this) { ((LedStrip *)_this)->rgb(); }
void LedStrip::startLoop(void *_this) { ((LedStrip *)_this)->loop(); }

void LedStrip::start() { xTaskCreatePinnedToCore(LedStrip::startLoop, "LedStripLoop", 2048, this, 3, NULL, 1); }

LedStrip::LedStrip(QueueHandle_t queue, CRGB leds[]) {
    queue_ = queue;
    leds_ = leds;
    lock_ = xSemaphoreCreateMutex();
}
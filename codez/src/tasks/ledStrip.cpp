#include "ledStrip.h"

void LedStrip::loop() {
    state oldState;
    TaskHandle_t loopHandle = NULL;
    for (;;) {
        state newState;
        if (xQueueReceive(queue_, &newState, portMAX_DELAY) == pdPASS) {
            if (newState != state_) {
                state_ = newState;
            }

            if (newState.led.mode != oldState.led.mode) {
                if (loopHandle != NULL) {
                    vTaskDelete(loopHandle);
                    loopHandle = NULL;
                }
                switch (newState.led.mode) {
                case OFF:
                    log_i("OFF");
                    break;
                case HSV_MODE:
                    log_i("HSV");
                    break;
                case RGB_MODE:
                    log_i("RGB");
                    break;
                default:
                    break;
                }
                oldState = newState;
            }
        }
    }
}

void LedStrip::startLoop(void *_this) { ((LedStrip *)_this)->loop(); }

void LedStrip::start() { xTaskCreate(LedStrip::startLoop, "LedStripLoop", 2048, this, 1, NULL); }

LedStrip::LedStrip(QueueHandle_t queue) { queue_ = queue; }
#include "display.h"

void Display::loop() {
    oled_->clearDisplay();
    oled_->setTextColor(SSD1306_WHITE); // Draw white text
    oled_->display();
    oled_->setTextSize(2);
    for (;;) {
        State newState;
        if (xQueueReceive(queue_, &newState, portMAX_DELAY) == pdPASS) {
            oled_->clearDisplay();
            oled_->setCursor(0, 0);
            switch (newState.led.mode) {
            case OFF:
                oled_->printf("Lights off\n");
                break;
            case HSV_MODE:
                oled_->printf("HSV\n%d, %d, %d", newState.led.pot1, newState.led.pot2, newState.led.brightness);
                break;
            case RGB_MODE:
                oled_->printf("RGB\nR %d, %d, %d\nBrightness %d", newState.led.pot1, newState.led.pot2, newState.led.pot3, newState.led.brightness);
                break;
            case SCAN_MODE:
                oled_->printf("Scan\nSpeed %d%%", map(newState.led.pot1, 0, 255, 0, 100));
                break;
            case RANDOM_MODE:
                oled_->printf("Random");
                break;
            default:
                break;
            }
            oled_->display();
        }
    }
}

void Display::startLoop(void *_this) { ((Display *)_this)->loop(); }

void Display::start() { xTaskCreate(Display::startLoop, "displayLoop", 2048, this, 1, NULL); }

Display::Display(QueueHandle_t queue, Adafruit_SSD1306 *oled) {
    queue_ = queue;
    oled_ = oled;
}
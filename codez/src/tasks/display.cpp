#include "display.h"

void Display::loop() {
    oled_->clearDisplay();
    oled_->setTextSize(2, 2);           // Normal 1:1 pixel scale
    oled_->setTextColor(SSD1306_WHITE); // Draw white text
    oled_->display();

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
                oled_->printf("HSV\nH(%d)\nS(%d)\nV(%d)\n", newState.led.pot1, 255, newState.led.brightness);
                break;
            case RGB_MODE:
                oled_->printf("RGB\nR(%d)\tB(%d)\nG(%d)\nB(%d)", newState.led.pot1, newState.led.brightness, newState.led.pot2, newState.led.pot3);
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
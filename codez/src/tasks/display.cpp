#include "display.h"

void Display::loop() {
    const char *stateStr[] = {"Off", "HSV", "RGB"};

    oled_->clearDisplay();
    oled_->setTextSize(2, 2);           // Normal 1:1 pixel scale
    oled_->setTextColor(SSD1306_WHITE); // Draw white text
    oled_->display();

    for (;;) {
        state newState;
        if (xQueueReceive(queue_, &newState, portMAX_DELAY) == pdPASS) {
            oled_->clearDisplay();
            oled_->setCursor(0, 0);
            oled_->printf("%s\n%d, %d, %d\nBr: %d", stateStr[newState.led.mode], newState.led.pot1, 255, 255, newState.led.brightness);
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
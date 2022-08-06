#ifndef DISPLAY_TASK
#define DISPLAY_TASK
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <datastructures.h>

class Display {
  private:
    QueueHandle_t queue_;
    Adafruit_SSD1306 *oled_;

    static void startLoop(void *pvParams);
    void loop();

  public:
    Display(QueueHandle_t queue, Adafruit_SSD1306 *oled);
    void start();
};

#endif
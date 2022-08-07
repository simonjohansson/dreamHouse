#ifndef LEDSTRIP_TASK
#define LEDSTRIP_TASK
#include <Arduino.h>
#include <FastLED.h>
#include <consts.h>
#include <datastructures.h>

class LedStrip {
  private:
    QueueHandle_t queue_;
    CRGB *leds_;
    State state;

    void rgb();
    static void startRGB(void *pvParams);

    void hsv();
    static void startHSV(void *pvParams);

    static void startLoop(void *pvParams);
    void loop();

  public:
    LedStrip(QueueHandle_t queue, CRGB leds[]);
    void start();
};

#endif
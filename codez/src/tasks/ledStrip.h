#ifndef LEDSTRIP_TASK
#define LEDSTRIP_TASK
#include <Arduino.h>
#include <FastLED.h>
#include <algorithm>
#include <consts.h>
#include <datastructures.h>
#include <vector>

class LedStrip {
  private:
    QueueHandle_t queue_;
    SemaphoreHandle_t lock_;
    CRGB *leds_;
    State state_;

    void trace();
    static void startTrace(void *pvParams);

    void random();
    static void startRandom(void *pvParams);

    void scan();
    static void startScan(void *pvParams);

    void rgb();
    static void startRGB(void *pvParams);

    void hsv();
    static void startHSV(void *pvParams);

    static void startLoop(void *pvParams);
    void loop();

    void withLock(const std::function<void()> &f);

  public:
    LedStrip(QueueHandle_t queue, CRGB leds[]);
    void start();
};

#endif
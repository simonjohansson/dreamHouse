#ifndef READER_TASK
#define READER_TASK
#include <ADS1X15.h>
#include <Arduino.h>
#include <Preferences.h>
#include <SPI.h>
#include <consts.h>
#include <datastructures.h>

class Reader {
  private:
    QueueHandle_t queue_;
    ADS1115 adc0_;
    ADS1115 adc1_;
    long pot0MaxValue_, pot1MaxValue_, pot2MaxValue_, pot3MaxValue_, pot4MaxValue_, pot5MaxValue_, pot6MaxValue_, pot7MaxValue_;

    PotState readState();
    long mapValue(long value, long potMaxValue, int min, int max);
    static void startLoop(void *pvParams);
    void loop();

  public:
    Reader(QueueHandle_t queue, ADS1115 adc0, ADS1115 adc1, Preferences *preferences);
    void start();
};

#endif
#ifndef MULTIPLEXER_TASK
#define MULTIPLEXER_TASK

#include <Arduino.h>
#include <datastructures.h>

class Multiplexer {
  private:
    QueueHandle_t input_;
    QueueHandle_t output1_;
    QueueHandle_t output2_;
    QueueHandle_t output3_;

    static void startLoop(void *pvParams);
    void loop();

  public:
    Multiplexer(QueueHandle_t input, QueueHandle_t output1, QueueHandle_t output2, QueueHandle_t output3);
    void start();
};

#endif

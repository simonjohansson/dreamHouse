#ifndef PRINTER_TASK
#define PRINTER_TASK
#include <Arduino.h>
#include <datastructures.h>

class Printer {
  private:
    QueueHandle_t queue_;
    static void startLoop(void *pvParams);
    void loop();

  public:
    Printer(QueueHandle_t queue);
    void start();
};

#endif
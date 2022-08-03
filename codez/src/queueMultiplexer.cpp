#include <queueMultiplexer.h>

QueueHandle_t i;
QueueHandle_t p;
QueueHandle_t l;

void multiplexLoop(void *pvParameters) {
  for (;;) {
    state state;
    if (xQueueReceive(i, &state, portMAX_DELAY) == pdPASS) {
      xQueueOverwrite(p, &state);
      xQueueOverwrite(l, &state);
    }
  }
}

void multiplex(QueueHandle_t input, QueueHandle_t printer, QueueHandle_t led) {
  i = input;
  p = printer;
  l = led;

  xTaskCreate(multiplexLoop, "multiplexLoop", 2048, NULL, 2, NULL);
}

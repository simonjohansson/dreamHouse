#include <queueMultiplexer.h>

QueueHandle_t i;
QueueHandle_t qs[NUM_CONSUMERS];

void multiplexLoop(void *pvParameters) {
  for (;;) {
    state state;
    if (xQueueReceive(i, &state, portMAX_DELAY) == pdPASS) {
      for (int i = 0; i < NUM_CONSUMERS; i++) {
        xQueueOverwrite(qs[i], &state);
      }
    }
  }
}

void multiplex(QueueHandle_t input, QueueHandle_t queues[]) {
  i = input;

  for (int i = 0; i < NUM_CONSUMERS; i++) {
    qs[i] = queues[i];
  }

  xTaskCreate(multiplexLoop, "multiplexLoop", 2048, NULL, 2, NULL);
}

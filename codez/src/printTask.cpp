#include <printTask.h>

QueueHandle_t pq;

void print(void *pvParameters) {
  for (;;) {
    state newState;
    if (xQueueReceive(pq, &newState, portMAX_DELAY) == pdPASS) {
      Serial.println(newState.printable());
    }
  }
}

QueueHandle_t printLoop() {
  pq = xQueueCreate(1, sizeof(struct state));
  xTaskCreate(print, "print", 2048, NULL, 2, NULL);
  return pq;
}

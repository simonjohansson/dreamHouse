#include <Arduino.h>
#include <SPI.h>
#include <consts.h>
#include <inputs.h>
#include <queueMultiplexer.h>

#include "ADS1X15.h"

QueueHandle_t printerQueue;
QueueHandle_t ledQueue;

void print1(void *pvParameters) {
  for (;;) {
    state newState;
    if (xQueueReceive(printerQueue, &newState, portMAX_DELAY) == pdPASS) {
      Serial.println(newState.printable());
    }
  }
}

void setup() {
  Serial.begin(115200);

  QueueHandle_t eventQueue = xQueueCreate(1, sizeof(struct state));
  readADCs(eventQueue);

  ledQueue = xQueueCreate(1, sizeof(struct state));
  printerQueue = xQueueCreate(1, sizeof(struct state));
  QueueHandle_t consumerQueues[NUM_CONSUMERS] = {ledQueue, printerQueue};
  multiplex(eventQueue, consumerQueues);

  xTaskCreate(print1, "print1", 2048, NULL, 2, NULL);
  pinMode(BUILTIN_LED, OUTPUT);
}

void loop() {
  digitalWrite(BUILTIN_LED, HIGH);
  vTaskDelay(500);
  digitalWrite(BUILTIN_LED, LOW);
  vTaskDelay(500);
}
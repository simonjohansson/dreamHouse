#include <ADS1X15.h>
#include <Arduino.h>
#include <FastLED.h>
#include <SPI.h>
#include <consts.h>
#include <inputs.h>
#include <ledTask.h>
#include <printTask.h>
#include <queueMultiplexer.h>

QueueHandle_t ledQueue;

void setup() {
  Serial.begin(115200);

  QueueHandle_t eventQueue = readADCs();
  QueueHandle_t printQueue = printLoop();
  QueueHandle_t ledQueue = ledLoop();
  multiplex(eventQueue, printQueue, ledQueue);
}

void loop() {}
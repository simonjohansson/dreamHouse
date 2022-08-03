#ifndef QUEUE_MULTIPLEXER
#define QUEUE_MULTIPLEXER

#include <Arduino.h>
#include <datastructures.h>

void multiplex(QueueHandle_t input, QueueHandle_t printer, QueueHandle_t led);

#endif

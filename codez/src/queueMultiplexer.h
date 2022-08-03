#ifndef QUEUE_MULTIPLEXER
#define QUEUE_MULTIPLEXER

#include <Arduino.h>
#include <consts.h>
#include <datastructures.h>

void multiplex(QueueHandle_t input, QueueHandle_t queues[]);

#endif

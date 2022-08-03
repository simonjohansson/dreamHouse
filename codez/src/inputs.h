#ifndef INPUTS
#define INPUTS

#include <Arduino.h>
#include <SPI.h>
#include <datastructures.h>

#include "ADS1X15.h"

QueueHandle_t readADCs();

#endif

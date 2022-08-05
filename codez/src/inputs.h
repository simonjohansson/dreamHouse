#ifndef INPUTS
#define INPUTS

#include <ADS1X15.h>
#include <Arduino.h>
#include <Preferences.h>
#include <SPI.h>
#include <consts.h>
#include <datastructures.h>

QueueHandle_t readADCs();

#endif

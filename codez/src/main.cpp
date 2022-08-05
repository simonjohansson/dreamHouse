#include <ADS1X15.h>
#include <Arduino.h>
#include <FastLED.h>
#include <Preferences.h>
#include <SPI.h>
#include <calibrate.h>
#include <consts.h>
#include <inputs.h>
#include <ledTask.h>
#include <printTask.h>
#include <queueMultiplexer.h>

Preferences preferences;

void setup() {
  Serial.begin(115200);
  preferences.begin(PREFERENCES_KEY, false);
  pinMode(CALIBRATION_PIN, INPUT);

  vTaskDelay(500 / portTICK_RATE_MS);

  if (!preferences.getBool(CALIBRATION_KEY, false) ||
      digitalRead(CALIBRATION_PIN) == HIGH) {
    calibrate();
  } else {
    Serial.println("Calibrated values:");
    Serial.printf("%d, %d, %d, %d, %d, %d, %d, %d\n",
                  preferences.getLong("pot0"), preferences.getLong("pot1"),
                  preferences.getLong("pot2"), preferences.getLong("pot3"),
                  preferences.getLong("pot4"), preferences.getLong("pot5"),
                  preferences.getLong("pot6"), preferences.getLong("pot7"));
    QueueHandle_t eventQueue = readADCs();
    QueueHandle_t printQueue = printLoop();
    QueueHandle_t ledQueue = ledLoop();
    multiplex(eventQueue, printQueue, ledQueue);
  }
}

void loop() {}
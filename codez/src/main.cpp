#include <ADS1X15.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <FastLED.h>
#include <Preferences.h>
#include <SPI.h>
#include <Wire.h>
#include <consts.h>
#include <ledTask.h>
#include <tasks/calibrator.h>
#include <tasks/display.h>
#include <tasks/ledStrip.h>
#include <tasks/multiplexer.h>
#include <tasks/printer.h>
#include <tasks/reader.h>

Calibrator *calibrator;
Printer *printer;
Preferences *preferences;
Multiplexer *multiplexer;
Reader *reader;
LedStrip *ledStrip;
Display *display;

ADS1115 adc0(ADC0);
ADS1115 adc1(ADC1);

QueueHandle_t eventQueue;
QueueHandle_t printQueue;
QueueHandle_t ledQueue;
QueueHandle_t displayQueue;

Adafruit_SSD1306 *oled;

void setup() {
    Serial.begin(115200);

    preferences = new Preferences();
    preferences->begin(PREFERENCES_KEY, false);

    pinMode(CALIBRATION_PIN, INPUT);

    adc0.begin();
    adc0.setDataRate(7);
    adc1.begin();
    adc1.setDataRate(7);

    oled = new Adafruit_SSD1306(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET);
    oled->begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);

    eventQueue = xQueueCreate(1, sizeof(struct state));
    printQueue = xQueueCreate(1, sizeof(struct state));
    ledQueue = xQueueCreate(1, sizeof(struct state));
    displayQueue = xQueueCreate(1, sizeof(struct state));

    vTaskDelay(500 / portTICK_RATE_MS);
    if (!preferences->getBool(CALIBRATION_KEY, false) || digitalRead(CALIBRATION_PIN) == HIGH) {
        calibrator = new Calibrator(adc0, adc1, preferences);
        calibrator->calibrate();
    } else {
        log_i("Calibrated values: %d, %d, %d, %d, %d, %d, %d, %d", preferences->getLong("pot0"), preferences->getLong("pot1"), preferences->getLong("pot2"),
              preferences->getLong("pot3"), preferences->getLong("pot4"), preferences->getLong("pot5"), preferences->getLong("pot6"),
              preferences->getLong("pot7"));

        reader = new Reader(eventQueue, adc0, adc1, preferences);
        multiplexer = new Multiplexer(eventQueue, printQueue, ledQueue, displayQueue);
        printer = new Printer(printQueue);
        ledStrip = new LedStrip(ledQueue);
        display = new Display(displayQueue, oled);

        printer->start();
        ledStrip->start();
        display->start();
        multiplexer->start();
        reader->start();
    }
}

void loop() {}
#include <ledTask.h>

QueueHandle_t lq;

state s;

CRGB leds[NUM_LEDS];

void cleanColour() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}

void hsvLoop(void *pvParameters) {
  for (;;) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(s.led.pot1, s.led.pot2, 255);
    }
    FastLED.show();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void rgbLoop(void *pvParameters) {
  Serial.println("Starting rgb");
  for (;;) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(s.led.pot1, s.led.pot2, 123);
    }
    FastLED.show();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void led(void *pvParameters) {
  cleanColour();

  int numHandles = 2;
  TaskHandle_t hsvHandle = NULL;
  TaskHandle_t rgbHandle = NULL;
  state oldState;
  for (;;) {
    state newState;
    if (xQueueReceive(lq, &newState, portMAX_DELAY) == pdPASS) {
      if (newState != s) {
        s = newState;
      }
      if (newState.led.mode != oldState.led.mode) {
        if (hsvHandle != NULL) {
          vTaskDelete(hsvHandle);
          hsvHandle = NULL;
        }
        if (rgbHandle != NULL) {
          vTaskDelete(rgbHandle);
          rgbHandle = NULL;
        }

        switch (newState.led.mode) {
          case OFF:
            cleanColour();
            break;
          case HSV_MODE:
            cleanColour();
            xTaskCreate(hsvLoop, "hsv", 2048, NULL, 2, &hsvHandle);
            break;
          case RGB_MODE:
            cleanColour();
            xTaskCreate(rgbLoop, "rgb", 2048, NULL, 2, &rgbHandle);
            break;
          default:
            break;
        }
        oldState = newState;
      }
    }
  }
}

QueueHandle_t ledLoop() {
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  lq = xQueueCreate(1, sizeof(struct state));
  xTaskCreate(led, "led", 2048, NULL, 2, NULL);
  return lq;
}

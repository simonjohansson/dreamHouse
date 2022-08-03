#include <ledTask.h>

QueueHandle_t lq;

CRGB leds[NUM_LEDS];

void led(void *pvParameters) {
  for (;;) {
    state newState;
    if (xQueueReceive(lq, &newState, portMAX_DELAY) == pdPASS) {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV(newState.led.pot1, newState.led.pot2, 255);
      }
      FastLED.show();
    }
  }
}

QueueHandle_t ledLoop() {
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  lq = xQueueCreate(1, sizeof(struct state));
  xTaskCreate(led, "led", 2048, NULL, 2, NULL);
  return lq;
}

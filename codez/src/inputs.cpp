#include <inputs.h>

ADS1115 adc0(0x48);
ADS1115 adc1(0x49);
QueueHandle_t q;

long mapValue(long value, int min, int max) {
  long v = value;
  if (value < 0) {
    v = 0;
  }
  if (value > 26370) {
    v = 26379;
  }

  long mapped = map(v, 0, 26370, min, max);
  if (mapped > max - 1) {
    return max - 1;
  }
  return mapped;
}

state readState() {
  long raw0 = adc0.readADC(0);
  long raw1 = adc0.readADC(1);
  long raw2 = adc0.readADC(2);
  long raw3 = adc0.readADC(3);
  long raw4 = adc0.readADC(3);

  long mapped0 = mapValue(raw0, 0, 3);
  long mapped1 = mapValue(raw1, 0, 256);
  long mapped2 = mapValue(raw2, 0, 256);
  long mapped3 = mapValue(raw3, 0, 256);
  long mapped4 = mapValue(raw3, 0, 256);
  return {{static_cast<ledMode>(mapped0), mapped1, mapped2, mapped3, mapped4},
          {raw0, raw1, raw2, raw3}};
}

void loop(void *pvParameters) {
  state currentState = readState();
  xQueueOverwrite(q, &currentState);

  for (;;) {
    state newState = readState();
    if (newState != currentState) {
      xQueueOverwrite(q, &newState);
      currentState = newState;
    }
  }
}

void readADCs(QueueHandle_t queue) {
  q = queue;

  adc0.begin();
  adc0.setGain(1);
  adc0.setDataRate(7);

  xTaskCreate(loop, "loop", 2048, NULL, 2, NULL);
}

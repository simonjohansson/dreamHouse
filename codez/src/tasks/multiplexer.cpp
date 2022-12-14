#include "multiplexer.h"

void Multiplexer::loop() {
    for (;;) {
        PotState state;
        if (xQueueReceive(input_, &state, portMAX_DELAY) == pdPASS) {
            xQueueOverwrite(output1_, &state);
            xQueueOverwrite(output2_, &state);
            xQueueOverwrite(output3_, &state);
        }
    }
}

void Multiplexer::startLoop(void *_this) { ((Multiplexer *)_this)->loop(); }

void Multiplexer::start() { xTaskCreate(Multiplexer::startLoop, "multiplexerLoop", 2048, this, 1, NULL); }

Multiplexer::Multiplexer(QueueHandle_t input, QueueHandle_t output1, QueueHandle_t output2, QueueHandle_t output3) {
    input_ = input;
    output1_ = output1;
    output2_ = output2;
    output3_ = output3;
}
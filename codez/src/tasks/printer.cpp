#include "printer.h"

void Printer::loop() {
    for (;;) {
        State newState;
        if (xQueueReceive(queue_, &newState, portMAX_DELAY) == pdPASS) {
            log_i("%s", newState.printable().c_str());
        }
    }
}

void Printer::startLoop(void *_this) { ((Printer *)_this)->loop(); }

void Printer::start() { xTaskCreate(Printer::startLoop, "printerLoop", 2048, this, 1, NULL); }

Printer::Printer(QueueHandle_t queue) { queue_ = queue; }
#include "pico/stdlib.h"
#include <stdio.h>

#define BUTTON_PIN 15

void button_callback(uint gpio, uint32_t events) {
    printf("PRESSED\n");
}

int main() {
    stdio_init_all();

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);

    printf("Starting...\n");

    while (true) {
        tight_loop_contents();
    }
}
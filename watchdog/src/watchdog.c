#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/watchdog.h"

#define BUTTON_PIN 15
#define DELAY_MS 250
#define STARTUP_DELAY_MS 5000
#define WATCHDOG_FEED_TIME_MS 2000

void button_callback(uint gpio, uint32_t events) {
    while (1){
        printf("No dog food!\n");
        sleep_ms(DELAY_MS);
    }
}

int init() {
    stdio_init_all();
    if (watchdog_caused_reboot()) {
        printf("Recovered from watchdog reset\n");
    } else {
        printf("Starting...\n");
    }
    watchdog_enable(WATCHDOG_FEED_TIME_MS, 1);

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);
}

int main() {
    sleep_ms(STARTUP_DELAY_MS);
    init();

    while (1) {
        printf("Feeding dog\n");
        watchdog_update();
        sleep_ms(DELAY_MS);
    }
}

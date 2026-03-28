#include "pico/stdlib.h"
#include <stdio.h>

#ifndef LOG_DELAY_MS
#define LOG_DELAY_MS 250
#endif


int main() {
    stdio_init_all();
    printf("Starting...\n");
    while (true) {
        printf("LED state: %d\n", true);
        sleep_ms(LOG_DELAY_MS);
        printf("LED state: %d\n", false);
        sleep_ms(LOG_DELAY_MS);
    }
}

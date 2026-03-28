#include "pico/stdlib.h"

#define LOG_DELAY_MS 250
#define BUTTON_PIN 15

int main() {
    stdio_init_all();

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    printf("Starting...\n");

    while (true) {
        bool pressed = !gpio_get(BUTTON_PIN); // active-low: pin reads 0 when pressed
        printf("Button: %s\n", pressed ? "PRESSED" : "released");
        sleep_ms(LOG_DELAY_MS);
    }
}
#include "pico/stdlib.h"
#include "pico/util/queue.h"
#include <stdio.h>

#define BTN_A_PIN 14
#define BTN_B_PIN 15

#define DELAY_MS 250
#define STARTUP_DELAY_MS 2000

// Queue for events
queue_t q;

typedef struct {
    uint gpio;
    uint32_t events;
} event_t;

// --------------------
// Slow process (NOT in ISR)
// --------------------
void slow_process(uint gpio) {
    printf("Start from GPIO %d\n", gpio);

    printf("Step A\n"); sleep_ms(DELAY_MS);
    printf("Step B\n"); sleep_ms(DELAY_MS);
    printf("Step C\n"); sleep_ms(DELAY_MS);
    printf("Step D\n"); sleep_ms(DELAY_MS);

    printf("Done\n");
}

// --------------------
// Interrupt handler
// --------------------
void gpio_isr(uint gpio, uint32_t events) {
    printf("GPIO %d pressed\n",  gpio);
    event_t e = {
        .gpio = gpio,
        .events = events
    };
    slow_process(gpio);
    // Non-blocking push (drops if full)
    //queue_try_add(&q, &e);
}

// --------------------
// Init helpers
// --------------------
void init_btn(int pin) {
    printf("Initializing GPIO %d\n", pin);
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin);
}

void init_all() {
    printf("Initializing system\n");

    queue_init(&q, sizeof(event_t), 16);

    init_btn(BTN_A_PIN);
    init_btn(BTN_B_PIN);

    // Register ISR once
    gpio_set_irq_enabled_with_callback(
        BTN_A_PIN,
        GPIO_IRQ_EDGE_FALL,
        true,
        &gpio_isr
    );

    // Enable second button IRQ
    gpio_set_irq_enabled_with_callback(
        BTN_B_PIN,
        GPIO_IRQ_EDGE_FALL,
        true,
        &gpio_isr
    );
}

// --------------------
// Main
// --------------------
int main() {
    stdio_init_all();
    sleep_ms(STARTUP_DELAY_MS);

    init_all();

    while (1) {
        /*
        event_t e;

        if (queue_try_remove(&q, &e)) {
            switch (e.gpio) {
            case BTN_A_PIN:
                printf("GPIO %d handled\n", e.gpio);
                slow_process(e.gpio);
                break;
            case BTN_B_PIN:
                printf("GPIO %d handled\n", e.gpio);
                slow_process(e.gpio);
                break;
            default:
                break;
            }
            */
        }

        tight_loop_contents();
    }
}
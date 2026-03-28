#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/tcp.h"
#include "lwip/dns.h"
#include <stdio.h>
#include <string.h>

#define BUTTON_PIN  15
#define SERVER_HOST "192.168.1.8"
#define SERVER_PORT 8050
#define SERVER_PATH "/endpoint"

static volatile bool button_pressed = false;

void button_callback(uint gpio, uint32_t events) {
    printf("Button Pressed\n");
    button_pressed = true;
}

void get_mac_string(char *buf) {
    uint8_t mac[6];
    cyw43_wifi_get_mac(&cyw43_state, CYW43_ITF_STA, mac);
    snprintf(buf, 18, "%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

void send_http_post(const char *mac) {
    char body[64];
    snprintf(body, sizeof(body), "{\"id\": \"%s\"}", mac);

    char request[256];
    snprintf(request, sizeof(request),
        "POST %s HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
        SERVER_PATH, SERVER_HOST, (int)strlen(body), body);

    struct tcp_pcb *pcb = tcp_new();
    ip_addr_t server_ip;

    if (dns_gethostbyname(SERVER_HOST, &server_ip, NULL, NULL) == ERR_OK) {
        tcp_connect(pcb, &server_ip, SERVER_PORT, NULL);
        tcp_write(pcb, request, strlen(request), TCP_WRITE_FLAG_COPY);
        tcp_output(pcb);
    }
}

int main() {
    stdio_init_all();

    if (cyw43_arch_init_with_country(CYW43_COUNTRY_SWEDEN)) {
        printf("WiFi init failed\n");
        return 1;
    }

    cyw43_arch_enable_sta_mode();
    printf("Connecting to WiFi...\n");

    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD,
                                            CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("WiFi connection failed\n");
        printf("ssid %s \n",WIFI_SSID);
        printf("psk %s \n",WIFI_PASSWORD);
        return 1;
    }

    printf("Connected!\n");

    // Print assigned IP address
    uint8_t *ip = (uint8_t *)&cyw43_state.netif[CYW43_ITF_STA].ip_addr.addr;
    printf("IP: %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);
    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);

    char mac[18];
    get_mac_string(mac);
    printf("MAC: %s\n", mac);
    printf("Ready — press button to send POST\n");

    while (true) {
        if (button_pressed) {
            button_pressed = false;
            printf("Sending POST...\n");
            send_http_post(mac);
        }
        cyw43_arch_poll();
        sleep_ms(10);
    }
}
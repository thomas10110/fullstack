#include "http_server.h"
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <windows.h>
    #include <stdio.h>
    #define SLEEP_MS(ms) Sleep(ms)
#else
    #include <unistd.h>
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <stdio.h>
    #define SLEEP_MS(ms) usleep((ms)*1000)
#endif

// Minimal sensor simulation (optimized for ESP32)
static float g_temp = 22.0f, g_water = 50.0f;

static inline void update_sensors() {
    // Lightweight random walk
    g_temp += (rand() & 1 ? 0.5f : -0.5f);
    if (g_temp < 18.0f) g_temp = 18.0f;
    if (g_temp > 28.0f) g_temp = 28.0f;
    
    g_water += (rand() & 1 ? 1.0f : -1.0f);
    if (g_water < 30.0f) g_water = 30.0f;
    if (g_water > 70.0f) g_water = 70.0f;
    
    http_setTemperature(g_temp);
    http_setWater(g_water);
}

int main() {
    srand((unsigned)time(NULL));
    
    if (!http_server_init()) {
        return 1;
    }
    
#ifndef ESP_PLATFORM
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        struct hostent* host = gethostbyname(hostname);
        if (host && host->h_addr_list[0]) {
            struct in_addr addr;
            memcpy(&addr, host->h_addr_list[0], sizeof(addr));
            printf("http://%s:%d\n", inet_ntoa(addr), HTTP_PORT);
        }
    }
#endif
    
    // Main loop
    while (1) {
        update_sensors();
        http_server_poll();
        SLEEP_MS(50);  // 50ms = 20Hz polling rate (balance responsiveness vs CPU)
    }
    
    http_server_shutdown();
    return 0;
}

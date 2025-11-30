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
    
#ifndef ESP_PLATFORM
    printf("HTTP Server starting on port %d...\n", HTTP_PORT);
#endif
    
    if (!http_server_init()) {
#ifndef ESP_PLATFORM
        printf("Failed to initialize server\n");
        printf("Error code: %d\n", WSAGetLastError());
#endif
        return 1;
    }
    
#ifndef ESP_PLATFORM
    printf("Server initialized successfully\n");
#endif
    
#ifndef ESP_PLATFORM
    // Display local network info (VS Code testing only)
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        struct hostent* host = gethostbyname(hostname);
        const char* ip = "127.0.0.1";
        if (host && host->h_addr_list[0]) {
            struct in_addr addr;
            memcpy(&addr, host->h_addr_list[0], sizeof(addr));
            ip = inet_ntoa(addr);
        }
        printf("Access at: http://localhost:%d or http://%s:%d\n", HTTP_PORT, ip, HTTP_PORT);
    } else {
        printf("Access at: http://localhost:%d\n", HTTP_PORT);
    }
    printf("Press Ctrl+C to stop\n\n");
#endif
    
    // Main loop
    int count = 0;
    while (1) {
        update_sensors();
        http_server_poll();
#ifndef ESP_PLATFORM
        if (++count % 100 == 0) {
            printf("Loop iteration %d\n", count);
        }
#endif
        SLEEP_MS(50);  // 50ms = 20Hz polling rate (balance responsiveness vs CPU)
    }
    
    http_server_shutdown();
    return 0;
}

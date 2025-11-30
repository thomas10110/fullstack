#include "http_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
    #include <windows.h>
    #define SLEEP_MS(ms) Sleep(ms)
#else
    #include <unistd.h>
    #define SLEEP_MS(ms) usleep((ms)*1000)
#endif

// Simulate sensor readings
static float simulate_temperature() {
    static float temp = 22.0f;
    temp += (rand() % 20 - 10) / 10.0f; // Random walk
    if (temp < 15.0f) temp = 15.0f;
    if (temp > 35.0f) temp = 35.0f;
    return temp;
}

static float simulate_water() {
    static float water = 50.0f;
    water += (rand() % 10 - 5) / 10.0f;
    if (water < 0.0f) water = 0.0f;
    if (water > 100.0f) water = 100.0f;
    return water;
}

int main() {
    // Initialize random seed
    srand((unsigned int)time(NULL));
    
    printf("Starting HTTP Server on port %d...\n", HTTP_PORT);
    
    if (!http_server_init()) {
        printf("Failed to initialize HTTP server\n");
        return 1;
    }
    
    // Get local IP address
    char hostname[256];
    gethostname(hostname, sizeof(hostname));
    
    struct hostent* host = gethostbyname(hostname);
    const char* local_ip = "127.0.0.1";
    if (host && host->h_addr_list[0]) {
        struct in_addr addr;
        memcpy(&addr, host->h_addr_list[0], sizeof(struct in_addr));
        local_ip = inet_ntoa(addr);
    }
    
    printf("\n===========================================\n");
    printf("Server running and accessible at:\n");
    printf("  Local:   http://localhost:%d\n", HTTP_PORT);
    printf("  Network: http://%s:%d\n", local_ip, HTTP_PORT);
    printf("===========================================\n");
    printf("Press Ctrl+C to stop\n\n");
    
    // Main loop: Poll for HTTP requests and update sensor data
    while (1) {
        // Update sensor values
        http_setTemperature(simulate_temperature());
        http_setWater(simulate_water());
        
        // Process HTTP requests (non-blocking)
        http_server_poll();
        
        // Small delay to prevent CPU spinning
        SLEEP_MS(50);
    }
    
    http_server_shutdown();
    return 0;
}

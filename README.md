# Minimal HTTP Server Library - AI Extension Guide

## Overview
Ultra-lightweight, single-header HTTP server library (`http_server.h`) designed for ESP32 and desktop platforms. Zero dynamic allocation, <10KB RAM footprint, handles 2-3 concurrent clients.

## Architecture

### Memory Model
- **Static allocation only**: All buffers pre-allocated globally
- `g_request_buffer[2048]`: Incoming HTTP requests
- `g_response_buffer[2048]`: Outgoing HTTP responses
- `g_sensor_temp`, `g_sensor_water`: Sensor state (floats)
- No `malloc`/`new`, no STL containers

### Platform Abstraction
```cpp
#ifdef ESP_PLATFORM
    // lwIP sockets for ESP32
#elif _WIN32
    // Winsock2 for Windows
#else
    // POSIX sockets for Linux
#endif
```

### Core Functions
1. **`http_server_init()`**: Creates socket, binds to port 80, sets non-blocking mode
2. **`http_server_poll()`**: Non-blocking accept() + request handling (call in loop)
3. **`http_handle_client(socket)`**: Recv → Parse → Route → Send → Close
4. **`http_server_shutdown()`**: Cleanup

### Request Flow
```
Client → recv() → http_parse_request() → http_route() → send() → close()
         ↓                ↓                    ↓
    g_request_buffer   method+path      g_response_buffer
```

## API Reference

### Initialization
```cpp
int http_server_init();  // Returns 1 on success, 0 on failure
```

### Runtime (Call in Main Loop)
```cpp
void http_server_poll();  // Checks for new connections, handles 1 request per call
```

### Data Injection
```cpp
void http_setTemperature(float value);
void http_setWater(float value);
```

### Cleanup
```cpp
void http_server_shutdown();
```

## Routes (Current Implementation)

| Method | Path    | Response                          |
|--------|---------|-----------------------------------|
| GET    | `/`     | HTML dashboard (embedded)         |
| GET    | `/data` | JSON: `{"temperature":X,"water":Y}` |
| *      | *       | 404 Not Found                     |

## Extending the Library

### Adding New Endpoints

**Location**: Modify `http_route()` function in `http_server.h`

**Example**: Add `/status` endpoint
```cpp
// Inside http_route(), after existing if-else blocks:
else if (strcmp(path, "/status") == 0) {
    const char* status_json = "{\"status\":\"ok\",\"uptime\":12345}";
    int len = sprintf(response,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n\r\n%s",
        (int)strlen(status_json), status_json);
    return len;
}
```

### Adding New Sensor Values

**Steps**:
1. Add global variable: `static float g_sensor_humidity = 0.0f;`
2. Add setter: `inline void http_setHumidity(float value) { g_sensor_humidity = value; }`
3. Update `/data` JSON in `http_route()`:
   ```cpp
   sprintf(json, "{\"temperature\":%.2f,\"water\":%.2f,\"humidity\":%.2f}", 
           g_sensor_temp, g_sensor_water, g_sensor_humidity);
   ```
4. Update HTML/JS to fetch and display new value

### Implementing POST Requests

**Location**: Inside `http_route()`, add POST handler before 404

**Example**: Control LED
```cpp
else if (strcmp(method, "POST") == 0 && strcmp(path, "/led") == 0) {
    // Parse body (assume simple format like "state=on")
    const char* body = strstr(g_request_buffer, "\r\n\r\n");
    if (body) {
        body += 4; // Skip \r\n\r\n
        if (strstr(body, "state=on")) {
            // Turn LED on (set global flag)
            g_led_state = 1;
        }
    }
    const char* ok = "OK";
    int len = sprintf(response,
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n\r\n%s",
        (int)strlen(ok), ok);
    return len;
}
```

### Modifying HTML Dashboard

**Location**: `g_html_page` string in `http_server.h`

**Guidelines**:
- Keep minified (remove whitespace, newlines)
- Embed CSS in `<style>` tags
- Use inline JavaScript
- Update `/data` fetch URL if changing endpoint
- Total size budget: <2KB (leaves buffer space)

**Example**: Add humidity display
```cpp
// In .row div, add:
"<div class='card'><h3>Humidity</h3><p><span id='humidity-value'>0</span> %</p></div>"

// In updateData() JS function:
"document.getElementById('humidity-value').textContent=d.humidity"
```

## Performance Tuning

### Buffer Sizes
```cpp
#define HTTP_BUFFER_SIZE 2048  // Increase if responses exceed 2KB
#define HTTP_MAX_PATH 64       // Max URL path length
```

### Concurrent Clients
```cpp
#define HTTP_BACKLOG 3  // OS queue for pending connections
```
**Note**: Single-threaded design processes 1 request per `poll()` call. For true concurrency, add `select()`/`poll()` multiplexing.

### Polling Frequency
In `main.cpp`:
```cpp
SLEEP_MS(50);  // Lower = more responsive, higher CPU usage
```

## ESP32-Specific Integration

### Example (ESP-IDF)
```cpp
#include "http_server.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void http_task(void* pvParameters) {
    http_server_init();
    while(1) {
        http_server_poll();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void app_main() {
    // WiFi setup here (AP or STA mode)
    xTaskCreate(&http_task, "http_server", 4096, NULL, 5, NULL);
}
```

### WiFi AP Mode
```cpp
#include "esp_wifi.h"

wifi_config_t wifi_config = {
    .ap = {
        .ssid = "ESP32-GrowLight",
        .password = "12345678",
        .max_connection = 3,
    },
};
esp_wifi_set_mode(WIFI_MODE_AP);
esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config);
esp_wifi_start();
```
Access dashboard at `http://192.168.4.1`

## Debugging

### Common Issues

**Port 80 already in use (Windows)**:
```powershell
# Check what's using port 80
netstat -ano | findstr :80
# Kill process (replace PID)
taskkill /PID <PID> /F
# Or change port in http_server.h
#define HTTP_PORT 8080
```

**Linux permission denied (port <1024)**:
```bash
# Run with sudo OR use port >1024
sudo ./main
# Or change port
#define HTTP_PORT 8080
```

**ESP32 not responding**:
- Check WiFi connection (`esp_wifi_get_mode()`)
- Verify IP address (`esp_netif_get_ip_info()`)
- Increase task stack size if crashing (`xTaskCreate(..., 8192, ...)`)

### Logging
Add debug prints in `http_handle_client()`:
```cpp
printf("Request: %.*s\n", 100, g_request_buffer);
printf("Method: %s, Path: %s\n", method, path);
```

## Compilation

### Windows (MinGW/MSYS2)
```powershell
g++ -o server.exe main.cpp -lws2_32
.\server.exe
```

### Linux
```bash
g++ -o server main.cpp
./server
```

### ESP32 (ESP-IDF)
```bash
idf.py build
idf.py flash monitor
```

## Memory Analysis

**Static allocation**:
- Request buffer: 2048 bytes
- Response buffer: 2048 bytes
- HTML page: ~1.5KB
- Sensor variables: 8 bytes
- **Total**: ~5.6KB

**Stack usage** (per call):
- `http_handle_client()`: ~200 bytes (method+path buffers)
- `http_route()`: ~150 bytes (JSON buffer)

## Security Considerations

**CRITICAL**: This library is for **local-only, non-production use**. It has:
- No input validation (vulnerable to buffer overflows if malformed requests exceed buffer)
- No authentication
- No HTTPS/encryption
- No DoS protection

**Safe use**: Isolated networks, development, demos, local IoT.

## AI Agent Instructions

When extending this library:
1. **Preserve static allocation**: Never introduce `malloc`, `new`, `std::string`, `std::vector`
2. **Check buffer sizes**: Ensure response fits in `HTTP_BUFFER_SIZE`
3. **Test cross-platform**: Use `#ifdef ESP_PLATFORM` for ESP32-specific code
4. **Maintain minimalism**: Add features only if <50 lines of code
5. **Update this README**: Document new endpoints, globals, or APIs

**Code style**:
- Free functions only (no classes)
- C-style strings (`strcpy`, `sprintf`, `strcmp`)
- Inline simple setters/getters
- Comment complex logic only

## Future Enhancements (Optional)

- WebSocket support for real-time bidirectional communication
- POST body parsing (form data, JSON)
- Static file serving from filesystem
- Basic authentication (hardcoded credentials)
- CORS headers for cross-origin requests
- Gzip compression for HTML responses

Each enhancement should be toggleable via `#define` to maintain minimalism when not needed.

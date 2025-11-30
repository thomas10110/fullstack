#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

// Platform detection and includes
#ifdef ESP_PLATFORM
    #include <lwip/sockets.h>
    #include <lwip/netdb.h>
    #define SOCKET_T int
    #define SOCKET_CLOSE(s) close(s)
    #define SOCKET_ERROR_VAL -1
    #define LIKELY(x) __builtin_expect(!!(x), 1)
    #define UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
    #ifdef _WIN32
        #include <winsock2.h>
        #include <ws2tcpip.h>
        #pragma comment(lib, "ws2_32.lib")
        #define SOCKET_T SOCKET
        #define SOCKET_CLOSE(s) closesocket(s)
        #define SOCKET_ERROR_VAL INVALID_SOCKET
        #define LIKELY(x) (x)
        #define UNLIKELY(x) (x)
    #else
        #include <sys/socket.h>
        #include <netinet/in.h>
        #include <unistd.h>
        #include <fcntl.h>
        #include <errno.h>
        #define SOCKET_T int
        #define SOCKET_CLOSE(s) close(s)
        #define SOCKET_ERROR_VAL -1
        #define LIKELY(x) __builtin_expect(!!(x), 1)
        #define UNLIKELY(x) __builtin_expect(!!(x), 0)
    #endif
#endif

#include <string.h>

#ifndef ESP_PLATFORM
    #include <stdio.h>
#endif

// Configuration - Aggressively minimized for ESP32
#ifndef HTTP_PORT
#define HTTP_PORT 8080  // Use 8080 for VS Code testing (80 for ESP32)
#endif

#ifndef HTTP_BACKLOG
#define HTTP_BACKLOG 2  // ESP32: minimal queue
#endif

// Ultra-minimal buffer sizes
#ifndef HTTP_REQUEST_BUFFER
#define HTTP_REQUEST_BUFFER 512  // Browsers send many headers (User-Agent, Accept, etc.)
#endif

#ifndef HTTP_RESPONSE_BUFFER
#define HTTP_RESPONSE_BUFFER 1400  // HTML: ~970B + headers: ~150B + margin
#endif

#ifndef HTTP_MAX_ACCEPT_PER_POLL
#define HTTP_MAX_ACCEPT_PER_POLL 1  // Process one at a time to minimize stack
#endif

#define HTTP_JSON_BODY_MAX 44  // "{\"temperature\":99.99,\"water\":99.99}"

// Global state (static allocation) - Minimal footprint
static SOCKET_T g_server_socket = SOCKET_ERROR_VAL;
static volatile float g_sensor_temp = 0.0f;
static volatile float g_sensor_water = 0.0f;
static char g_request_buffer[HTTP_REQUEST_BUFFER];
static char g_response_buffer[HTTP_RESPONSE_BUFFER];

// Atomic access for sensor values
#ifdef ESP_PLATFORM
    #include "freertos/portmacro.h"
    #define ATOMIC_READ_FLOAT(var, dest) do { \
        portDISABLE_INTERRUPTS(); \
        dest = var; \
        portENABLE_INTERRUPTS(); \
    } while(0)
    #define ATOMIC_WRITE_FLOAT(var, value) do { \
        portDISABLE_INTERRUPTS(); \
        var = value; \
        portENABLE_INTERRUPTS(); \
    } while(0)
#else
    #define ATOMIC_READ_FLOAT(var, dest) dest = var
    #define ATOMIC_WRITE_FLOAT(var, value) var = value
#endif

// Pre-calculated constant headers
static const char g_http_200_html[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: ";
static const char g_http_200_json[] = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: ";
static const char g_http_close[] = "\r\nConnection: close\r\n\r\n";
static const char g_http_404[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 9\r\nConnection: close\r\n\r\nNot Found";

// Ultra-minified HTML - Aggressively compressed
static const char g_html_page[] = 
"<!DOCTYPE html><html><head><meta charset=\"utf-8\"><title>ESP32</title><style>"
"body{background:#000;color:#fff;margin:0;font:14px sans-serif}"
".s{position:fixed;left:0;top:0;width:120px;height:100%;background:#333;padding-top:15px}"
".s a{padding:8px 10px;text-decoration:none;color:#fff;display:block;font-size:18px}"
".c{margin-left:140px;padding:15px}.h{display:none}"
".r{display:flex;gap:20px;flex-wrap:wrap}"
".d{background:#222;padding:15px;border-radius:8px;min-width:120px;text-align:center;border:2px solid #fff}"
"</style></head><body>"
"<div class=s>"
"<a href=# onclick=\"f('a');return false\">Dashboard</a>"
"<a href=# onclick=\"f('b');return false\">Analytics</a>"
"<a href=# onclick=\"f('c');return false\">Settings</a>"
"</div>"
"<div id=a class=c>"
"<h1>Overview</h1>"
"<div class=r>"
"<div class=d><h3>Temp</h3><p><span id=t>0</span>&deg;C</p></div>"
"<div class=d><h3>Water</h3><p><span id=w>0</span>%</p></div>"
"</div></div>"
"<div id=b class='c h'><h1>Analytics</h1></div>"
"<div id=c class='c h'><h1>Settings</h1></div>"
"<script>"
"function f(i){a.className=b.className=c.className='c h';document.getElementById(i).className='c'}"
"function u(){fetch('/data').then(r=>r.json()).then(d=>{t.textContent=d.temperature;w.textContent=d.water})}"
"onload=()=>{f('a');u();setInterval(u,1e3)}"
"</script></body></html>";

#define HTML_LEN (sizeof(g_html_page) - 1)

// Public API: Set sensor values (thread-safe)
inline void http_setTemperature(float value) { ATOMIC_WRITE_FLOAT(g_sensor_temp, value); }
inline void http_setWater(float value) { ATOMIC_WRITE_FLOAT(g_sensor_water, value); }

// Fast integer to string conversion (no division)
static inline int http_itoa(char* dst, unsigned val) {
    char tmp[10];
    int i = 0;
    do { tmp[i++] = '0' + (val % 10); val /= 10; } while (val);
    int len = i;
    while (i--) *dst++ = tmp[i];
    return len;
}

// Fast float to string (2 decimal places, optimized for ESP32)
static inline int http_ftoa(char* dst, float val) {
    char* p = dst;
    if (val < 0.0f) { *p++ = '-'; val = -val; }
    unsigned long fixed = (unsigned long)(val * 100.0f + 0.5f);
    unsigned whole = fixed / 100u, frac = fixed % 100u;
    p += http_itoa(p, whole);
    *p++ = '.';
    *p++ = '0' + (frac / 10u);
    *p++ = '0' + (frac % 10u);
    return (int)(p - dst);
}

// Build JSON response: {"temperature":XX.XX,"water":XX.XX}
static inline int http_build_json(char* buf, float temp, float water) {
    char* p = buf;
    // Manual string construction (faster than sprintf on ESP32)
    *p++ = '{'; *p++ = '"'; *p++ = 't'; *p++ = 'e'; *p++ = 'm'; *p++ = 'p';
    *p++ = 'e'; *p++ = 'r'; *p++ = 'a'; *p++ = 't'; *p++ = 'u'; *p++ = 'r';
    *p++ = 'e'; *p++ = '"'; *p++ = ':';
    p += http_ftoa(p, temp);
    *p++ = ','; *p++ = '"'; *p++ = 'w'; *p++ = 'a'; *p++ = 't'; *p++ = 'e';
    *p++ = 'r'; *p++ = '"'; *p++ = ':';
    p += http_ftoa(p, water);
    *p++ = '}';
    return (int)(p - buf);
}

// Zero-copy request parser (modifies buffer in-place)
static inline int http_parse(char* req, int len, char** method, char** path) {
    char *p = req, *end = req + len;
    *method = p;
    while (p < end && *p > ' ') p++;
    if (UNLIKELY(p >= end)) return 0;
    *p++ = '\0';
    while (p < end && *p == ' ') p++;
    *path = p;
    while (p < end && *p > ' ') p++;
    if (UNLIKELY(p >= end)) return 0;
    *p = '\0';
    return 1;
}

// Route handler (hot path - optimized branching)
#ifdef __GNUC__
__attribute__((hot))
#endif
static int http_route(const char* method, const char* path, char* resp) {
    // Fast GET check
    if (LIKELY(method[0] == 'G' && method[1] == 'E' && method[2] == 'T' && !method[3])) {
        // Root: /
        if (LIKELY(path[0] == '/' && !path[1])) {
            char* p = resp;
            memcpy(p, g_http_200_html, sizeof(g_http_200_html) - 1);
            p += sizeof(g_http_200_html) - 1;
            p += http_itoa(p, HTML_LEN);
            memcpy(p, g_http_close, sizeof(g_http_close) - 1);
            p += sizeof(g_http_close) - 1;
            memcpy(p, g_html_page, HTML_LEN);
            return (int)(p - resp) + HTML_LEN;
        }
        // Data: /data
        if (path[1] == 'd' && path[2] == 'a' && path[3] == 't' && path[4] == 'a' && !path[5]) {
            float temp, water;
            ATOMIC_READ_FLOAT(g_sensor_temp, temp);
            ATOMIC_READ_FLOAT(g_sensor_water, water);
            
            char* p = resp;
            memcpy(p, g_http_200_json, sizeof(g_http_200_json) - 1);
            p += sizeof(g_http_200_json) - 1;
            char* len_pos = p;
            p += 2; // Reserve for content-length
            memcpy(p, g_http_close, sizeof(g_http_close) - 1);
            p += sizeof(g_http_close) - 1;
            char* json_start = p;
            p += http_build_json(p, temp, water);
            int json_len = (int)(p - json_start);
            
            // Backfill length
            len_pos[0] = '0' + (json_len / 10);
            len_pos[1] = '0' + (json_len % 10);
            return (int)(p - resp);
        }
    }
    
    // 404
    memcpy(resp, g_http_404, sizeof(g_http_404) - 1);
    return sizeof(g_http_404) - 1;
}

// Send all data with minimal overhead
static inline int http_send_all(SOCKET_T sock, const char* data, int len) {
    int sent = 0;
    while (sent < len) {
        int n = send(sock, data + sent, len - sent, 0);
        if (UNLIKELY(n <= 0)) {
#ifdef _WIN32
            int err = WSAGetLastError();
            if (err == WSAEWOULDBLOCK || err == WSAEINTR) continue;
#else
            if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) continue;
#endif
            return 0;
        }
        sent += n;
    }
    return 1;
}

// Handle single client connection (minimized stack usage)
static void http_handle_client(SOCKET_T sock) {
    // Set timeout
#ifdef _WIN32
    DWORD timeout = 2000;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
#else
    struct timeval tv = {2, 0};
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
#endif

#ifdef ESP_PLATFORM
    // Immediate close on ESP32
    struct linger sl = {1, 0};
    setsockopt(sock, SOL_SOCKET, SO_LINGER, &sl, sizeof(sl));
#endif

    int n = recv(sock, g_request_buffer, HTTP_REQUEST_BUFFER - 1, 0);
    if (UNLIKELY(n <= 0)) { SOCKET_CLOSE(sock); return; }
    
    g_request_buffer[n] = '\0';
    char *method, *path;
    if (UNLIKELY(!http_parse(g_request_buffer, n, &method, &path))) {
        SOCKET_CLOSE(sock);
        return;
    }
    
    int resp_len = http_route(method, path, g_response_buffer);
    http_send_all(sock, g_response_buffer, resp_len);
    SOCKET_CLOSE(sock);
}

// Initialize server
static int http_server_init() {
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa)) return 0;
#endif
    
    g_server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (UNLIKELY(g_server_socket == SOCKET_ERROR_VAL)) return 0;
    
    int opt = 1;
    setsockopt(g_server_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(HTTP_PORT);
    
    if (UNLIKELY(bind(g_server_socket, (struct sockaddr*)&addr, sizeof(addr)) < 0)) {
        SOCKET_CLOSE(g_server_socket);
        return 0;
    }
    
    if (UNLIKELY(listen(g_server_socket, HTTP_BACKLOG) < 0)) {
        SOCKET_CLOSE(g_server_socket);
        return 0;
    }
    
    // Non-blocking mode
#ifdef _WIN32
    u_long mode = 1;
    ioctlsocket(g_server_socket, FIONBIO, &mode);
#else
    int flags = fcntl(g_server_socket, F_GETFL, 0);
    fcntl(g_server_socket, F_SETFL, flags | O_NONBLOCK);
#endif

    return 1;
}

// Poll for incoming connections (call from main loop)
static void http_server_poll() {
    if (UNLIKELY(g_server_socket == SOCKET_ERROR_VAL)) return;
    
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    
    for (int i = 0; i < HTTP_MAX_ACCEPT_PER_POLL; i++) {
        SOCKET_T sock = accept(g_server_socket, (struct sockaddr*)&addr, &len);
        if (UNLIKELY(sock == SOCKET_ERROR_VAL)) break;
        
        // Set blocking mode for client socket
#ifdef _WIN32
        u_long mode = 0;
        ioctlsocket(sock, FIONBIO, &mode);
#else
        int flags = fcntl(sock, F_GETFL, 0);
        fcntl(sock, F_SETFL, flags & ~O_NONBLOCK);
#endif
        
#ifdef TCP_NODELAY
        int nodelay = 1;
        setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&nodelay, sizeof(nodelay));
#endif
        
        http_handle_client(sock);
    }
}

// Shutdown server
static void http_server_shutdown() {
    if (g_server_socket != SOCKET_ERROR_VAL) {
        SOCKET_CLOSE(g_server_socket);
        g_server_socket = SOCKET_ERROR_VAL;
    }
#ifdef _WIN32
    WSACleanup();
#endif
}

#endif // HTTP_SERVER_H

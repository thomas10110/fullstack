#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

// Platform detection and includes
#ifdef ESP_PLATFORM
    #include <lwip/sockets.h>
    #include <lwip/netdb.h>
    #define SOCKET_T int
    #define SOCKET_CLOSE(s) close(s)
    #define SOCKET_ERROR_VAL -1
#else
    #ifdef _WIN32
        #include <winsock2.h>
        #include <ws2tcpip.h>
        #pragma comment(lib, "ws2_32.lib")
        #define SOCKET_T SOCKET
        #define SOCKET_CLOSE(s) closesocket(s)
        #define SOCKET_ERROR_VAL INVALID_SOCKET
    #else
        #include <sys/socket.h>
        #include <netinet/in.h>
        #include <unistd.h>
        #include <fcntl.h>
        #define SOCKET_T int
        #define SOCKET_CLOSE(s) close(s)
        #define SOCKET_ERROR_VAL -1
    #endif
#endif

#include <string.h>
#include <stdio.h>

// Configuration
#define HTTP_PORT 1111
#define HTTP_BACKLOG 3
#define HTTP_BUFFER_SIZE 2048

// Global state (static allocation)
static SOCKET_T g_server_socket = SOCKET_ERROR_VAL;
static float g_sensor_temp = 0.0f;
static float g_sensor_water = 0.0f;
static char g_request_buffer[HTTP_BUFFER_SIZE];
static char g_response_buffer[HTTP_BUFFER_SIZE];

// Optimization: Pre-calculated header for the index page
static char g_index_header[128];
static int g_index_header_len = 0;

// Embedded HTML (minified from existing index.html)
static const char g_html_page[] = 
"<!DOCTYPE html><html><head><title>GrowLightUserInterface</title><style>"
"body{background:#000;color:#fff;margin:0;font-family:sans-serif}"
".sidebar{position:fixed;left:0;top:0;width:150px;height:100%;background:#333;padding-top:20px}"
".sidebar a{padding:10px 15px;text-decoration:none;color:#fff;display:block}"
".content{margin-left:220px;padding:20px}.hidden{display:none}"
".row{display:flex;gap:30px;flex-wrap:wrap}"
".card{background:#222;padding:20px;border-radius:10px;min-width:150px;text-align:center;border:2px solid #fff}"
"</style></head><body>"
"<div class='sidebar'>"
"<a href='#' onclick=\"showSection('dashboard');return false\" style='font-size:25px'>Dashboard</a>"
"<a href='#' onclick=\"showSection('analytics');return false\" style='font-size:25px'>Analytics</a>"
"<a href='#' onclick=\"showSection('settings');return false\" style='font-size:25px'>Settings</a>"
"</div>"
"<div id='dashboard' class='content'>"
"<h1 style='font-size:50px'>Overview</h1>"
"<div class='row'>"
"<div class='card'><h3>Temperature</h3><p><span id='temperature-value'>0</span> °C</p></div>"
"<div class='card'><h3>Water</h3><p><span id='water-value'>0</span> %</p></div>"
"</div></div>"
"<div id='analytics' class='content hidden'><h1>Analytics Page</h1><p>Analytics content goes here.</p></div>"
"<div id='settings' class='content hidden'><h1>Settings Page</h1><p>Settings content goes here.</p></div>"
"<script>"
"function showSection(s){document.getElementById('dashboard').classList.add('hidden');"
"document.getElementById('analytics').classList.add('hidden');"
"document.getElementById('settings').classList.add('hidden');"
"document.getElementById(s).classList.remove('hidden')}"

"function updateData(){fetch('/data').then(r=>r.json()).then(d=>{"
"document.getElementById('temperature-value').textContent=d.temperature;"
"document.getElementById('water-value').textContent=d.water})}"
"window.onload=function(){showSection('dashboard');updateData();setInterval(updateData,1000)}"
"</script></body></html>";

#define HTML_PAGE_LEN (sizeof(g_html_page) - 1)

// Public API: Set sensor values
inline void http_setTemperature(float value) { g_sensor_temp = value; }
inline void http_setWater(float value) { g_sensor_water = value; }

// Internal: Zero-Copy Parse HTTP request
// Modifies req buffer in-place to terminate strings
static int http_parse_request(char* req, char** method, char** path) {
    *method = req;
    
    // Find end of method
    char* space1 = strchr(req, ' ');
    if (!space1) return 0;
    *space1 = '\0'; // Null-terminate method
    
    // Start of path
    *path = space1 + 1;
    
    // Find end of path
    char* space2 = strchr(*path, ' ');
    if (!space2) return 0;
    *space2 = '\0'; // Null-terminate path
    
    return 1;
}

// Internal: Route and generate response
static int http_route(const char* method, const char* path, char* response) {
    // Fast check for GET
    if (method[0] == 'G' && method[1] == 'E' && method[2] == 'T' && method[3] == '\0') {
        if (path[0] == '/' && path[1] == '\0') {
            // Root path: Use pre-calculated header + static HTML
            memcpy(response, g_index_header, g_index_header_len);
            memcpy(response + g_index_header_len, g_html_page, HTML_PAGE_LEN);
            return g_index_header_len + HTML_PAGE_LEN;
        }
        else if (strcmp(path, "/data") == 0) {
            // Generate JSON with current sensor values
            char json_body[128];
            int json_len = sprintf(json_body, "{\"temperature\":%.2f,\"water\":%.2f}", 
                                   g_sensor_temp, g_sensor_water);
            
            int len = sprintf(response,
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: application/json\r\n"
                "Content-Length: %d\r\n"
                "Connection: close\r\n\r\n%s",
                json_len, json_body);
            return len;
        }
    }
    
    // 404 for all other requests
    const char* msg = "Not Found";
    int len = sprintf(response,
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n\r\n%s",
        (int)strlen(msg), msg);
    return len;
}

// Internal: Handle client connection
static void http_handle_client(SOCKET_T client_socket) {
    int recv_len = recv(client_socket, g_request_buffer, HTTP_BUFFER_SIZE - 1, 0);
    if (recv_len <= 0) {
        SOCKET_CLOSE(client_socket);
        return;
    }
    
    g_request_buffer[recv_len] = '\0';
    
    char *method, *path;
    // Pass addresses of pointers for zero-copy extraction
    if (!http_parse_request(g_request_buffer, &method, &path)) {
        SOCKET_CLOSE(client_socket);
        return;
    }
    
    int response_len = http_route(method, path, g_response_buffer);
    
    const char* send_ptr = g_response_buffer;
    int remaining = response_len;
    while (remaining > 0) {
        int sent = send(client_socket, send_ptr, remaining, 0);
        if (sent <= 0) break;
        send_ptr += sent;
        remaining -= sent;
    }
    
    SOCKET_CLOSE(client_socket);
}

// Public API: Initialize server
static int http_server_init() {
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 0;
#endif
    
    g_server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (g_server_socket == SOCKET_ERROR_VAL) return 0;
    
    // Allow port reuse
    int opt = 1;
    setsockopt(g_server_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
    
#ifdef TCP_NODELAY
    setsockopt(g_server_socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(opt));
#endif
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(HTTP_PORT);
    
    if (bind(g_server_socket, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        SOCKET_CLOSE(g_server_socket);
        return 0;
    }
    
    if (listen(g_server_socket, HTTP_BACKLOG) < 0) {
        SOCKET_CLOSE(g_server_socket);
        return 0;
    }
    
    // Set non-blocking mode
#ifdef _WIN32
    u_long mode = 1;
    ioctlsocket(g_server_socket, FIONBIO, &mode);
#else
    int flags = fcntl(g_server_socket, F_GETFL, 0);
    fcntl(g_server_socket, F_SETFL, flags | O_NONBLOCK);
#endif

    // Optimization: Pre-calculate the static index header once
    g_index_header_len = sprintf(g_index_header,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html; charset=utf-8\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n\r\n",
        (int)HTML_PAGE_LEN);
    
    return 1;
}

// Public API: Process pending requests (call in main loop)
static void http_server_poll() {
    if (g_server_socket == SOCKET_ERROR_VAL) return;
    
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    SOCKET_T client_socket;
    
    // Loop to accept all pending connections
    while ((client_socket = accept(g_server_socket, (struct sockaddr*)&client_addr, &addr_len)) != SOCKET_ERROR_VAL) {
        http_handle_client(client_socket);
    }
}

// Public API: Shutdown server
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

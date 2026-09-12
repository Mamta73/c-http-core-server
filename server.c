#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <process.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 4096

void serve_html(SOCKET client_fd, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        char *not_found = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n404 File Not Found";
        send(client_fd, not_found, strlen(not_found), 0);
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *file_content = malloc(file_size + 1);
    fread(file_content, 1, file_size, file);
    fclose(file);
    file_content[file_size] = '\0';

    char header[512];
    snprintf(header, sizeof(header),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/html\r\n"
             "Content-Length: %ld\r\n"
             "Connection: close\r\n\r\n", file_size);

    send(client_fd, header, strlen(header), 0);
    send(client_fd, file_content, file_size, 0);

    free(file_content);
}

void handle_client(void *client_socket_ptr) {
    SOCKET client_fd = *(SOCKET *)client_socket_ptr;
    free(client_socket_ptr);

    char buffer[BUFFER_SIZE] = {0};
    recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    
    printf("\n[LOG] Received Request:\n%.100s...\n", buffer);

    serve_html(client_fd, "index.html");

    closesocket(client_fd);
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    SOCKET server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    int addr_len = sizeof(client_addr);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        WSACleanup();
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed\n");
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    if (listen(server_fd, 10) == SOCKET_ERROR) {
        printf("Listen failed\n");
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    printf("===========================================\n");
    printf(" Server running on http://localhost:%d\n", PORT);
    printf("===========================================\n");

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
        if (client_fd == INVALID_SOCKET) {
            continue;
        }

        SOCKET *new_sock = malloc(sizeof(SOCKET));
        *new_sock = client_fd;

        _beginthread(handle_client, 0, (void *)new_sock);
    }

    closesocket(server_fd);
    WSACleanup();
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "proxy.h"

#define BUFFER_SIZE 4096

void forward_data(int source_sock, int dest_sock) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = recv(source_sock, buffer, BUFFER_SIZE, 0)) > 0) {
        send(dest_sock, buffer, bytes_read, 0);
    }
}

void handle_client(int client_sock, const Config *config) {
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Failed to create server socket");
        close(client_sock);
        return;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(config->target_port);
    inet_pton(AF_INET, config->target_address, &server_addr.sin_addr);

    if (connect(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to connect to target server");
        close(client_sock);
        close(server_sock);
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        forward_data(client_sock, server_sock);
        exit(0);
    } else {
        forward_data(server_sock, client_sock);
    }

    close(client_sock);
    close(server_sock);
}

void run_proxy(const Config *config) {
    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock < 0) {
        perror("Failed to create listen socket");
        exit(1);
    }

    struct sockaddr_in listen_addr;
    memset(&listen_addr, 0, sizeof(listen_addr));
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_port = htons(config->listen_port);
    inet_pton(AF_INET, config->listen_address, &listen_addr.sin_addr);

    if (bind(listen_sock, (struct sockaddr *)&listen_addr, sizeof(listen_addr)) < 0) {
        perror("Failed to bind listen socket");
        exit(1);
    }

    if (listen(listen_sock, 10) < 0) {
        perror("Failed to listen");
        exit(1);
    }

    printf("Reverse proxy listening on %s:%d\n", config->listen_address, config->listen_port);
    printf("Forwarding to %s:%d\n", config->target_address, config->target_port);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_sock = accept(listen_sock, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_sock < 0) {
            perror("Failed to accept client connection");
            continue;
        }

        pid_t pid = fork();
        if (pid == 0) {
            close(listen_sock);
            handle_client(client_sock, config);
            exit(0);
        } else {
            close(client_sock);
        }
    }
}

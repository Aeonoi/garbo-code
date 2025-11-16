#include "Network.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>

Network::Network() : sock(-1) {}

Network::~Network() {
    if (sock != -1) close(sock);
    for (int client_sock : client_socks) {
        if (client_sock != -1) close(client_sock);
    }
}

bool Network::host(int port) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        return false;
    }

    int reuse = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;
    }

    // Set to non-blocking
    fcntl(sock, F_SETFL, O_NONBLOCK);

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        return false;
    }

    listen(sock, 5); // Listen for up to 5 connections
    return true;
}

void Network::accept_new_client() {
    sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);
    int new_client_sock = accept(sock, (struct sockaddr*)&client_addr, &client_len);
    if (new_client_sock >= 0) {
        client_socks.push_back(new_client_sock);
    }
}

bool Network::join(const std::string& ip, int port) {
    if (sock != -1) {
        return true;
    }
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        return false;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0) {
        return false;
    }

    return connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) >= 0;
}

void Network::send_start() {
    broadcast_data("start", 5);
}

bool Network::wait_for_start() {
    char buffer[1024] = {0};
    if (sock != -1) {
        read(sock, buffer, 1024);
        return std::string(buffer) == "start";
    }
    return false;
}

bool Network::send_data(const char* data, size_t len) {
    if (!client_socks.empty()) {
        int target_sock = client_socks[0]; // For now, send to the first client
        ssize_t sent = send(target_sock, data, len, MSG_NOSIGNAL);
        return sent == static_cast<ssize_t>(len);
    }
    return false;
}

bool Network::broadcast_data(const char* data, size_t len) {
    bool all_sent = true;
    for (int client_sock : client_socks) {
        ssize_t sent = send(client_sock, data, len, MSG_NOSIGNAL);
        if (sent != static_cast<ssize_t>(len)) {
            all_sent = false;
        }
    }
    return all_sent;
}

bool Network::recv_data(char* buffer, size_t len) {
    int target_sock = sock; // On client, sock is the connection to the server
    if (target_sock != -1) {
        ssize_t received = recv(target_sock, buffer, len, MSG_DONTWAIT);
        return received > 0;
    }
    return false;
}

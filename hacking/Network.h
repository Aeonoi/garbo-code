#pragma once

#include <string>
#include <vector>

class Network {
public:
    Network();
    ~Network();

    bool host(int port);
    bool join(const std::string& ip, int port);

    void send_start();
    bool wait_for_start();
    
    bool send_data(const char* data, size_t len);
    bool broadcast_data(const char* data, size_t len);
    bool recv_data(char* buffer, size_t len);
    bool is_connected() const { return sock != -1 || !client_socks.empty(); }
    size_t getClientCount() const { return client_socks.size(); }
    void accept_new_client();

private:
    int sock;
    std::vector<int> client_socks;
};

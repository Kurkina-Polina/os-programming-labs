#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <chrono>
#include <thread>
#include <vector>
#include <atomic>

#define TCP_PORT 8081
#define UDP_PORT 8080

std::atomic<bool> servers_running(true);

void tcp_server() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("TCP socket failed");
        return;
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(TCP_PORT);

    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
        perror("TCP bind failed");
        close(server_fd);
        return;
    }

    if (listen(server_fd, 10) < 0) {
        perror("TCP listen failed");
        close(server_fd);
        return;
    }

    std::cout << "TCP server listening on port " << TCP_PORT << std::endl;

    while (servers_running) {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_fd, (sockaddr*)&client_addr, &client_len);
        
        if (client_socket < 0) {
            if (servers_running) perror("TCP accept failed");
            continue;
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        std::cout << "TCP client connected: " << client_ip << ":" << ntohs(client_addr.sin_port) << std::endl;

        char buffer[1024] = {0};
        read(client_socket, buffer, 1024);
        send(client_socket, "TCP ACK", 7, 0);
        close(client_socket);
    }

    close(server_fd);
}

void udp_server() {
    int server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd == -1) {
        perror("UDP socket failed");
        return;
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(UDP_PORT);

    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
        perror("UDP bind failed");
        close(server_fd);
        return;
    }

    std::cout << "UDP server listening on port " << UDP_PORT << std::endl;

    while (servers_running) {
        char buffer[1024] = {0};
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        ssize_t len = recvfrom(server_fd, buffer, sizeof(buffer), 0, 
                              (sockaddr*)&client_addr, &client_len);
        if (len < 0) {
            if (servers_running) perror("UDP recvfrom failed");
            continue;
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        std::cout << "Received UDP packet from " << client_ip << ":" << ntohs(client_addr.sin_port) << std::endl;

        sendto(server_fd, "UDP ACK", 7, 0, 
              (sockaddr*)&client_addr, client_len);
    }

    close(server_fd);
}

void run_client(const std::string& host, int message_count) {
    std::cout << "Testing with " << message_count << " messages" << std::endl;

    // TCP test
    auto tcp_start = std::chrono::high_resolution_clock::now();
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("TCP client socket failed");
        
    }

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(TCP_PORT);
    inet_pton(AF_INET, host.c_str(), &serv_addr.sin_addr);

    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("TCP connect failed");
        close(sock);
        
    }
    for (int i = 0; i < message_count; ++i) {
        send(sock, "TEST", 4, 0);
        char buffer[1024] = {0};
        read(sock, buffer, 1024);
        
    }
    close(sock);
    auto tcp_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> tcp_elapsed = tcp_end - tcp_start;
    std::cout << "TCP: Sent " << message_count << " messages in " << tcp_elapsed.count() << " seconds" << std::endl;

    // UDP test
    auto udp_start = std::chrono::high_resolution_clock::now();
    int sock2 = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock2 == -1) {
        perror("UDP client socket failed");
        return;
    }

    sockaddr_in serv_addr2;
    serv_addr2.sin_family = AF_INET;
    serv_addr2.sin_port = htons(UDP_PORT);
    inet_pton(AF_INET, host.c_str(), &serv_addr2.sin_addr);

    for (int i = 0; i < message_count; ++i) {
        sendto(sock2, "TEST", 4, 0, 
              (sockaddr*)&serv_addr2, sizeof(serv_addr2));
        
        char buffer[1024] = {0};
        socklen_t len = sizeof(serv_addr2);
        recvfrom(sock2, buffer, sizeof(buffer), 0, 
                (sockaddr*)&serv_addr2, &len);
    }
    close(sock2);
    auto udp_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> udp_elapsed = udp_end - udp_start;
    std::cout << "UDP: Sent " << message_count << " messages in " << udp_elapsed.count() << " seconds" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " -s (server) | -c <host> <message_count> (client)" << std::endl;
        return 1;
    }

    if (std::string(argv[1]) == "-s") {
        std::cout << "Starting servers..." << std::endl;
        std::thread tcp_thread(tcp_server);
        std::thread udp_thread(udp_server);

        tcp_thread.join();
        udp_thread.join();
    } 
    else if (std::string(argv[1]) == "-c" && argc >= 4) {
        std::string host = argv[2];
        int message_count = std::stoi(argv[3]);
        run_client(host, message_count);
    } 
    else {
        std::cerr << "Invalid arguments" << std::endl;
        return 1;
    }

    return 0;
}
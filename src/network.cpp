#include "network.h"

namespace Network {
int sockfd;
}  // namespace Network

void Network::createConnection(const char* serverIp, const char* serverPort) {
    sockaddr_in serverAddress;
    in_addr inaddr;

    serverAddress.sin_family = AF_INET;
    // port in network byte order
    serverAddress.sin_port = htons(std::stoi(std::string(serverPort)));
    inaddr.s_addr = inet_addr(serverIp);
    serverAddress.sin_addr = inaddr;

    if ((Network::sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Client socket error\n";
    }

    if (connect(Network::sockfd, (sockaddr*)&serverAddress, sizeof serverAddress) < 0) {
        std::cerr << "Client connection error\n";
    }

    std::cout << "Client connected to " << serverIp << '\n';
}

void Network::sendMsg(uint8_t type, uint32_t len, uint8_t* msg) {
    // 1 Byte for type; 4 Byte for length; len Bytes for msg
    uint8_t buffer[len + 5];
    buffer[0] = type;
    // get all 4 bytes of length to the right spot in the buffer
    buffer[1] = (len >> 24) & 0xFF;
    buffer[2] = (len >> 16) & 0xFF;
    buffer[3] = (len >> 8) & 0xFF;
    buffer[4] = len & 0xFF;

    for (uint32_t i = 0; i < (len); i++) {
        buffer[5 + i] = msg[i];
    }
    if (send(Network::sockfd, buffer, (len + 5), 0) < 0) {
        std::cerr << "client: send msg failed\n";
        exit(EXIT_FAILURE);
    }
}

void Network::recvMsg(uint8_t* type, uint32_t* len, uint8_t** msg) {
    uint8_t* buffer;

    // recv type and length of msg
    buffer = new uint8_t[5];
    ssize_t rec = 0;
    do {
        int result = recv(Network::sockfd, &buffer[rec], 5 - rec, 0);
        if (result == -1) {
            std::cerr << "client: receive msg failed\n";
            exit(EXIT_FAILURE);
        } else if (result == 0) {
#ifdef CONNECTION_LOGGING
            std::cout << "server disconnected tcp-connection\n";
#endif
            *type = 0;
            return;
        } else {
            rec += result;
        }
    } while (rec < 5);

    *type = buffer[0];
    *len = (buffer[1] << 24) + (buffer[2] << 16) + (buffer[3] << 8) + buffer[4];
    delete[] buffer;

    // recv msg
    if ((*len) == 0) {
        // no msg part
        return;
    }
    buffer = new uint8_t[*len];
    rec = 0;
    do {
        int result = recv(Network::sockfd, &buffer[rec], *len - rec, 0);
        if (result == -1) {
            std::cerr << "client failed to receive msg\n";
            exit(EXIT_FAILURE);
        } else if (result == 0) {
#ifdef CONNECTION_LOGGING
            std::cout << "server disconnected tcp-connection\n";
#endif
            *type = 0;
            return;
        } else {
            rec += result;
        }
    } while (rec < *len);
    *msg = buffer;
}
#include <time.h>

#include <cstdint>
#include <iostream>
#include <random>
#include <string>

#include "client.h"
#include "network.h"

int main(int argc, char* argv[]) {
    const char* ip = nullptr;
    const char* port = nullptr;

    // get ip and port from input parameters
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-i" || std::string(argv[i]) == "--ip") {
            i++;
            ip = argv[i];
        }
        if (std::string(argv[i]) == "-p" || std::string(argv[i]) == "--port") {
            i++;
            port = argv[i];
        }
        if (std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help") {
            std::cout << "Options:\n--ip\t\tip adress to connect to\t\toptional (default: 127.0.0.1\n--port\t\tport to connect to\t\toptional (default: 7777)\n--help\t\tdisplays help information\n";
        }
    }

    // set standard inputs for ip and port
    if (!ip || std::string(ip) == "localhost") {
        ip = "127.0.0.1";
    }
    if (!port) {
        port = "7777";
    }

    // initialize random with current time
    std::srand(time(NULL));

#ifdef CONNECTION_LOGGING
    std::cout << "Connecting to " << ip << " on port " << port << "...\n";
#endif
    Network::createConnection(ip, port);
    Client::runClient();

    return 0;
}
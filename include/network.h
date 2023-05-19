#ifndef NETWORK_H
#define NETWORK_H

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <cstdint>
#include <iostream>
#include <string>

#include "logging.h"

namespace Network {

/** @brief file descriptor for the clients socket */
extern int sockfd;

/** @brief Creates a connection to the server
 * @param serverIp ip string to connect
 * @param serverPort port string to connect
 */
void createConnection(const char* serverIp, const char* serverPort);

/** @brief Creates a connection to the server
 * @param type ip to connect
 * @param len port to connect
 * @param msg message to send (not NULL terminated)
 */
void sendMsg(uint8_t type, uint32_t len, uint8_t* msg);

/** @brief Creates a connection to the server
 * @param[out] type returns the type of the message
 * @param[out] len returns the length of the message
 * @param[out] msg returns a pointer to the message (not NULL terminated)
 */
void recvMsg(uint8_t* type, uint32_t* len, uint8_t** msg);
}  // namespace Network

#endif  // NETWORK_H

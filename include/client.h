#ifndef CLIENT_H
#define CLIENT_H

#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>
#include <atomic>

#include "currentstate.h"
#include "logging.h"
#include "map.h"
#include "moves.h"
#include "network.h"
#include "players.h"
#include "settings.h"

namespace Client {
/** @brief looks out for messages from the server and reacts accordingly
 */
void runClient();
}  // namespace Client

#endif  // CLIENT_H

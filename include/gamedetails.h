#ifndef GAMEDETAILS_H
#define GAMEDETAILS_H

#include <cstdint>
#include <map>
#include <sstream>
#include <string>

#include "logging.h"
#include "settings.h"

namespace GameDetails {
extern uint8_t playerCount;
extern uint8_t playerNumber;

extern uint8_t boardWidth;
extern uint8_t boardHeight;

/** @brief sets playerCount from map line
 * @param line line of the map containing the player count
 * @return bool
 * @retval true setting playerCount succeeded
 * @retval false setting playerCount failed
 */
bool setPlayerCount(std::string& line);

/** @brief sets boardWidth and boardHeight from map line
 * @param line line of the map containing the board width and board height
 * @return bool
 * @retval true setting boardWidth and boardHeight succeeded
 * @retval false setting boardWidth and boardHeight failed
 */
bool setBoardDimensions(std::string& line);
}  // namespace GameDetails

extern std::map<char, uint8_t> codeLookup;

extern std::map<uint8_t, char> asciiLookup;

extern uint8_t oppositeDirectionLookup[8];

#endif  // GAMEDETAILS_H

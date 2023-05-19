#ifndef PLAYERS_H
#define PLAYERS_H

#include <cstdint>

#include "currentstate.h"
#include "gamedetails.h"
#include "logging.h"
#include "player.h"

namespace Players {
/** @brief calculates the active player count (non-disqualified players)
 * @return uint8_t
 * @retval [2-8] depending on the number of non-disqualified players
 */
uint8_t getActivePlayerCount();
}  // namespace Players

#endif  // PLAYERS_H

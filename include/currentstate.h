#ifndef CURRENTSTATE_H
#define CURRENTSTATE_H

#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>

#include "arrayops.h"
#include "gamedetails.h"
#include "logging.h"
#include "move.h"
#include "moves.h"
#include "neighbor.h"
#include "player.h"
#include "settings.h"

namespace CurrentState {
extern uint32_t timelimit;
extern uint8_t searchDepth;
extern uint8_t activePlayerCount;

extern uint16_t moveCount;

extern uint8_t** boardArr;
extern Neighbor*** mapNeighbors;
extern Player* playerArr;

/** @brief creates the board array with GameDetails::boardWidth and GameDetails::boardHeight
 */
void createBoardArr();

/** @brief correctly deletes the board array and sets it to nullptr
 */
void deleteBoardArr();

/** @brief populates boardArr from board section of the map
 * @param boardStream section of the map containing the board
 * @return bool
 * @retval true populating boardArr succeeded
 * @retval false populating boardArr failed
 */
bool populateBoardArr(std::stringstream& boardStream);

/** @brief prints boardArr to cout
 */
void printBoardArr();

/** @brief populates the playerArr in CurrentState with a Player object for each player (index + 1 = player number)
 */
void populatePlayers();

/** @brief creates the map neighbors array initialized with nullptr in dimension 3
 */
void createMapNeighbors();

/** @brief correctly deletes map neighbors array and sets it to nullptr
 */
void deleteMapNeighbors();

/** @brief populates mapNaighbors from natural neighbors (see boardArr) and transitions section of the map
 * @return bool
 * @retval true populating mapNeighbors succeeded
 * @retval false populating mapNeighbors failed
 */
bool populateNeighbors();


}  // namespace CurrentState

#endif  // CURRENTSTATE_H

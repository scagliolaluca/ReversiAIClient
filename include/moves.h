#ifndef MOVES_H
#define MOVES_H

#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

#include "arrayops.h"
#include "currentstate.h"
#include "gamedetails.h"
#include "logging.h"
#include "move.h"
#include "player.h"
#include "settings.h"

namespace Moves {

/** @brief get the best possible move in given time or depth (coordinates what search algorithm is used)
 * @param[out] x return x-coordinate of the move
 * @param[out] y return y-coordinate of the move
 * @param[out] choice return choice of the move (only needed when moving on to a bonus- or choice-tile (otherwise 0))
 */
void getMove(uint8_t& x, uint8_t& y, uint8_t& choice);

/** @brief get a random move
 * @param[out] x return x-coordinate of the move
 * @param[out] y return y-coordinate of the move
 * @param board gameboard on which the moves must be valid
 * @param playerNumber player whose turn it is
 */
void getRandomMove(uint8_t& x, uint8_t& y, uint8_t** board, uint8_t playerNumber);

/** @brief validate a given move
 * @param board gameboard for move validation
 * @param x x-coordinate of the move
 * @param y y-coordinate of the move
 * @param playerNumber player for whom the move is validated
 * @return bool
 * @retval true move is valid
 * @retval false move is invalid
 */
bool isValidMove(uint8_t** board, uint8_t x, uint8_t y, uint8_t playerNumber);

/** @brief validate a given move for a specific direction
 * @param board gameboard for move validation
 * @param x x-coordinate of the move
 * @param y y-coordinate of the move
 * @param playerNumber player for whom the move is validated
 * @param direction direction in which th move should be validated
 * @return int
 * @retval -1 move cannot be valid (no need to check other directions)
 * @retval 0 move is invalid for given direction (try the next direction)
 * @retval 1 move is valid for given direction (--> move is valid)
 */
int validateDirection(uint8_t** board, uint8_t x, uint8_t y, uint8_t playerNumber, uint8_t direction);

/** @brief populate valid moves and valid override moves
 * @param[out] validMoves return vector with populated valid moves
 * @param board gameboard on which the moves must be valid
 * @param playerNumber player for whom the moves must be valid
 */
void populateValidMoves(std::vector<Move>& validMoves, uint8_t** board, uint8_t playerNumber);

/** @brief makes a valid Move on given gameboard (move does not get validated!)
 * @param board gameboard on which to move
 * @param x x-coordinate of the move
 * @param y y-coordinate of the move
 * @param playerNumber player for whom the moves is done
 */
void makeMove(uint8_t** board, uint8_t x, uint8_t y, uint8_t playerNumber);

/** @brief mark tiles for recolor in a given direction
 * @param board gameboard on which to recolor
 * @param x x-coordinate of the move
 * @param y y-coordinate of the move
 * @param playerNumber player for whom the recolor is done
 * @param direction direction in which to look for recolor
 * @param[out] markedTiles return vector of pointers to marked tiles on the boardArr
 */

void makeMove(uint8_t** board, uint8_t x, uint8_t y, uint8_t playerNumber, uint32_t& hash);

/** like makeMove above, just with TranspositionTable functionality
 * @param board gameboard on which to recolor
 * @param x x-coordinate of the move
 * @param y y-coordinate of the move
 * @param playerNumber player for whom the recolor is done
 * @param direction direction in which to look for recolor
 * @param[out] markedTiles return vector of pointers to marked tiles on the boardArr
 * @param hash is the current hash value of board
 */

void markRecolor(uint8_t** board, uint8_t x, uint8_t y, uint8_t playerNumber, uint8_t direction, std::vector<uint8_t*>& markedTiles);

/** @brief recolor all marked tiles
 * @param playerNumber player for whom the recolor is done
 * @param markedTiles vector of pointers to marked tiles on the boardArr
 */

void markRecolor(uint8_t** board, uint8_t x, uint8_t y, uint8_t playerNumber, uint8_t direction, std::vector<uint8_t*>& markedTiles, uint32_t& hash);

void recolor(uint8_t playerNumber, std::vector<uint8_t*>& markedTiles);

}  // namespace Moves

#endif  // MOVES_H

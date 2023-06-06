#ifndef HEURISTICS_H
#define HEURISTICS_H

#include <cstdint>

namespace Heuristics {
    // Utility function
    int evaluateEndState(uint8_t** board, uint8_t playerNumber);

    uint8_t playerRanking(uint8_t **board, uint8_t playerNumber);

    // Heuristic
    int weightedHeuristic(uint8_t** board, uint8_t playerNumber);

    int getScore(uint8_t** board, uint8_t playerNumber);
    int getScoreEnemyMoves(uint8_t** board, uint8_t ourPlayerNumber);
    int getScoreTakeEnemy(uint8_t** board, uint8_t playerNumber);
    int getMovecount(uint8_t** board, uint8_t playerNumber);
    void copyBoard(uint8_t** copyBoard, uint8_t** board);
    void printBoard(uint8_t** board);
} // namespace Heuristics
#endif //HEURISTICS_H

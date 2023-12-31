#ifndef HEURISTICS_H
#define HEURISTICS_H

#include <cstdint>
#include <vector>

namespace Heuristics {
    // Utility function
    float evaluateEndState(uint8_t** board, uint8_t playerNumber);

    uint8_t playerRanking(uint8_t **board, uint8_t playerNumber);

    // Heuristic
    struct Weights {
        int myPieces = 0;
        int myPieceValue = 0;
        int myMoves = 0;

        int enemyPieces = 0;
        int enemyPieceValue = 0;
        int enemyMoves = 0;
    };
    float normalizedHeuristic(uint8_t** board, uint8_t playerNumber);
    Weights heuristicWeightsForGameProgress(float progress);
    float normalizedMyHeuristicValue(float myValue, float totalValue, uint8_t playerCount);
    float normalizedOthersHeuristicValue(float othersValue, float totalValue, uint8_t playerCount);

    float weightedHeuristic(uint8_t** board, uint8_t playerNumber);

    int getScore(uint8_t** board, uint8_t playerNumber);
    void getScores(std::vector<float> &rawScores, std::vector<float> &weightedScores, uint8_t **board, float **valueMask);
    int getScoreEnemyMoves(uint8_t** board, uint8_t ourPlayerNumber);
    int getScoreTakeEnemy(uint8_t** board, uint8_t playerNumber);
    int getMovecount(uint8_t** board, uint8_t playerNumber);

    // Helpers / Debugging
    void copyBoard(uint8_t** copyBoard, uint8_t** board);
    void printBoard(uint8_t** board);
} // namespace Heuristics
#endif //HEURISTICS_H

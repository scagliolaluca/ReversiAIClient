#pragma once

#include <cstdint>
#include <deque>

namespace MapPreprocessing
{
    extern float **tileValueMask;
    extern int nReachableTiles;

    void createValueMask();
    void deleteValueMask();
    float **createRawValues();
    void deductNeighbors(float **rawValueArray);
    void getBlockedAxes(uint8_t &blockedTakeDirections, uint8_t &blockedGetTakenAxes, uint8_t x, uint8_t y);

    // Calculates the number of reachable tiles.
    // Note 1: This doesn't take player numbers into account -> upper bound (true value may be lower)
    // Note 2: During a game, the number of reachable tiles (taking player numbers into account) may decrease
    void calculateReachableTiles();
    void initializeStartingTiles(int8_t **reachableTiles, std::deque<std::pair<uint8_t, uint8_t>> &candidates);
    void addNewCandidates(uint8_t x, uint8_t y, int8_t **reachableTiles, std::deque<std::pair<uint8_t, uint8_t>> &candidates);
    bool isTileReachable(uint8_t x, uint8_t y, int8_t **reachableTiles);
    int countReachable(int8_t **reachableTiles);
} // namespace MapPreprocessing

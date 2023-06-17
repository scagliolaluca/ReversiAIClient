#pragma once

#include <cstdint>

namespace MapPreprocessing
{
    extern float **tileValueMask;
    extern int reachableTiles;

    void createValueMask();
    void deleteValueMask();
    float **createRawValues();
    void deductNeighbors(float **rawValueArray);
    void getBlockedAxes(uint8_t &blockedTakeDirections, uint8_t &blockedGetTakenAxes, uint8_t x, uint8_t y);

    void calculateReachableTiles();
    bool isTileReachable(uint8_t x, uint8_t y, int8_t **reachableTiles);
} // namespace MapPreprocessing

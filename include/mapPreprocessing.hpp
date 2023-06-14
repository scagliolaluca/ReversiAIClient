#pragma once

#include <cstdint>

namespace MapPreprocessing
{
    extern float **tileValueMask;

    void createValueMask();
    void deleteValueMask();

    float **createRawValues();
    void deductNeighbors(float **rawValueArray);

    void getBlockedAxes(uint8_t &blockedTakeDirections, uint8_t &blockedGetTakenAxes, uint8_t x, uint8_t y);
} // namespace MapPreprocessing

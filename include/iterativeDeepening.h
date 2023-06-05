#ifndef ITERATIVEDEEPENING_H
#define ITERATIVEDEEPENING_H

#include <cstdint>
#include <chrono>
#include <functional>

#include "minimax.hpp"

namespace IterativeDeepening
{
    void getMoveIterativeDeepening(uint8_t &x, uint8_t &y, uint8_t **board, uint8_t playerNumber, uint32_t maxTime, const std::chrono::time_point<std::chrono::steady_clock> startTime, const std::function<int(uint8_t **)> &heuristic);
    bool timeForNextIteration(uint8_t iterationDepth, const std::chrono::time_point<std::chrono::steady_clock> startTime, uint32_t maxTime);
    bool checkForTimeEnd(uint32_t maxTime, const std::chrono::time_point<std::chrono::steady_clock> startTime);
} //namespace IterativeDeepening

#endif //ITERATIVEDEEPENING_H
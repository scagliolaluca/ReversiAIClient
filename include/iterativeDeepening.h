#ifndef ITERATIVEDEEPENING_H
#define ITERATIVEDEEPENING_H

#include <cstdint>
#include <chrono>
#include <functional>
#include <deque>

#include "minimax.hpp"

namespace IterativeDeepening
{
    void getMoveIterativeDeepening(uint8_t &x, uint8_t &y, uint8_t **board, uint8_t playerNumber, const std::function<int(uint8_t **, uint8_t)> &heuristic, const std::chrono::time_point<std::chrono::steady_clock> &stopTime);
    bool timeForNextIteration(uint8_t iterationDepth, const std::chrono::time_point<std::chrono::steady_clock> &stopTime, const std::deque<std::chrono::duration<double>> &iterationDurations);
} //namespace IterativeDeepening

#endif //ITERATIVEDEEPENING_H
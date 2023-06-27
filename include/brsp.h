#ifndef BRSP_H
#define BRSP_H

#include <cstdint>
#include <functional>
#include <chrono>
#include <vector>
#include <limits>

namespace BRSP {

    bool getMoveBRSP(uint8_t& x, uint8_t& y, uint8_t** board, uint8_t playerNumber, uint8_t depth, const std::function<float(uint8_t**, uint8_t)>& heuristic, const std::chrono::time_point<std::chrono::steady_clock>& stopTime);

    float BRSPHelper(uint8_t** board, uint8_t depth, uint8_t normalMovesLeft, uint8_t playerNumber, uint8_t maxnumber, const std::function<float(uint8_t**, uint8_t)>& heuristic, const std::chrono::time_point<std::chrono::steady_clock>& stopTime);

} // namespace BRSP

#endif // BRSP_H

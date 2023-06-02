#pragma once

#include "arrayops.h"
#include "move.h"
#include "iterativeDeepening.h"

#include <cstdint>
#include <functional>
#include <vector>
#include <chrono>

namespace Minimax
{
    struct Node {
        Node() = default;
        Node(const Node &other) = delete;
        Node(Node &&other);
        ~Node();

        uint8_t **board;
        std::vector<Move> validMoves;
        int value;
        uint8_t player;
    };

    bool getMoveMinimaxTimeCheck(uint8_t &x, uint8_t &y, uint8_t **board, uint8_t playerNumber, uint8_t maxDepth, uint32_t maxTime, const std::chrono::time_point<std::chrono::steady_clock> startTime, const std::function<int(uint8_t **)> &heuristic);
    void getMoveMinimax(uint8_t &x, uint8_t &y, uint8_t **board, uint8_t playerNumber, uint8_t maxDepth, const std::function<int(uint8_t **)> &heuristic);

    uint8_t highestPieceCount(uint8_t **board);
    uint8_t nextValidPlayerMoves(std::vector<Move> &validMoves, uint8_t **board, uint8_t currentPlayer);
} // namespace Minimax


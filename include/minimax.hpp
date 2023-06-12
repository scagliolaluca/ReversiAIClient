#pragma once

#include "arrayops.h"
#include "move.h"
#include "iterativeDeepening.h"

#include <limits.h>
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

        bool hasValidMoves();
        const Move &nextMove();

        uint8_t **board;
        std::vector<Move> validMoves;
        uint currentMoveIndex = 0;
        int value;
        uint8_t player;

        int alpha = INT_MIN;
        int beta = INT_MAX;
    };
    void getMoveMinimax(uint8_t &x, uint8_t &y, uint8_t **board, uint8_t playerNumber, uint8_t maxDepth, const std::function<int(uint8_t **, uint8_t)> &heuristic);
    bool getMoveMinimax(uint8_t &x, uint8_t &y, bool &reachedMaxDepth, uint8_t **board, uint8_t playerNumber, uint8_t maxDepth, const std::function<int(uint8_t **, uint8_t)> &heuristic, const std::chrono::time_point<std::chrono::steady_clock> &stopTime);
    bool checkTimeLeft(const std::chrono::time_point<std::chrono::steady_clock> &stopTime);
    void sortMoves(std::vector<Move> &moves, uint8_t **board, uint8_t player);
    uint8_t nextValidPlayerMoves(std::vector<Move> &validMoves, uint8_t **board, uint8_t currentPlayer);

} // namespace Minimax


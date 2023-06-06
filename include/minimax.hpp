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

        bool hasValidMoves();
        const Move &nextMove();

        uint8_t **board;
        std::vector<Move> validMoves;
        uint currentMoveIndex = 0;
        int value;
        uint8_t player;
    };

    void getMoveMinimax(uint8_t &x, uint8_t &y, uint8_t **board, uint8_t playerNumber, uint8_t maxDepth, const std::function<int(uint8_t **, uint8_t)> &heuristic);

    uint8_t nextValidPlayerMoves(std::vector<Move> &validMoves, uint8_t **board, uint8_t currentPlayer);
} // namespace Minimax


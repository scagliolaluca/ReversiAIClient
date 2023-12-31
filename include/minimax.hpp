#pragma once

#include "arrayops.h"
#include "move.h"
#include "transpositionTable.h"
#include "iterativeDeepening.h"
#include "zobristKey.h"

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
        float value;
        uint8_t player;

        float alpha;
        float beta;

        TranspositionTable::Entry bestFollowing;
        uint8_t moveToGetHere_x;
        uint8_t moveToGetHere_y;
        uint32_t hash;
    };
    void getMoveMinimax(uint8_t &x, uint8_t &y, uint8_t **board, uint8_t playerNumber, uint8_t maxDepth, const std::function<float(uint8_t **, uint8_t)> &heuristic);
    bool getMoveMinimax(uint8_t &x, uint8_t &y, bool &reachedMaxDepth, uint8_t **board, uint8_t playerNumber, uint8_t maxDepth, const std::function<float(uint8_t **, uint8_t)> &heuristic, const std::chrono::time_point<std::chrono::steady_clock> &stopTime);
    bool checkTimeLeft(const std::chrono::time_point<std::chrono::steady_clock> &stopTime);
    std::vector<int> getEvaluationVector(std::vector<Move> &moves, uint8_t **board, uint8_t player);
    void sortMoves(std::vector<Move> &moves, uint8_t **board, uint8_t player);
    uint8_t nextValidPlayerMoves(std::vector<Move> &validMoves, uint8_t **board, uint8_t currentPlayer);

} // namespace Minimax


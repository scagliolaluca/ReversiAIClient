#include "heuristics.h"

#include "gamedetails.h"
#include "moves.h"

#include <iostream>
#include <vector>

namespace Heuristics
{
    int weightedHeuristic(uint8_t** board) {
        // TODO: weights
        return getScore(board, GameDetails::playerNumber) + getMovecount(board, GameDetails::playerNumber);
    }

    int getScore(uint8_t** board, uint8_t playerNumber) {
        int score = 0;

        for (uint8_t i = 0; i < GameDetails::boardHeight; i++) {
            for (uint8_t j = 0; j < GameDetails::boardWidth; j++) {
                /*
                char v = board[i][j];
                std::map<uint8_t, char>::const_iterator it = asciiLookup.find(v);
                if (it == asciiLookup.cend()) {
                    std::cerr << "Lookup Error occurred while printing boardArr...\n";
                    return 0;
                }
                char c = it->second;

                if ((int)c - 48 == int(playerNumber)) {
                    score += 1;
                }
                */
                if (board[i][j] == playerNumber) {
                    ++score;
                }
            }
        }
        return score;
    }

    int getMovecount(uint8_t** board, uint8_t playerNumber) {
        std::vector<Move> validMoves;
        Moves::populateValidMoves(validMoves, board, playerNumber);
        return int(validMoves.size());
    }

    void printBoard(uint8_t** board) {
        for (uint8_t i = 0; i < GameDetails::boardHeight; i++) {
            for (size_t j = 0; j < GameDetails::boardWidth; j++) {
                char v = board[i][j];
                std::map<uint8_t, char>::const_iterator it = asciiLookup.find(v);
                if (it == asciiLookup.cend()) {
                    std::cerr << "Lookup Error occurred while copying boardArr...\n";
                    return;
                }
                char c = it->second;
                std::cout << c << " ";
            }
            std::cout << '\n';
        }
    }

    void copyBoard(uint8_t** copyBoard, uint8_t** board) {
        for (uint8_t i = 0; i < GameDetails::boardHeight; i++) {
            for (size_t j = 0; j < GameDetails::boardWidth; j++) {
                copyBoard[i][j] = board[i][j];
            }
        }
    }
} // namespace Heuristics

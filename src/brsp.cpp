#include "brsp.h"
#include "heuristics.h"
#include "moves.h"

#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

namespace BRSP {

    bool getMoveBRSP(uint8_t& x, uint8_t& y, bool& miniMaxReachedMaxDepth, uint8_t** board, uint8_t playerNumber, uint8_t depth, const std::function<float(uint8_t**, uint8_t)>& heuristic, const std::chrono::time_point<std::chrono::steady_clock>& stopTime) {
        if (std::chrono::steady_clock::now() >= stopTime) {
            return false;
        }

        std::vector<Move> availableMoves;
        Moves::populateValidMoves(availableMoves, board, playerNumber);

        float bestValue = std::numeric_limits<float>::lowest();

        for (const auto& move : availableMoves) {
            uint8_t newX = move.x;
            uint8_t newY = move.y;

            uint8_t** boardcopy = create2DArr<uint8_t>(GameDetails::boardHeight, GameDetails::boardWidth);
            Heuristics::copyBoard(boardcopy, board);
            Moves::makeMove(boardcopy, newX, newY, playerNumber);

            float moveValue = BRSPHelper(boardcopy, depth - 1, 1, playerNumber, playerNumber, heuristic, stopTime); 

            if (moveValue > bestValue) {
                bestValue = moveValue;
                x = newX;
                y = newY;
            }
        }

        return true;
    }

    float BRSPHelper(uint8_t** board, uint8_t depth, uint8_t normalMovesLeft, uint8_t playerNumber, uint8_t maxnumber, const std::function<float(uint8_t**, uint8_t)>& heuristic, const std::chrono::time_point<std::chrono::steady_clock>& stopTime) {
        if (depth == 0) { //terminal state check function? eh
            return heuristic(board, playerNumber);
        }
        if (std::chrono::steady_clock::now() >= stopTime) {
            return 0.0; 
        }

        bool maximizingPlayer = playerNumber == maxnumber;
        std::vector<Move> availableMoves;
        Moves::populateValidMoves(availableMoves, board, playerNumber);

        if (maximizingPlayer) {
            float bestValue = std::numeric_limits<float>::lowest();

            for (const auto& move : availableMoves) {
                uint8_t newX = move.x;
                uint8_t newY = move.y;

                uint8_t** boardcopy = create2DArr<uint8_t>(GameDetails::boardHeight, GameDetails::boardWidth);
                Heuristics::copyBoard(boardcopy, board);
                Moves::makeMove(boardcopy, newX, newY, playerNumber);

                float moveValue = BRSPHelper(board, depth - 1, 1, (playerNumber + 1) % GameDetails::playerCount, maxnumber, heuristic, stopTime); 

                bestValue = std::max(bestValue, moveValue);
            }

            return bestValue;
        } else {
            float bestValue = std::numeric_limits<float>::max();
            if(normalMovesLeft < 1){
                int i = rand() % availableMoves.size(); //picking random move as special move
                Move specialMove = availableMoves[i];
                availableMoves.erase(availableMoves.begin()+i);

                uint8_t newX = specialMove.x;
                uint8_t newY = specialMove.y;
                
                uint8_t** boardcopy = create2DArr<uint8_t>(GameDetails::boardHeight, GameDetails::boardWidth);
                Heuristics::copyBoard(boardcopy, board);
                Moves::makeMove(boardcopy, newX, newY, playerNumber);

                float moveValue = BRSPHelper(board, depth - 1, normalMovesLeft, (playerNumber + 1) % GameDetails::playerCount, maxnumber, heuristic, stopTime);

                bestValue = std::min(bestValue, moveValue);
                return bestValue;
            }
            else if (normalMovesLeft > 0 && (playerNumber + 1) % GameDetails::playerCount == maxnumber) //muss normal moves anschauen da nächster wieder max ist
            {
                for (const auto& move : availableMoves) {
                    uint8_t newX = move.x;
                    uint8_t newY = move.y;
                    
                    uint8_t** boardcopy = create2DArr<uint8_t>(GameDetails::boardHeight, GameDetails::boardWidth);
                    Heuristics::copyBoard(boardcopy, board);
                    Moves::makeMove(boardcopy, newX, newY, playerNumber);

                    float moveValue = BRSPHelper(board, depth - 1, normalMovesLeft-1 , (playerNumber + 1) % GameDetails::playerCount, maxnumber, heuristic, stopTime);

                    bestValue = std::min(bestValue, moveValue);
                }
                return bestValue;
            }else{ //nächster ist min und wir haben normalmove übrig, alles betrachten
                int i = rand() % availableMoves.size(); //picking random move as special move
                Move specialMove = availableMoves[i];
                availableMoves.erase(availableMoves.begin()+i);

                uint8_t newX = specialMove.x;
                uint8_t newY = specialMove.y;
                
                uint8_t** boardcopy = create2DArr<uint8_t>(GameDetails::boardHeight, GameDetails::boardWidth);
                Heuristics::copyBoard(boardcopy, board);
                Moves::makeMove(boardcopy, newX, newY, playerNumber);

                float moveValue = BRSPHelper(board, depth - 1, 1, (playerNumber + 1) % GameDetails::playerCount, maxnumber, heuristic, stopTime);

                bestValue = std::min(bestValue, moveValue);

                for (const auto& move : availableMoves) {
                    uint8_t newX = move.x;
                    uint8_t newY = move.y;
                    
                    uint8_t** boardcopy = create2DArr<uint8_t>(GameDetails::boardHeight, GameDetails::boardWidth);
                    Heuristics::copyBoard(boardcopy, board);
                    Moves::makeMove(boardcopy, newX, newY, playerNumber);

                    float moveValue = BRSPHelper(board, depth - 1, normalMovesLeft-1 , (playerNumber + 1) % GameDetails::playerCount, maxnumber, heuristic, stopTime);

                    bestValue = std::min(bestValue, moveValue);
                }
                return bestValue;
            }
        }
    }
}

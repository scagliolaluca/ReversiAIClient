#include "brsp.h"
#include "heuristics.h"
#include "moves.h"
#include "minimax.hpp"

#include <stdexcept>
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

namespace BRSP {

    bool getMoveBRSP(uint8_t& x, uint8_t& y, uint8_t** board, uint8_t playerNumber, uint8_t depth, const std::function<float(uint8_t**, uint8_t)>& heuristic, const std::chrono::time_point<std::chrono::steady_clock>& stopTime) {
        if (std::chrono::steady_clock::now() >= stopTime) {
            return false;
        }

        std::vector<Move> availableMoves;
        Moves::populateValidMoves(availableMoves, board, playerNumber);

        float bestValue = std::numeric_limits<float>::lowest();

        for (const auto& move : availableMoves) {
            uint8_t newX = move.x;
            uint8_t newY = move.y;

            uint8_t** boardcopy = copy2DArr(board, GameDetails::boardHeight, GameDetails::boardWidth);
            Moves::makeMove(boardcopy, newX, newY, playerNumber);
            
            float moveValue = std::numeric_limits<float>::lowest();
            try{
                moveValue = BRSPHelper(boardcopy, depth - 1, 1, playerNumber, playerNumber, heuristic, stopTime); 
            }catch(const std::exception& e){
                return false; //time over
            }

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
        //if (std::chrono::steady_clock::now() >= stopTime) {
        //    throw std::runtime_error("not enough time");
        //}
        if(!Minimax::checkTimeLeft(stopTime)){
            throw std::runtime_error("not enough time");
        }

        bool maximizingPlayer = playerNumber == maxnumber;
        bool nextIsMaxPlayer = (playerNumber + 1) % GameDetails::playerCount == maxnumber;
        std::vector<Move> availableMoves;
        Moves::populateValidMoves(availableMoves, board, playerNumber);
        Minimax::sortMoves(availableMoves,board,playerNumber);

        if (maximizingPlayer) {
            float bestValue = std::numeric_limits<float>::lowest();

            for (const auto& move : availableMoves) {
                uint8_t** boardcopy = copy2DArr(board, GameDetails::boardHeight, GameDetails::boardWidth);
                Moves::makeMove(boardcopy, move.x, move.y, playerNumber);

                float moveValue = BRSPHelper(board, depth - 1, 1, (playerNumber + 1) % GameDetails::playerCount, maxnumber, heuristic, stopTime); 
                bestValue = std::max(bestValue, moveValue);
                delete2DArr(boardcopy,GameDetails::boardHeight);
            }
            return bestValue;
        } else {
            float bestValue = std::numeric_limits<float>::max();

            if(normalMovesLeft == 0 || (normalMovesLeft > 0 && !nextIsMaxPlayer)){
                //int i = rand() % availableMoves.size(); //picking random move as special move
                //Move specialMove = availableMoves[i];
                //availableMoves.erase(availableMoves.begin()+i);
                Move specialMove = availableMoves[0];
                availableMoves.erase(availableMoves.begin());

                uint8_t** boardcopy = copy2DArr(board, GameDetails::boardHeight, GameDetails::boardWidth);
                Moves::makeMove(boardcopy, specialMove.x, specialMove.y, playerNumber);

                float moveValue = BRSPHelper(board, depth - 1, normalMovesLeft, (playerNumber + 1) % GameDetails::playerCount, maxnumber, heuristic, stopTime);
                bestValue = std::min(bestValue, moveValue);
                delete2DArr(boardcopy,GameDetails::boardHeight);
            }

            if(normalMovesLeft > 0){
                for (const auto& move : availableMoves) {
                    uint8_t** boardcopy = copy2DArr(board, GameDetails::boardHeight, GameDetails::boardWidth);
                    Moves::makeMove(boardcopy, move.x, move.y, playerNumber);

                    float moveValue = BRSPHelper(board, depth - 1, normalMovesLeft-1 , (playerNumber + 1) % GameDetails::playerCount, maxnumber, heuristic, stopTime);
                    bestValue = std::min(bestValue, moveValue);
                    delete2DArr(boardcopy,GameDetails::boardHeight);
                }
            }

            return bestValue;
        }
    }
}

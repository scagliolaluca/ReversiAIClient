#include "heuristics.h"

#include "gamedetails.h"
#include "moves.h"

#include <iostream>
#include <vector>

namespace Heuristics
{
    int weightedHeuristic(uint8_t** board) {
        // TODO: weights; normalization (for player count and map size)
        int heuristic = 0;
        heuristic += getScore(board, GameDetails::playerNumber);
        heuristic += getMovecount(board, GameDetails::playerNumber);
        heuristic -= getScoreEnemyMoves(board, GameDetails::playerNumber);

        // Maybe remove: (Expensive and maybe not so helpful)
        heuristic += getScoreTakeEnemy(board, GameDetails::playerNumber);

        return heuristic;
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

    int getScoreEnemyMoves(uint8_t** board, uint8_t ourPlayerNumber){
        int numEnemyMoves = 0;
        for(int i = 1; i <= GameDetails::playerCount; i++){
            if (i != ourPlayerNumber) {
                numEnemyMoves += getMovecount(board,i);
            }
        }
        return numEnemyMoves;
    }

    int getScoreTakeEnemy(uint8_t** board, uint8_t playerNumber) {
        std::vector<Move> validMoves;
        Moves::populateValidMoves(validMoves, board, playerNumber);
        int numMovesThatTake = 0;
        uint8_t** boardcopy = create2DArr<uint8_t>(GameDetails::boardHeight, GameDetails::boardWidth);
        for(Move m : validMoves){
            Heuristics::copyBoard(boardcopy, board);
            Moves::makeMove(boardcopy, m.x, m.y, playerNumber);
            //if the move takes some enemy thingies
            bool takesEnemy = false;
            for(int i = 1; i <= GameDetails::playerCount; i++){
                if (i == playerNumber) continue;
                if (getScore(board, i) != getScore(boardcopy, i)) takesEnemy = true;
            }
            if (takesEnemy) numMovesThatTake++;
        }
        return numMovesThatTake;
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

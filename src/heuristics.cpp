#include "heuristics.h"

#include "gamedetails.h"
#include "moves.h"

#include <iostream>
#include <vector>
#include <limits.h>
#include <algorithm>

namespace Heuristics
{
    int evaluateEndState(uint8_t** board, uint8_t playerNumber) {
        uint8_t rank = playerRanking(board, playerNumber);

        // TODO tweak values
        switch (rank) {
            case 1:
                return INT_MAX;
            case 2:
                return 1;
            case 3:
                return -1;

            default:
                break;
        }

        return INT_MIN;
    }

    uint8_t playerRanking(uint8_t **board, uint8_t playerNumber) {
        std::vector<int> pieceCounts(GameDetails::playerCount, 0);
        uint8_t boardVal;

        // Calculate each player's score
        for (uint8_t i = 0; i < GameDetails::boardHeight; ++i) {
            for (uint8_t j = 0; j < GameDetails::boardWidth; ++j) {
                boardVal = board[i][j];
                if (boardVal >= 1 && boardVal <= GameDetails::playerCount) {
                    ++pieceCounts[boardVal - 1];
                }
            }
        }

        // Get a list of players, sorted by their scores
        std::vector<uint8_t> players;
        std::iota(players.begin(), players.end(), 1);
        std::sort(players.begin(), players.end(), [&pieceCounts](int i, int j){ return pieceCounts[i] < pieceCounts[j]; });

        // Obtain the actual rank
        uint8_t rank = 1;
        int prevPlayerScore = -1;
        for (uint i = 0; i < players.size(); ++i) {
            if (pieceCounts[i] != prevPlayerScore) {
                rank = i + 1;
            }
            if (players[i] == playerNumber) {
                break;
            }
            prevPlayerScore = pieceCounts[i];
        }

        return rank;
    }

    int weightedHeuristic(uint8_t** board, uint8_t playerNumber) {
        // TODO: weights; normalization (for player count and map size)
        int heuristic = 0;
        heuristic += 2 * getScore(board, playerNumber);
        heuristic += getMovecount(board, playerNumber);
        heuristic -= getScoreEnemyMoves(board, playerNumber);

        // Maybe remove: (Expensive and maybe not so helpful)
        // heuristic += getScoreTakeEnemy(board, playerNumber);

        return heuristic;
    }

    int getScore(uint8_t** board, uint8_t playerNumber) {
        int score = 0;

        for (uint8_t i = 0; i < GameDetails::boardHeight; i++) {
            for (uint8_t j = 0; j < GameDetails::boardWidth; j++) {
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

#include "heuristics.h"

#include "gamedetails.h"
#include "moves.h"

#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

namespace Heuristics
{
    float evaluateEndState(uint8_t** board, uint8_t playerNumber) {
        uint8_t rank = playerRanking(board, playerNumber);

        if (rank == 1) {
            return std::numeric_limits<float>::max();
        }
        else if (rank == GameDetails::playerCount) {
            return std::numeric_limits<float>::lowest();
        }

        // TODO: balancing in respect to heuristic
        if (GameDetails::playerCount == 4) {
            if (rank == 2) {
                return 0.5f;
            }
            else {
                return -0.5f;
            }
        }

        return 0;
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

    float normalizedHeuristic(uint8_t** board, uint8_t playerNumber) {
        const int PC = GameDetails::playerCount;
        // TODO: obtain these values... (more efficiently)
        int myPieceValue = getScore(board, playerNumber);
        int enemyPieceValue = 0;
        for (int i = 1; i <= GameDetails::playerCount; ++i) {
            if (i != playerNumber) {
                enemyPieceValue += getScore(board, i);
            }
        }
        int totalPieceValue = myPieceValue + enemyPieceValue;

        int myMoves = getMovecount(board, playerNumber);
        int enemyMoves = getScoreEnemyMoves(board, playerNumber);
        int totalMoves = myMoves + enemyMoves;

        // Weights for the heuristic parts
        // TODO: adjust depending on game state
        int w_myPieces = 2;
        int w_enemyPieces = 2;
        int w_myMoves = 1;
        int w_enemyMoves = 1;

        // Normalized piece score
        float h_myPieces = 0;

        float contribution = (float)myPieceValue / totalPieceValue;
        if (contribution <= (float)1 / PC) {
            h_myPieces = contribution * PC - 1;
        }
        else {
            h_myPieces = (contribution * PC - 1) / (PC - 1);
        }

        // Normalized enemy piece score
        float h_enemyPieces = 0;

        float enemyContribution = (float)enemyPieceValue / totalPieceValue;
        float pcRatio = (float)PC / (PC - 1);
        if (enemyContribution <= (float)(PC - 1) / PC) {
            h_enemyPieces = -enemyContribution * pcRatio + 1;
        }
        else {
            h_enemyPieces = -(enemyContribution * pcRatio - 1) / (pcRatio - 1);
        }

        // Normalized moves
        float h_myMoves = 0;

        float moveContribution = (float)myMoves / totalMoves;
        if (moveContribution <= (float)1 / PC) {
            h_myMoves = moveContribution * PC - 1;
        }
        else {
            h_myMoves = (moveContribution * PC - 1) / (PC - 1);
        }

        // Normalized enemy moves
        float h_enemyMoves = 0;

        float enemyMoveContribution = (float)enemyMoves / totalMoves;
        //float pcRatio = (float)PC / (PC - 1);
        if (enemyMoveContribution <= (float)(PC - 1) / PC) {
            h_enemyMoves = -enemyMoveContribution * pcRatio + 1;
        }
        else {
            h_enemyMoves = -(enemyMoveContribution * pcRatio - 1) / (pcRatio - 1);
        }

        // Blend heuristics
        float h = 0;
        h += w_myPieces * h_myPieces;
        h += w_enemyPieces * h_enemyPieces;
        h += w_myMoves * h_myMoves;
        h += w_enemyMoves * h_enemyMoves;
        h /= (w_myPieces + w_enemyPieces + w_myMoves + w_enemyMoves);

        return h;
    }

    float weightedHeuristic(uint8_t** board, uint8_t playerNumber) {
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

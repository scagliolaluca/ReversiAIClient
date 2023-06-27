#include "heuristics.h"

#include "gamedetails.h"
#include "moves.h"
#include "mapPreprocessing.hpp"

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

        // Data for heuristic calculation
        std::vector<float> rawScores;
        std::vector<float> weightedScores;
        getScores(rawScores, weightedScores, board, MapPreprocessing::tileValueMask);

        float myPieces = rawScores[playerNumber-1];
        float enemyPieces = 0;
        for (int i = 1; i <= GameDetails::playerCount; ++i) {
            if (i != playerNumber) {
                enemyPieces += rawScores[i-1];
            }
        }
        float totalPieces = myPieces + enemyPieces;

        float myPieceValue = weightedScores[playerNumber-1];
        float enemyPieceValue = 0;
        for (int i = 1; i <= GameDetails::playerCount; ++i) {
            if (i != playerNumber) {
                enemyPieceValue += weightedScores[i-1];
            }
        }
        float totalPieceValue = myPieceValue + enemyPieceValue;

        int myMoves = getMovecount(board, playerNumber);
        int enemyMoves = getScoreEnemyMoves(board, playerNumber);
        int totalMoves = myMoves + enemyMoves;

        // Normalized piece counts
        float h_myPieces = normalizedMyHeuristicValue(myPieces, totalPieces, PC);
        float h_enemyPieces = normalizedOthersHeuristicValue(enemyPieces, totalPieces, PC);

        // Normalized piece values
        float h_myPieceValue = normalizedMyHeuristicValue(myPieceValue, totalPieceValue, PC);
        float h_enemyPieceValue = normalizedOthersHeuristicValue(enemyPieceValue, totalPieceValue, PC);

        // Normalized moves
        float h_myMoves = normalizedMyHeuristicValue(myMoves, totalMoves, PC);
        float h_enemyMoves = normalizedOthersHeuristicValue(enemyMoves, totalMoves, PC);

        // Blend heuristics
        float progress = (float)totalPieces / MapPreprocessing::nReachableTiles;
        Weights w = heuristicWeightsForGameProgress(progress);
        float h = 0;
        h += w.myPieces * h_myPieces;
        h += w.enemyPieces * h_enemyPieces;
        h += w.myPieceValue * h_myPieceValue;
        h += w.enemyPieceValue * h_enemyPieceValue;
        h += w.myMoves * h_myMoves;
        h += w.enemyMoves * h_enemyMoves;
        h /= (w.myPieces + w.enemyPieces + w.myPieceValue + w.enemyPieceValue + w.myMoves + w.enemyMoves);

        return h;
    }

    Weights heuristicWeightsForGameProgress(float progress) {
        Weights w;

        // Enemy weights can be left at 0 because current normalization takes enemy values for own values into account
        if (progress <= 0.2f) {
            w.myPieces = 1;
            w.myPieceValue = 1;
            w.myMoves = 2;
        }
        else if (progress <= 0.9f) {
            w.myPieces = 1;
            w.myPieceValue = 1;
            w.myMoves = 1;
        }
        else {
            w.myPieces = 1;
            w.myPieceValue = 0;
            w.myMoves = 1;
        }

        return w;
    }
    float normalizedMyHeuristicValue(float myValue, float totalValue, uint8_t playerCount) {
        float contribution = myValue / totalValue;
        if (contribution <= (float)1 / playerCount) {
            return contribution * playerCount - 1;
        }
        else {
            return (contribution * playerCount - 1) / (playerCount - 1);
        }
    }
    float normalizedOthersHeuristicValue(float othersValue, float totalValue, uint8_t playerCount) {
        float othersContribution = othersValue / totalValue;
        float pcRatio = (float)playerCount / (playerCount - 1);
        if (othersContribution <= (float)(playerCount - 1) / playerCount) {
            return -othersContribution * pcRatio + 1;
        }
        else {
            return -(othersContribution * pcRatio - 1) / (pcRatio - 1);
        }
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

    void getScores(std::vector<float> &rawScores, std::vector<float> &weightedScores, uint8_t **board, float **valueMask) {
        rawScores.clear();
        weightedScores.clear();

        rawScores.resize(GameDetails::playerCount, 0);
        weightedScores.resize(GameDetails::playerCount, 0);

        for (uint8_t i = 0; i < GameDetails::boardHeight; i++) {
            for (uint8_t j = 0; j < GameDetails::boardWidth; j++) {

                uint8_t boardValue = board[i][j];
                if (boardValue < 1 || boardValue > GameDetails::playerCount) {
                    continue;
                }
                rawScores[boardValue-1]++;
                weightedScores[boardValue-1] += valueMask[i][j];
            }
        }
        return;
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

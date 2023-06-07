#include "mapPreprocessing.hpp"

#include "gamedetails.h"
#include "arrayops.h"

namespace MapPreprocessing
{
    uint8_t** createValueMask(uint8_t** board) {
        // Create mask with the same dimensions as the board
        uint8_t** mask = create2DArr<uint8_t>(GameDetails::boardHeight, GameDetails::boardWidth);

        // Iterate over the board elements
        for (int i = 0; i < GameDetails::boardHeight; i++) {
            for (int j = 0; j < GameDetails::boardWidth; j++) {
                // for all fields that are a valid place for a piece
                if (board[i][j] != 13) {
                    int NumberOfBoarderingEdges = 0;
                    //check all four directions to determin if it is an edge
                    //down
                    if (i == GameDetails::boardHeight - 1 || board[i+1][j] == 13)
                        NumberOfBoarderingEdges++;
                    //up
                    if (i == 0 || board[i-1][j] == 13)
                        NumberOfBoarderingEdges++;
                    //right
                    if (j == GameDetails::boardWidth - 1 || board[i][j+1] == 13)
                        NumberOfBoarderingEdges++;
                    //left
                    if (j == 0 || board[i][j-1] == 13)
                        NumberOfBoarderingEdges++;
                    
                    mask[i][j] = NumberOfBoarderingEdges * ValueEdge; //+1; for also counting pieces
                }
            }
        }
        return mask;
    }


    int evaluateEdges(uint8_t** board, uint8_t** mask, uint8_t playerNumber) {
        int evaluation = 0;
        //go over al fields and check if it is one of our pieces
        for (int i = 0; i < GameDetails::boardHeight; i++) {
            for (int j = 0; j < GameDetails::boardWidth; j++) {
                if (board[i][j] == playerNumber)
                    //look up value of square occupied by piece and add to evaluation
                    evaluation += mask[i][j];
            }
        }

        return evaluation;
    }
} // namespace MapPreprocessing

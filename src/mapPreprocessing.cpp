#include "mapPreprocessing.hpp"

#include "gamedetails.h"
#include "currentstate.h"
#include "arrayops.h"

#include <math.h>

#include "debugUtils.hpp"

namespace MapPreprocessing
{
    float **tileValueMask = nullptr;

    void createValueMask() {
        if (tileValueMask) {
            deleteValueMask();
        }
        float **mask = createRawValues();
        //DebugUtils::printArray(mask, GameDetails::boardHeight, GameDetails::boardWidth);
        //std::cout << std::endl;
        deductNeighbors(mask);
        tileValueMask = mask;

        return;
    }

    void deleteValueMask() {
        delete2DArr(tileValueMask, GameDetails::boardHeight);
        tileValueMask = nullptr;
        return;
    }

    float **createRawValues() {
        // Create mask with the same dimensions as the board
        float **mask = createZeroInit2DArr<float>(GameDetails::boardHeight, GameDetails::boardWidth);

        // Iterate over the board elements
        for (int i = 0; i < GameDetails::boardHeight; i++) {
            for (int j = 0; j < GameDetails::boardWidth; j++) {
                // for all fields that are a valid place for a piece
                if (CurrentState::boardArr[i][j] == 13) {
                    continue;
                }
                float tileValue = 1;

                uint8_t blockedTake;
                uint8_t blockedGetTaken;
                getBlockedAxes(blockedTake, blockedGetTaken, j, i);

                // TODO: balance
                if (blockedGetTaken >= 4) {
                    tileValue += 10;
                }
                else {
                    tileValue += blockedGetTaken;
                }
                tileValue -= (float)blockedTake / 8;

                mask[i][j] = tileValue;
            }
        }
        return mask;
    }

    void deductNeighbors(float **rawValueArray) {
        float **maskCopy = copy2DArr(rawValueArray, GameDetails::boardHeight, GameDetails::boardWidth);

        // Iterate over the board elements
        for (int i = 0; i < GameDetails::boardHeight; i++) {
            for (int j = 0; j < GameDetails::boardWidth; j++) {
                if (!CurrentState::mapNeighbors[i][j]) {
                    continue;
                }

                float maxNeighbor = std::numeric_limits<float>::lowest();
                // Iterate over the neighbors
                for (uint k = 0; k < 8; ++k) {
                    Neighbor n = CurrentState::mapNeighbors[i][j][k];
                    
                    // Skip invalid neighbors
                    if (n.pos > 7) {
                        continue;
                    }
                    
                    float value = maskCopy[n.y][n.x];
                    maxNeighbor = std::max(value, maxNeighbor);
                }
                rawValueArray[i][j] -= (maxNeighbor - 1) / 2;
            }
        }
        delete2DArr(maskCopy, GameDetails::boardHeight);
        return;
    }

    void getBlockedAxes(uint8_t &blockedTakeDirections, uint8_t &blockedGetTakenAxes, uint8_t x, uint8_t y) {
        blockedTakeDirections = 0;
        blockedGetTakenAxes = 0;

        if (!CurrentState::mapNeighbors[y][x]) {
            return;
        }

        bool axesBlocked[4] {false};
        for (int i = 0; i < 8; ++i) {
            Neighbor n = CurrentState::mapNeighbors[y][x][i];

            // Valid neighbors are not blockers, but...
            // It is still impossible to take pieces if there isn't at least a 2 wide gap in the direction
            if (n.pos <= 7) {
                Neighbor n2 = CurrentState::mapNeighbors[n.y][n.x][i];
                if (n2.pos > 7) {
                    ++blockedTakeDirections;
                }
                continue;
            }

            ++blockedTakeDirections;

            uint8_t axis = i % 4;
            if (!axesBlocked[axis]) {
                axesBlocked[axis] = true;
                ++blockedGetTakenAxes;
            }
        }
        return;
    }
} // namespace MapPreprocessing

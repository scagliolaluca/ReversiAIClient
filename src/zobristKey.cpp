#include "zobristKey.h"


#include "gamedetails.h"
#include "currentstate.h"
#include "arrayops.h"
#include "debugUtils.hpp"
#include <bitset> //can be deletet only for debugging

namespace ZobristKey
{
    uint32_t ***zobristKeyMask = nullptr;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<uint32_t> distrib(0, LONG_MAX);

    void createZobristKeyMask() {
    std::cout << "Create Zobrist Key Mask..." << std::endl;
        if (zobristKeyMask) {
            deleteZobristKeyMask();
        }
        zobristKeyMask = createZobristKeyValues();
    }

    void deleteZobristKeyMask() {
    std::cout << "Delete Zobrist Key Maks" << std::endl;
        delete3DArr(zobristKeyMask, GameDetails::boardHeight, GameDetails::boardWidth);
        zobristKeyMask = nullptr;
        return;
    }

    uint32_t ***createZobristKeyValues() {
        // Create mask with the same dimensions as the board
        uint32_t ***mask = createZeroInit3DArr<uint32_t>(GameDetails::boardHeight, GameDetails::boardWidth, GameDetails::playerCount);

        // Iterate over the board elements
        for (int i = 0; i < GameDetails::boardHeight; i++) {
            for (int j = 0; j < GameDetails::boardWidth; j++) {
                // for all fields that are a valid place for a piece
                if (CurrentState::boardArr[i][j] == 13) {
                    continue;
                }
                uint32_t *arr = new uint32_t[]{distrib(rng), distrib(rng)};
                mask[i][j] = arr;
            }
        }
        return mask;
    }

    uint32_t generateZobristValue(uint8_t **board) {
        std::cout << "Create a new ZobristHash" << std::endl;
        uint32_t h = 0;
        for (int i = 0; i < GameDetails::boardHeight; i++) {
            for (int j = 0; j < GameDetails::boardWidth; j++) {
                // for all fields that have a piece
                if (board[i][j] > 0 and board[i][j] <= GameDetails::playerCount) {
                    //std::cout << std::bitset<32>(h) << "\t" << h << std::endl;
                    //std::cout << std::bitset<32>(zobristKeyMask[i][j][board[i][j]-1]) << "\t" << zobristKeyMask[i][j][board[i][j]-1] << std::endl;
                    h = h ^ zobristKeyMask[i][j][board[i][j]-1];
                    std::cout << std::bitset<32>(h) << "\t" << h << "\t" << &h << std::endl;
                    //std::cout << std::endl;
                }
            }
        }
        return h;
    }


} // namespace ZobristKey
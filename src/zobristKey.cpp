#include "zobristKey.h"


#include "gamedetails.h"
#include "currentstate.h"
#include "arrayops.h"
#include "debugUtils.hpp"
#include <bitset> //can be deletet only for debugging

namespace ZobristKey
{
    uint32_t ***zobristKeyMask = nullptr;
    uint32_t *whosTurnMask = nullptr;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<uint32_t> distrib(0, UINT32_MAX);

    void createZobristKeyMask() {
        std::cout << "Create Zobrist Key Mask..." << std::endl;
        if (zobristKeyMask) {
            deleteZobristKeyMask();
        }
        zobristKeyMask = createZobristKeyValues();
        return;
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
                for (int k = 0; k < GameDetails::playerCount; k++) {
                    mask[i][j][k] = distrib(rng);
                }
            }
        }
        return mask;
    }

    void createWhosTurnMask() {
        std::cout << "Create Zobrist Key for players..." << std::endl;
        if (whosTurnMask) {
            deleteWhosTurnMask();
        }
        whosTurnMask = createWhosTurnMaskValues();
        return;
    }

    void deleteWhosTurnMask() {
        std::cout << "Delete Zobrist Key for players..." << std::endl;
        delete[] zobristKeyMask;
        zobristKeyMask = nullptr;
        return;
    }

    uint32_t *createWhosTurnMaskValues() {
        uint32_t *mask = new uint32_t[GameDetails::playerCount]();
        for (int i = 0; i < GameDetails::playerCount; i++) {
            mask[i] = distrib(rng);
        }
        return mask;
    }

    uint32_t generateZobristValue(uint8_t **board, uint8_t player) {
        uint32_t h = 0;
        h = h ^ whosTurnMask[player-1];
        for (int i = 0; i < GameDetails::boardHeight; i++) {
            for (int j = 0; j < GameDetails::boardWidth; j++) {
                // for all fields that have a piece
                if (board[i][j] > 0 and board[i][j] <= GameDetails::playerCount) {
                   h = h ^ zobristKeyMask[i][j][board[i][j]-1];
                }
            }
        }
        return h;
    }

    void xorInOut(uint32_t& h, uint8_t y, uint8_t x, uint8_t xorOut, uint8_t xorIn) {
        // if xorOut == 0, this field previously wasn't occupied, so no XORout
        if (xorOut != 0) {
            // XORout the old value by reXORing it in
            h = h ^ zobristKeyMask[y][x][xorOut - 1];
        }
        // XORin the new value by XORing it in
        h = h ^ zobristKeyMask[y][x][xorIn-1];
    }

    void xorInOutPlayer(uint32_t& h, uint8_t xorOut, uint8_t xorIn) {
        h = h ^ whosTurnMask[xorOut-1];
        h = h ^ whosTurnMask[xorIn-1];
    }

} // namespace ZobristKey

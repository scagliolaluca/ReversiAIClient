#ifndef ZOBRISTKEY_H
#define ZOBRISTKEY_H

#include <random>
#include <cstdint>
#include <climits>
#include <iostream>

namespace ZobristKey
{
    extern uint32_t ***zobristKeyMask;
    void createZobristKeyMask();
    void deleteZobristKeyMask();
    uint32_t ***createZobristKeyValues();
    uint32_t generateZobristValue(uint8_t **board);

    template <class T>
    T*** createZeroInit3DArr(size_t sizeFirst, size_t sizeSecond, size_t sizeThird) {

        T*** newArr = new T**[sizeFirst];

        for (size_t i = 0; i < sizeFirst; i++) {
            newArr[i] = new T*[sizeSecond];

            for (size_t j = 0; j < sizeSecond; j++) {
                newArr[i][j] = new T[sizeThird]();
            }
        }

        return newArr;
    }

    template <class T>
    void delete3DArr(T*** arr, size_t sizeFirst, size_t sizeSecond) {
        if (!arr) {
            return;
        }

        for (size_t i = 0; i < sizeFirst; i++) {
            for (size_t j = 0; j < sizeSecond; j++) {
                delete[] arr[i][j];
            }
            delete[] arr[i];
        }

        delete[] arr;
        arr = nullptr;
    }
} // namespace ZobristKey
#endif // ZOBRISTKEY_H

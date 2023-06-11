#pragma once

#include <cstdlib>
#include <iostream>
#include <iomanip>

namespace DebugUtils
{
    template <class T>
    void printArray(T **array, size_t sizeFirst, size_t sizeSecond) {
        for (size_t i = 0; i < sizeFirst; ++i) {
            for (size_t j = 0; j < sizeSecond; ++j) {
                std::cout << std::setw(5) << std::setfill(' ') << std::setprecision(2) << std::showpoint << array[i][j] << " ";
            }
            std::cout << std::setw(0) << std::endl;
        }
        return;
    }
} // namespace DebugUtils

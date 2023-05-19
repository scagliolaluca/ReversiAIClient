#ifndef MOVE_H
#define MOVE_H

#include <cstdint>

struct Move {
    uint8_t x;
    uint8_t y;
    Move(uint8_t x, uint8_t y);
};

#endif  // MOVE_H

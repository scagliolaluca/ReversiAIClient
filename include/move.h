#ifndef MOVE_H
#define MOVE_H

#include <cstdint>

struct Move {
    uint8_t x;
    uint8_t y;
    Move(uint8_t x, uint8_t y);
};

bool operator==(const Move& a, const Move& b);

#endif  // MOVE_H

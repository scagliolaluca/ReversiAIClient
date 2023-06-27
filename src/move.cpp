#include "move.h"

Move::Move(uint8_t x, uint8_t y) : x(x), y(y) {}

bool operator==(const Move& a, const Move& b) {
    return a.x == b.x && a.y == b.y;
}
#ifndef HEURISTICS_H
#define HEURISTICS_H

#include <cstdint>
#include <iostream>
#include <vector>

#include "currentstate.h"

namespace Heuristics {

int getScore(uint8_t**, uint8_t);
int getMovecount(uint8_t**, uint8_t);
void copyBoard(uint8_t**, uint8_t**);
void printBoard(uint8_t**);
}
#endif //HEURISTICS_H

#ifndef NEIGHBOR_H
#define NEIGHBOR_H

#include <cstdint>

#include "logging.h"

struct Neighbor {
    uint8_t x, y;
    uint8_t pos;

    /** @brief constucts a Neighbor object with pos(-1)
     */
    Neighbor();

    /** @brief constucts a Neighbor object with given parameters
     * @param x x-coordinate of the Neighbor
     * @param y y-coordinate of the Neighbor
     * @param pos position of the Neighbor ([0-7] if the neighbor exists at given position or -1 if there is no neighbor)
     */
    Neighbor(uint8_t x, uint8_t y, uint8_t pos);
};

#endif  // NEIGHBOR_H

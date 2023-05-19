#ifndef PLAYER_H
#define PLAYER_H

#include <cstdint>

#include "gamedetails.h"

struct Player {
    bool isDisqualified;
    /** @brief constucts a Player object with the values provided in GameDetails
     */
    Player();
};

#endif  // PLAYER_H

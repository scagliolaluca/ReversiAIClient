#include "players.h"

uint8_t Players::getActivePlayerCount() {
    uint8_t count = 0;
    for (uint8_t i = 0; i < GameDetails::playerCount; i++) {
        if (!CurrentState::playerArr[i].isDisqualified) {
            count++;
        }
    }
    return count;
}
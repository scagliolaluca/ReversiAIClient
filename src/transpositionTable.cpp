#include "transpositionTable.h"

#include <iostream>

namespace TranspositionTable
{
    const uint16_t size = UINT_MAX;
    Entry entries[size];

    // Check if Transposition Table has entry with same gamestate
    bool alreadySeen(uint32_t *zobrist){
        Entry entry = entries[*zobrist%size];
        return entry.zobristkey == zobrist;
    }

    // Add entry to the Transposition Table
    bool addEntry(uint32_t *zobrist, uint8_t x, uint8_t y, uint8_t depth, float value){
        Entry entry = entries[*zobrist%size];

        // Check if gamestates are equal
        if (entry.zobristkey == zobrist){
            std::cout << "This gamestat was reached already! " << std::endl;
            // Check if our gamestate is achieved at lower depth
            if (depth < entry.depth){
                std::cout << "But ours is at lower depth (better)" << std::endl;
                entry.depth = depth;
            }
        }

        // Clash or empty slot
        else {
            std::cout << "Clash or empty slot" << std::endl;
            if (depth < entry.depth){
                std::cout << "Overwrite, because ours is at lower depth" << std::endl;
                entry.depth = depth;
                entry.zobristkey = zobrist;
                entry.x = x;
                entry.y = y;
                entry.value = value;
            }
        }
    }

} // namespace TranspositionTable

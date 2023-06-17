#include "transpositionTable.h"

#include <iostream>

namespace TranspositionTable
{
    const uint16_t size = UINT_MAX;
    Entry entries[size];

    // Check if Transposition Table has entry with same gamestate
    bool alreadySeen(uint32_t zobrist){
        Entry entry = entries[zobrist%size];
        if(entry.zobristkey){
            return *entry.zobristkey == zobrist;
        }
        return false;
    }

    // Add entry to the Transposition Table
    void addEntry(uint32_t zobrist, uint8_t x, uint8_t y, uint8_t depth, float value){
        Entry entry = entries[zobrist%size];
        std::cout << "\n\nAdd new Entry with values: " << zobrist << "\tXY: " << int(x) << int(y) << "\tDepth: " << int(depth) << "\tValue: " << value << std::endl;
        // if zobristkey isn't nullptr (set in the past)
        if (entry.zobristkey) {
            std::cout << "This field in TT is empty" << std::endl;
            // if keys are the same
            if (*entry.zobristkey == zobrist) {
                std::cout << "This gamestat was reached already! " << std::endl;
                // Check if our gamestate is achieved at lower depth
                if (depth < entry.depth) {
                    std::cout << "But ours is at lower depth (better) --> update" << std::endl;
                    entry.depth = depth;
                    entry.value = value;
                    entry.x = x;
                    entry.y = y;
                }
            }
            // clash with older entry --> TODO: Add some logic here
            else {
                std::cout << "!!!Clash!!! --- This entry was set previously! " << std::endl;
                if (depth < entry.depth) {
                    std::cout << "Overwrite, because ours is at lower depth" << std::endl;
                    entry.depth = depth;
                    entry.zobristkey = new uint32_t(zobrist);
                    entry.value = value;
                    entry.x = x;
                    entry.y = y;
                }
            }
        }

        // empty slot
        else {
            std::cout << "Empty slot" << std::endl;
            entry.depth = depth;
            entry.zobristkey = new uint32_t(zobrist);
            entry.value = value;
            entry.x = x;
            entry.y = y;
        }

        return;
    }

    // Add Leave entry to the Transposition Table (Leave has only its value from Heuristik, no next moves
    void addLeaveEntry(uint32_t zobrist, uint8_t depth, float value){
        std::cout << zobrist << " " << &zobrist << std::endl;
        Entry entry = entries[zobrist%size];
        std::cout << "\n\nAdd new Leave Entry with values: " << zobrist << "\tDepth: " << int(depth) << "\tValue: " << value << std::endl;

        // if zobristkey isn't nullptr (set in the past)
        if (entry.zobristkey){
            std::cout << "This field in TT is empty" << std::endl;
            // if keys are the same
            if (*entry.zobristkey == zobrist){
                std::cout << "This gamestat was reached already! " << std::endl;
                // Check if our gamestate is achieved at lower depth
                if (depth < entry.depth){
                    std::cout << "But ours is at lower depth (better) --> update" << std::endl;
                    entry.depth = depth;
                    entry.value = value;
                }
            }
            // clash with older entry --> TODO: Add some logic here
            else {
                std::cout << "!!!Clash!!! --- This entry was set previously to other gamestate! " << std::endl;
                if (depth < entry.depth){
                    std::cout << "Overwrite, because ours is at lower depth" << std::endl;
                    entry.depth = depth;
                    *entry.zobristkey = zobrist;
                    entry.value = value;
                }
            }
        }

        // empty slot
        else {
            std::cout << "Empty slot" << std::endl;
            entry.depth = depth;
            entry.zobristkey = new uint32_t(zobrist);
            std::cout << "New set Entry: " << entry.zobristkey << " " << *entry.zobristkey << std::endl;
            entry.value = value;
        }

        return;
    }

} // namespace TranspositionTable

#include "transpositionTable.h"

#include <iostream>

namespace TranspositionTable
{
    const uint16_t size = UINT16_MAX;
    Entry *entries = nullptr;

    void createTranspositionTable() {
        std::cout << "Create Transposition Table...";
        if (entries) {
            deleteTranspositionTable();
        }
        entries = new Entry[size];
        std::cout << entries << std::endl;
    }

    void deleteTranspositionTable() {
        std::cout << "Delete Transposition Table..." << std::endl;
        delete[] entries;
        entries = nullptr;
    }

    // Check if Transposition Table has entry with same gamestate
    bool alreadySeen(uint32_t zobrist){
        Entry entry = entries[zobrist%size];
        if(entry.zobristkey){
            return *entry.zobristkey == zobrist;
        }
        return false;
    }

    // Add entry to the Transposition Table
    void addEntry(uint32_t zobrist, uint8_t x, uint8_t y, uint8_t depth, uint8_t calculatedDepth, float value){
        Entry *entry = &entries[zobrist%size];

        // if zobristkey isn't nullptr (was set in the past) --> field is occupied
        if (entry->zobristkey){
            // if keys are the same --> gamestate was reached previously
            if (*entry->zobristkey == zobrist){
                // if new gamestate searched deeper
                if (calculatedDepth - depth > entry->calculatedDepth - entry->depth){
                    entry->depth = depth;
                    entry->calculatedDepth = calculatedDepth;
                    entry->value = value;
                    entry->x = x;
                    entry->y = y;
                }
            }
            // clash with older entry --> TODO: Add some better logic here
            else {
                if (depth < entry->depth){
                    //std::cout << "Overwrite, because ours is at lower depth" << std::endl;
                    entry->depth = depth;
                    entry->calculatedDepth = calculatedDepth;
                    *entry->zobristkey = zobrist;
                    entry->value = value;
                    entry->x = x;
                    entry->y = y;
                }
            }
        }

        // empty slot
        else {
            entry->depth = depth;
            entry->calculatedDepth = calculatedDepth;
            entry->zobristkey = new uint32_t(zobrist);
            entry->value = value;
            entry->x = x;
            entry->y = y;
        }
        return;
    }

    // Add Leave entry to the Transposition Table (Leave has only its value from Heuristik, no next moves
    void addLeaveEntry(uint32_t zobrist, uint8_t depth, uint8_t calculatedDepth, float value){
        Entry *entry = &entries[zobrist%size];

        // if zobristkey isn't nullptr (was set in the past) --> field is occupied
        if (entry->zobristkey){
            // if keys are the same --> newEntry searched exactely 0, and old searched > 0, so no update possible

            // clash with older entry --> TODO: Add some better logic here
            if (*entry->zobristkey != zobrist) {
                //std::cout << "!!!Clash!!! --- This entry was set previously to other gamestate! " << std::endl;
                if (depth < entry->depth){
                    entry->depth = depth;
                    *entry->zobristkey = zobrist;
                    entry->value = value;
                    entry->calculatedDepth = calculatedDepth;
                }
            }
        }
        // empty slot
        else {
            entry->depth = depth;
            entry->zobristkey = new uint32_t(zobrist);
            entry->value = value;
            entry->calculatedDepth = calculatedDepth;
        }

        return;
    }

    Entry* getEntry(uint32_t zobrist){
        Entry *entry = &entries[zobrist%size];
        return entry;
    }

} // namespace TranspositionTable

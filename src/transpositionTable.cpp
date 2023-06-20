#include "transpositionTable.h"

#include <iostream>

namespace TranspositionTable
{
    const uint16_t size = UINT_MAX;
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
        //std::cout << zobrist%size << std::endl;
        if(entry.zobristkey){
            //std::cout << "Entry.zobristkey is no nullptr" << std::endl;
            //std::cout << "Other values: " << entry.value << int(entry.depth) << std::endl;
            return *entry.zobristkey == zobrist;
        }
        //std::cout << "Entry.zobristkey is nullptr" << std::endl;
        //std::cout << "Other values: " << entry.value << int(entry.depth) << std::endl;
        return false;
    }

    // Add entry to the Transposition Table
    void addEntry(uint32_t zobrist, uint8_t x, uint8_t y, uint8_t depth, uint8_t calculatedDepth, float value){
        Entry *entry = &entries[zobrist%size];
        //std::cout << "\n\nAdd new Entry with values: " << zobrist << "\tDepth: " << int(depth) << "\tValue: " << value << std::endl;
        //std::cout << "To existing Node: " << entry << " with values " << entry->zobristkey << std::endl;

        // if zobristkey isn't nullptr (set in the past)
        if (entry->zobristkey){
            //std::cout << "This field in TT is occupied" << std::endl;
            // if keys are the same
            if (*entry->zobristkey == zobrist){
                //std::cout << "This gamestate was reached already! " << std::endl;
                // Check if new gamestate searched deeper //TODO: Check at which level this node is
                if (calculatedDepth > entry->calculatedDepth){
                    //std::cout << "But new result goes deeper (better) --> update" << std::endl;
                    entry->depth = depth;
                    entry->calculatedDepth = calculatedDepth;
                    entry->value = value;
                    entry->x = x;
                    entry->y = y;
                }
            }
                // clash with older entry --> TODO: Add some logic here
            else {
                //std::cout << "!!!Clash!!! --- This entry was set previously to other gamestate! " << std::endl;
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
            //std::cout << "Empty slot" << std::endl;
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
        //std::cout << "\n\nAdd new Leave Entry with values: " << zobrist << "\tDepth: " << int(depth) << "\tValue: " << value << std::endl;
        //std::cout << "To existing Node: " << entry << " with value " << entry->zobristkey << std::endl;

        // if zobristkey isn't nullptr (set in the past)
        if (entry->zobristkey){
            //std::cout << "This field in TT is occupied" << std::endl;
            // if keys are the same
            if (*entry->zobristkey == zobrist){
                //std::cout << "This gamestate was reached already! " << std::endl;
                // Check if new gamestate searched deeper //TODO: Check at which level this node is
                if (calculatedDepth > entry->calculatedDepth){
                    //std::cout << "But new result goes deeper (better) --> update" << std::endl;
                    entry->depth = depth;
                    entry->value = value;
                    entry->calculatedDepth = calculatedDepth;
                }
            }
            // clash with older entry --> TODO: Add some logic here
            else {
                //std::cout << "!!!Clash!!! --- This entry was set previously to other gamestate! " << std::endl;
                if (depth < entry->depth){
                    //std::cout << "Overwrite, because ours is at lower depth" << std::endl;
                    entry->depth = depth;
                    *entry->zobristkey = zobrist;
                    entry->value = value;
                    entry->calculatedDepth = calculatedDepth;
                }
            }
        }

        // empty slot
        else {
            //std::cout << "Empty slot" << std::endl;
            entry->depth = depth;
            entry->zobristkey = new uint32_t(zobrist);
            entry->value = value;
            entry->calculatedDepth = calculatedDepth;
        }

        return;
    }

    Entry getEntry(uint32_t zobrist){
        Entry entry = entries[zobrist%size];
        return entry;
    }

} // namespace TranspositionTable

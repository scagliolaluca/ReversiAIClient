#ifndef TRANSPOSITIONTABLE_H
#define TRANSPOSITIONTABLE_H

#include <cstdint>
#include <climits>


namespace TranspositionTable
{
    class Entry {
    public:
        uint32_t *zobristkey = nullptr;
        uint8_t x, y;
        float value;
        uint8_t depth = UINT8_MAX;
        uint8_t calculatedDepth;
    };

    extern Entry *entries;
    extern const uint16_t size;
    void createTranspositionTable();
    void deleteTranspositionTable();
    bool alreadySeen(uint32_t zobrist);
    void addEntry(uint32_t zobrist, uint8_t x, uint8_t y, uint8_t depth, uint8_t calculatedDepth, float value);
    void addLeaveEntry(uint32_t zobrist, uint8_t depth, uint8_t calculatedDepth, float value);
    Entry* getEntry(uint32_t zobrist);

} // namespace TranspositionTable

#endif // TRANSPOSITIONTABLE_H


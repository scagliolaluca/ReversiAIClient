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
        uint8_t depth = UCHAR_MAX;
    };

    bool alreadySeen(uint32_t zobrist);
    void addEntry(uint32_t zobrist, uint8_t x, uint8_t y, uint8_t depth, float value);
    void addLeaveEntry(uint32_t zobrist, uint8_t depth, float value);

} // namespace TranspositionTable

#endif // TRANSPOSITIONTABLE_H


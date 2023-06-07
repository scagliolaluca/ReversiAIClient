#include <cstdint>

namespace MapPreprocessing
{
    const uint8_t ValueEdge = 2;

    uint8_t** createValueMask(uint8_t** board);
    int evaluateEdges(uint8_t** board, uint8_t** mask, uint8_t playerNumber);
} // namespace MapPreprocessing

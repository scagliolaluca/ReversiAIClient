#include "gamedetails.h"

namespace GameDetails {
uint8_t playerCount = 0;
uint8_t playerNumber = 1;

uint8_t boardWidth = 0;
uint8_t boardHeight = 0;
};  // namespace GameDetails

std::map<char, uint8_t> codeLookup = {
    {'0', 0},
    {'1', 1},
    {'2', 2},
    {'3', 3},
    {'4', 4},
    {'5', 5},
    {'6', 6},
    {'7', 7},
    {'8', 8},
    {'-', 13},
};

std::map<uint8_t, char> asciiLookup = {
    {0, '0'},
    {1, '1'},
    {2, '2'},
    {3, '3'},
    {4, '4'},
    {5, '5'},
    {6, '6'},
    {7, '7'},
    {8, '8'},
    {13, '-'},
};

uint8_t oppositeDirectionLookup[8] = {4, 5, 6, 7, 0, 1, 2, 3};

bool GameDetails::setPlayerCount(std::string& line) {
    if (!(line.length())) {
        // empty string
        return false;
    }

    uint8_t playerCount = line.at(0) - '0';

    if (playerCount > 8) {
        return false;
    }

    GameDetails::playerCount = playerCount;
    return true;
}

bool GameDetails::setBoardDimensions(std::string& line) {
    uint16_t height = 0;
    uint16_t width = 0;
    std::stringstream buffer;
    buffer << line;
    std::getline(buffer, line, ' ');
    for (int i = 0; i < int(line.length()); i++) {
        uint8_t c = line.at(i) - '0';
        if (c > 9) {
            return false;
        }
        height = height * 10 + c;
        if (height > 50) {
            return false;
        }
    }

    std::getline(buffer, line);
    for (int i = 0; i < int(line.length()); i++) {
        uint8_t c = line.at(i);
        if ((c == '\r') && (i == int(line.length()) - 1)) {
            break;
        }
        c = c - '0';
        if (c > 9) {
            return false;
        }
        width = width * 10 + c;
        if (width > 50) {
            return false;
        }
    }

    GameDetails::boardHeight = height;
    GameDetails::boardWidth = width;

    return true;
}

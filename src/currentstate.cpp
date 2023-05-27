#include "currentstate.h"

namespace CurrentState {
uint32_t timelimit = 0;
uint8_t searchDepth = 0;
uint8_t activePlayerCount = 0;

uint16_t moveCount = 0;

uint8_t** boardArr = nullptr;
Neighbor*** mapNeighbors = nullptr;
Player* playerArr = nullptr;
}  // namespace CurrentState

void CurrentState::createBoardArr() {
    if (CurrentState::boardArr) {
        deleteBoardArr();
    }

    CurrentState::boardArr = create2DArr<uint8_t>(GameDetails::boardHeight, GameDetails::boardWidth);
}

void CurrentState::deleteBoardArr() {
    delete2DArr(CurrentState::boardArr, GameDetails::boardHeight);
    CurrentState::boardArr = nullptr;
}

bool CurrentState::populateBoardArr(std::stringstream& boardStream) {
    std::string line;
    std::string tile;
    std::stringstream lineStream;
    for (uint8_t i = 0; i < GameDetails::boardHeight; i++) {
        std::getline(boardStream, line);
        lineStream.clear();
        lineStream << line;
        for (uint8_t j = 0; j < GameDetails::boardWidth; j++) {
            if (lineStream.eof()) {
                return false;
            }
            std::getline(lineStream, tile, ' ');
            if (tile.length() > 1) {
                if (tile.length() > 2) {
                    return false;
                }
                if (tile.at(1) != '\r') {
                    return false;
                }
            }
            // map character to integer for faster equasion
            std::map<char, uint8_t>::const_iterator it = codeLookup.find(tile.at(0));
            if (it == codeLookup.cend()) {
                return false;
            }
            uint8_t c = it->second;

            CurrentState::boardArr[i][j] = c;
        }
    }
    return true;
}

void CurrentState::populatePlayers() {
    if (CurrentState::playerArr) {
        delete[] CurrentState::playerArr;
    }
    CurrentState::playerArr = new Player[GameDetails::playerCount];
}

void CurrentState::createMapNeighbors() {
    if (CurrentState::mapNeighbors) {
        CurrentState::deleteMapNeighbors();
    }

    CurrentState::mapNeighbors = new Neighbor**[GameDetails::boardHeight];

    for (uint8_t i = 0; i < GameDetails::boardHeight; i++) {
        CurrentState::mapNeighbors[i] = new Neighbor*[GameDetails::boardWidth];
        for (uint8_t j = 0; j < GameDetails::boardWidth; j++) {
            CurrentState::mapNeighbors[i][j] = nullptr;
        }
    }
}

void CurrentState::deleteMapNeighbors() {
    if (!CurrentState::mapNeighbors) {
        return;
    }

    for (uint8_t i = 0; i < GameDetails::boardHeight; i++) {
        if (!CurrentState::mapNeighbors[i]) {
            continue;
        }
        for (uint8_t j = 0; j < GameDetails::boardWidth; j++) {
            if (!CurrentState::mapNeighbors[i][j]) {
                continue;
            }
            delete[] CurrentState::mapNeighbors[i][j];
        }

        delete[] CurrentState::mapNeighbors[i];
    }

    delete[] CurrentState::mapNeighbors;
    CurrentState::mapNeighbors = nullptr;
}

bool CurrentState::populateNeighbors() {
    Neighbor* n;
    for (uint8_t i = 0; i < GameDetails::boardHeight; i++) {
        for (uint8_t j = 0; j < GameDetails::boardWidth; j++) {
            // if the tile is set to '-' do not create neighbors for this tile
            if (CurrentState::boardArr[i][j] == 13) {
                continue;
            }
            n = new Neighbor[8];
            // set natural neighbors if it is not a border and neighbor is not '-'
            // otherwise leave Neighbor at default
            if (i) {
                if (CurrentState::boardArr[i - 1][j] != 13) {
                    n[0] = Neighbor(j, uint8_t(i - 1), 4);
                }
            }
            if (i && (j < GameDetails::boardWidth - 1)) {
                if (CurrentState::boardArr[i - 1][j + 1] != 13) {
                    n[1] = Neighbor(uint8_t(j + 1), uint8_t(i - 1), 5);
                }
            }
            if (j < GameDetails::boardWidth - 1) {
                if (CurrentState::boardArr[i][j + 1] != 13) {
                    n[2] = Neighbor(uint8_t(j + 1), i, 6);
                }
            }
            if ((i < GameDetails::boardHeight - 1) && (j < GameDetails::boardWidth - 1)) {
                if (CurrentState::boardArr[i + 1][j + 1] != 13) {
                    n[3] = Neighbor(uint8_t(j + 1), uint8_t(i + 1), 7);
                }
            }
            if (i < GameDetails::boardHeight - 1) {
                if (CurrentState::boardArr[i + 1][j] != 13) {
                    n[4] = Neighbor(j, uint8_t(i + 1), 0);
                }
            }
            if ((i < GameDetails::boardHeight - 1) && j) {
                if (CurrentState::boardArr[i + 1][j - 1] != 13) {
                    n[5] = Neighbor(uint8_t(j - 1), uint8_t(i + 1), 1);
                }
            }
            if (j) {
                if (CurrentState::boardArr[i][j - 1] != 13) {
                    n[6] = Neighbor(uint8_t(j - 1), i, 2);
                }
            }
            if (i && j) {
                if (CurrentState::boardArr[i - 1][j - 1] != 13) {
                    n[7] = Neighbor(uint8_t(j - 1), uint8_t(i - 1), 3);
                }
            }

            CurrentState::mapNeighbors[i][j] = n;
        }
    }
    return true;
}

void CurrentState::printBoardArr() {
    for (uint8_t i = 0; i < GameDetails::boardHeight; i++) {
        for (size_t j = 0; j < GameDetails::boardWidth; j++) {
            char v = CurrentState::boardArr[i][j];
            std::map<uint8_t, char>::const_iterator it = asciiLookup.find(v);
            if (it == asciiLookup.cend()) {
                std::cerr << "Lookup Error occurred while printing boardArr...\n";
                return;
            }
            char c = it->second;
            std::cout << c << " ";
        }
        std::cout << '\n';  // end line after each row
    }
}

#include "map.h"

#include "mapPreprocessing.hpp"

#ifdef LOAD_LOGGING
#include "debugUtils.hpp"
#include "gamedetails.h"
#endif

bool Map::loadMap(std::string path) {
    std::stringstream buffer;

    std::ifstream mapFile(path);
    if (!mapFile.is_open()) {
        return false;
    }

    buffer << mapFile.rdbuf();
    mapFile.close();
    return loadMap(buffer);
}

bool Map::loadMap(std::stringstream& mapStream) {
    std::string line;
    std::getline(mapStream, line);

#ifdef LOAD_LOGGING
    std::cout << "Set player count...\n";
#endif
    if (!GameDetails::setPlayerCount(line)) {
        return false;
    }
    CurrentState::activePlayerCount = GameDetails::playerCount;
#ifdef LOAD_LOGGING
    std::cout << "Player count: " << int(GameDetails::playerCount) << "\n\nPopulate players...\n";
#endif
    // override stones not implemented in basic client
    std::getline(mapStream, line);
    // bombs not implemented in basic client
    std::getline(mapStream, line);

    CurrentState::populatePlayers();

#ifdef LOAD_LOGGING
    std::cout << "Player array successfully populated."
              << "\n\nSet board dimensions...\n";
#endif
    std::getline(mapStream, line);
    if (!GameDetails::setBoardDimensions(line)) {
        return false;
    }
#ifdef LOAD_LOGGING
    std::cout << "Board width: " << int(GameDetails::boardWidth) << " | Board height: " << int(GameDetails::boardHeight) << "\n\nCreate board array...\n";
#endif
    CurrentState::createBoardArr();
#ifdef LOAD_LOGGING
    std::cout
        << "Board array successfully created."
        << "\n\nPopulate board array...\n";
#endif
    std::stringstream boardStream;
    for (int i = 0; i < GameDetails::boardHeight; i++) {
        if (mapStream.eof()) {
            return false;
        }
        std::getline(mapStream, line);
        boardStream << line << '\n';
    }
    if (!CurrentState::populateBoardArr(boardStream)) {
        return false;
    }

#ifdef LOAD_LOGGING
    std::cout << "Board array successfully populated\n";
    CurrentState::printBoardArr();
    std::cout << "\nCreate map neighbors...\n";
#endif

    CurrentState::createMapNeighbors();

#ifdef LOAD_LOGGING
    std::cout << "Map neighbors successfully created."
              << "\n\nPopulate map neighbors...\n";
#endif

    if (!CurrentState::populateNeighbors()) {
        return false;
    }

#ifdef LOAD_LOGGING
    std::cout << "Map neighbors successfully populated\n";
    std::cout << "\nCreate value mask...\n";
#endif

    MapPreprocessing::createValueMask();

#ifdef LOAD_LOGGING
    std::cout << "Value mask created:\n";
    DebugUtils::printArray(MapPreprocessing::tileValueMask, GameDetails::boardHeight, GameDetails::boardWidth);
#endif

    return true;
}

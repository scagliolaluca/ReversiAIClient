#include "moves.h"

void Moves::getRandomMove(uint8_t& x, uint8_t& y, uint8_t** board, uint8_t playerNumber) {
    std::vector<Move> validMoves;

    Moves::populateValidMoves(validMoves, board, playerNumber);
    if (!validMoves.empty()) {
        Move m = validMoves.at(std::rand() % validMoves.size());
        
        x = m.x;
        y = m.y;
    }
}

bool Moves::isValidMove(uint8_t** board, uint8_t x, uint8_t y, uint8_t playerNumber) {
    // check if x, y are out of bounds
    if ((x >= GameDetails::boardWidth) || (y >= GameDetails::boardHeight)) {
        return false;
    }
    // check if tile is "-"
    if (board[y][x] != 0) {
        return false;
    }

    // for each direction check if the move is valid
    for (uint8_t i = 0; i < 8; i++) {
        int valid = Moves::validateDirection(board, x, y, playerNumber, i);
        if (valid < 0) {
            return false;
        } else if (valid > 0) {
            return true;
        }
    }
    // no valid direction found
    return false;
}

int Moves::validateDirection(uint8_t** board, uint8_t x, uint8_t y, uint8_t playerNumber, uint8_t direction) {
    // no neighbors found
    if (!CurrentState::mapNeighbors[y][x]) {
        return -1;
    }
    // get first neighbor in direction i
    Neighbor n = CurrentState::mapNeighbors[y][x][direction];
    // check if no neighbor in direction
    if (n.pos > 7) {
        return 0;
    }

    // first neighbor stone must not be a stone from given player (playerNumber)
    // if value in boardArr is not an enemy stone or a dead stone (1-9) -> direction is invalid -> try next direction
    if ((board[n.y][n.x] == playerNumber) || !((board[n.y][n.x] >= 1) && (board[n.y][n.x] <= 9))) {
        return 0;
    }
    // if first neighbor ok -> go to next neighbor
    while (true) {
        // should not occure (at least the tile you came from should be a neighbor)
        if (!CurrentState::mapNeighbors[n.y][n.x]) {
            return 0;
        }
        // get next Neighbor in the opposite direction you came from
        n = CurrentState::mapNeighbors[n.y][n.x][oppositeDirectionLookup[n.pos]];
        // check if no neighbor in direction
        if (n.pos > 7) {
            return 0;
        }

        // recolor in a circle is not allowed
        if ((n.x == x) && (n.y == y)) {
            return 0;
        }
        // if value in boardArr is playerNumber -> direction is valid and therefore its a valid move
        if (board[n.y][n.x] == playerNumber) {
            return 1;
        }
        // if value in boardArr is not an enemy stone (1-8) -> direction is invalid -> try next direction
        if (!((board[n.y][n.x] >= 1) && (board[n.y][n.x] <= 8))) {
            return 0;
        }
    }
    return 0;
}

void Moves::populateValidMoves(std::vector<Move>& validMoves, uint8_t** board, uint8_t playerNumber) {
    validMoves.clear();

    for (uint8_t y = 0; y < GameDetails::boardHeight; y++) {
        for (uint8_t x = 0; x < GameDetails::boardWidth; x++) {
            if (Moves::isValidMove(board, x, y, playerNumber)) {
                validMoves.push_back(Move(x, y));
            }
        }
    }
}

void Moves::makeMove(uint8_t** board, uint8_t x, uint8_t y, uint8_t playerNumber) {
    board[y][x] = playerNumber;

    std::vector<uint8_t*> recolorVec;
    // iterate over directions
    for (uint8_t i = 0; i < 8; i++) {
        // if direction is validated mark all stones in this direction for recolor
        if (Moves::validateDirection(board, x, y, playerNumber, i) > 0) {
            markRecolor(board, x, y, playerNumber, i, recolorVec);
        }
    }
    Moves::recolor(playerNumber, recolorVec);
}

void Moves::markRecolor(uint8_t** board, uint8_t x, uint8_t y, uint8_t playerNumber, uint8_t direction, std::vector<uint8_t*>& markedTiles) {
    // no neighbors found
    if (!CurrentState::mapNeighbors[y][x]) {
        return;
    }
    // get first neighbor at direction i
    Neighbor n = CurrentState::mapNeighbors[y][x][direction];
    // check if no neighbor in direction
    if (n.pos > 7) {
        return;
    }
    // push first neighbor in given direction to vec
    markedTiles.push_back(&board[n.y][n.x]);

    // set all the upcoming neighbors to playerNumber until playerNumber comes
    while (true) {
        // should not occure (at least the tile you came from should be a neighbor)
        if (!CurrentState::mapNeighbors[n.y][n.x]) {
            return;
        }
        // get next Neighbor in the opposite direction you came from
        n = CurrentState::mapNeighbors[n.y][n.x][oppositeDirectionLookup[n.pos]];
        // check if no neighbor in direction
        if (n.pos > 7) {
            return;
        }
        // if value in boardArr is playerNumber -> recoloring for the given direction done
        if (board[n.y][n.x] == playerNumber) {
            return;
        }
        // push tile for recoloring
        markedTiles.push_back(&board[n.y][n.x]);
    }
}

void Moves::recolor(uint8_t playerNumber, std::vector<uint8_t*>& markedTiles) {
    for (uint8_t* entry : markedTiles) {
        *entry = playerNumber;
    }
}


#include "client.h"

#define	SERVER_CLOSED_CONNECTION 0
#define SERVER_TRANSMITTED_MAP 2
#define SERVER_ASSIGNED_PLAYERNUMBER 3
#define SEND_MOVE_TO_SERVER 4
#define GOT_MOVE_FROM_SERVER 6
#define SERVER_SENT_DISQUALIFICATION 7
#define SERVER_SENT_END_PHASE_ONE 8
#define SERVER_SENT_END_PHASE_TWO 9	
	
void Client::runClient() {
    uint8_t* msg;
    uint8_t type;
    uint32_t len;
    bool running = true;

    uint8_t playerNumber;
    uint8_t x, y;

    msg = new uint8_t[1];
    msg[0] = GROUPNUM;
    Network::sendMsg(1, 1, msg);
    delete[] msg;

    while (running) {
        Network::recvMsg(&type, &len, &msg);
        switch (type) {
            case SERVER_CLOSED_CONNECTION: {
                running = false;
                break;
            }
            // message contains map (player count, board dimensions, board, etc.)
            case SERVER_TRANSMITTED_MAP: {
                std::string map((char*)msg, len);
                std::stringstream ss;
                ss << map;
#ifdef CLIENT_LOGGING
                std::cout << "Map received. Loading...\n";
#endif
                if (!Map::loadMap(ss)) {
#ifdef CLIENT_LOGGING
                    std::cout << "Loading the map failed.\n";
#endif
                    std::cerr << "Map could not be parsed!\n";
                    exit(EXIT_FAILURE);
                }
#ifdef CLIENT_LOGGING
                std::cout << "Map successfully loaded.\n\n";
#endif

                delete[] msg;
                break;
            }
            // message contains player number
            case SERVER_ASSIGNED_PLAYERNUMBER: {
                GameDetails::playerNumber = msg[0];
#ifdef CLIENT_LOGGING
                std::cout << "\n\n===== Playernumber: " << int(GameDetails::playerNumber) << " =====\n\n";
#endif
                delete[] msg;
                break;
            }
            // message contains move promt
            case SEND_MOVE_TO_SERVER: {
#ifdef CLIENT_LOGGING
                std::cout << "\n\n======= Move " << std::setw(4) << ++CurrentState::moveCount << " =======\n";
#endif
                // get timelimit and/or search depth (basic client always makes a random move)
                CurrentState::timelimit = (msg[0] << 24) + (msg[1] << 16) + (msg[2] << 8) + msg[3];
                CurrentState::searchDepth = msg[4];

                if (CurrentState::timelimit) {
                    // searchTime given
#ifdef CLIENT_LOGGING
                    std::cout << "== Timelimit: " << CurrentState::timelimit << "ms ==\n"
                              << "Getting random move...\n";
                    auto startTime = std::chrono::steady_clock::now();
#endif
                    Moves::getRandomMove(x, y, CurrentState::boardArr, GameDetails::playerNumber);
#ifdef CLIENT_LOGGING
                    auto endTime = std::chrono::steady_clock::now();
                    int64_t d = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
                    std::cout << "Random move found (x: " << int(x) << ", y: " << int(y) << ").\n"
                              << "Time spent: " << d << "ms\n";
#endif

                } else {
                    // searchDepth given
#ifdef CLIENT_LOGGING
                    std::cout << "== Search depth: " << int(CurrentState::searchDepth) << " ==\n"
                              << "Searching for random move...\n";
                    auto startTime = std::chrono::steady_clock::now();
#endif
                    Moves::getRandomMove(x, y, CurrentState::boardArr, GameDetails::playerNumber);
#ifdef CLIENT_LOGGING
                    auto endTime = std::chrono::steady_clock::now();
                    int64_t d = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
                    std::cout << "Random move found (x: " << int(x) << ", y: " << int(y) << ").\n"
                              << "Time spent: " << d << "ms\n";
#endif
                }
#ifdef CLIENT_LOGGING
                std::cout << "=========================\n";
#endif

                // create msg
                msg = new uint8_t[5];
                msg[0] = 0;
                msg[1] = x;
                msg[2] = 0;
                msg[3] = y;
                msg[4] = 0;  // not needed in basic client

                Network::sendMsg(5, 5, msg);

                delete[] msg;
                break;
            }
            // message contains player move
            case GOT_MOVE_FROM_SERVER: {
                // makeMove for given Player
                // (msg[0] << 8) + msg[1] -> 16bit uint but on a 50x50 board only 8bit needed
                x = msg[1];
                y = msg[3];
                playerNumber = msg[5];

                Moves::makeMove(CurrentState::boardArr, x, y, playerNumber);

#ifdef CLIENT_LOGGING
                if (playerNumber == GameDetails::playerNumber) {
                    CurrentState::moveCount--;
                }
                std::cout << "\n\n===== Move " << std::setw(4) << ++CurrentState::moveCount << " received =====\n"
                          << "==== Player: " << int(playerNumber) << " (Move(x: " << int(x) << ", y: " << int(y) << ")) ====\n";
#endif

#ifdef BOARD_LOGGING
                //! because of log file cutting in matchpoint (just for debugging)
                // if (CurrentState::moveCount > 500) {
                std::cout << "\n\n==== Current Board ====\n";
                CurrentState::printBoardArr();
                // }
#endif
                delete[] msg;
                break;
            }
            // message contains player disqualification
            case SERVER_SENT_DISQUALIFICATION: {
                // store disqualification in playerArr
                CurrentState::playerArr[msg[0] - 1].isDisqualified = true;

#ifdef CLIENT_LOGGING
                std::cout << "\n\nPlayer " << int(msg[0]) << " is disqualified.\n";
#endif
                CurrentState::activePlayerCount = Players::getActivePlayerCount();
                delete[] msg;
                break;
            }
            // end of phase 1
            case SERVER_SENT_END_PHASE_ONE: {
                // no bomb phase implemented in basic client
                break;
            }
            // end phase 2
            case SERVER_SENT_END_PHASE_TWO: {
#ifdef CLIENT_LOGGING
                std::cout << "\n======= Game Over =======\n";
#endif
                break;
            }
            default: {
                std::cerr << "unexpected msg-type received\n";
                exit(EXIT_FAILURE);
            }
        }
    }
}
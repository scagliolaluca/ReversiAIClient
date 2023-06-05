#include "minimax.hpp"

#include "gamedetails.h"
#include "currentstate.h"
#include "moves.h"

#include <limits.h>
#include <algorithm>
#include <stack>
#include <iostream>

namespace Minimax
{
    Node::~Node() {
        delete2DArr(board, GameDetails::boardHeight);
    }

    Node::Node(Node &&other) {
        board = other.board;
        other.board = nullptr;

        validMoves = std::move(other.validMoves);
        value = other.value;
        player = other.player;
    }

    void getMoveMinimax(uint8_t &x, uint8_t &y, uint8_t **board, uint8_t playerNumber, uint8_t maxDepth, const std::function<int(uint8_t **)> &heuristic) {

        std::stack<Node> nodeStack;
        nodeStack.push(std::move(Node()));

        Node &root = nodeStack.top();
        root.board = copy2DArr(board, GameDetails::boardHeight, GameDetails::boardWidth);
        Moves::populateValidMoves(root.validMoves, board, playerNumber);
        root.value = INT_MIN;
        root.player = playerNumber;

        uint8_t depth = 0;
        Move currentRootMove = root.validMoves.back();
        x = currentRootMove.x;
        y = currentRootMove.y;

        /*
        std::cout << "Moves (root):" << std::endl;
        for (auto &element : nodeStack.top().validMoves) {
            std::cout << "X = " << (int)element.x << ", Y = " << (int)element.y << std::endl;
        }
        */

        // Iterative DFS
        while (!nodeStack.empty()) {
            Node &currentNode = nodeStack.top();
            //std::cout << "Current node player " << (int)currentNode.player << " on depth " << (int)depth << std::endl;

            // All moves done
            if (currentNode.validMoves.empty()) {
                //std::cout << "Node finished for player " << (int)currentNode.player << " on depth " << (int)depth << std::endl;
                if (depth == 0) {
                    break;
                }

                int compareVal = currentNode.value;

                --depth;
                nodeStack.pop();

                Node &node = nodeStack.top();
                // Maximize
                if (node.player == playerNumber) {
                    if (compareVal > node.value) {
                        node.value = compareVal;
                        if (depth == 0) {
                            x = currentRootMove.x;
                            y = currentRootMove.y;
                        }
                    }
                }
                    // Minimize
                else {
                    if (compareVal < node.value) {
                        node.value = compareVal;
                    }
                }

                continue;
            }

            // Generate new node
            Move currentMove = currentNode.validMoves.back();
            if (depth == 0) {
                currentRootMove = currentMove;
            }
            currentNode.validMoves.pop_back();

            Node newNode;
            // new Board
            newNode.board = copy2DArr(currentNode.board, GameDetails::boardHeight, GameDetails::boardWidth);
            Moves::makeMove(newNode.board, currentMove.x, currentMove.y, currentNode.player);

            // new Player + Moves
            newNode.player = nextValidPlayerMoves(newNode.validMoves, newNode.board, currentNode.player);

            bool isLeaf = false;
            // If game ended
            if (newNode.player == 0) {
                isLeaf = true;
                // Win
                if (highestPieceCount(newNode.board) == playerNumber) {
                    newNode.value = INT_MAX;
                }
                    // Loss
                else {
                    newNode.value = INT_MIN;
                }
                //std::cout << "Leaf node (game ended) after player " << (int)currentNode.player << " on depth " << (int)depth + 1 << std::endl;
            }
                // If reached max depth
            else if (depth + 1 >= maxDepth) {
                isLeaf = true;
                newNode.value = heuristic(newNode.board);
                //std::cout << "Leaf node (max depth reached) after player " << (int)currentNode.player << " on depth " << (int)depth + 1 << std::endl;
            }

            // Leaf nodes are handled immediately -> aren't added to the stack
            if (isLeaf) {
                // Maximize
                if (currentNode.player == playerNumber) {
                    if (newNode.value > currentNode.value) {
                        currentNode.value = newNode.value;
                        if (depth == 0) {
                            x = currentRootMove.x;
                            y = currentRootMove.y;
                        }
                    }
                }
                    // Minimize
                else {
                    if (newNode.value < currentNode.value) {
                        currentNode.value = newNode.value;
                    }
                }
            }
            else {
                if (newNode.player == playerNumber) {
                    newNode.value = INT_MIN;
                }
                else {
                    newNode.value = INT_MAX;
                }

                ++depth;
                /*
                std::cout << "Created player " << (int)newNode.player << " node on depth " << (int)depth << std::endl;
                std::cout << "Moves:" << std::endl;
                for (auto &element : newNode.validMoves) {
                    std::cout << "X = " << (int)element.x << ", Y = " << (int)element.y << std::endl;
                }
                */
                nodeStack.push(std::move(newNode));
            }
        }
        return;
    }


    bool getMoveMinimaxTimeCheck(uint8_t &x, uint8_t &y, uint8_t **board, uint8_t playerNumber, uint8_t maxDepth, uint32_t maxTime, const std::chrono::time_point<std::chrono::steady_clock> startTime, bool &islastMove, const std::function<int(uint8_t **)> &heuristic) {

        std::stack<Node> nodeStack;
        nodeStack.push(std::move(Node()));

        Node &root = nodeStack.top();
        root.board = copy2DArr(board, GameDetails::boardHeight, GameDetails::boardWidth);
        Moves::populateValidMoves(root.validMoves, board, playerNumber);
        root.value = INT_MIN;
        root.player = playerNumber;

        uint8_t depth = 0;
        Move currentRootMove = root.validMoves.back();
        x = currentRootMove.x;
        y = currentRootMove.y;

        bool continue_calculation = false;

        /*
        std::cout << "Moves (root):" << std::endl;
        for (auto &element : nodeStack.top().validMoves) {
            std::cout << "X = " << (int)element.x << ", Y = " << (int)element.y << std::endl;
        }
        */

        // Iterative DFS
        while (!nodeStack.empty()) {
            if(IterativeDeepening::checkForTimeEnd(maxTime, startTime)) {
                Node &currentNode = nodeStack.top();
                //std::cout << "Current node player " << (int)currentNode.player << " on depth " << (int)depth << std::endl;

                // All moves done
                if (currentNode.validMoves.empty()) {
                    //std::cout << "Node finished for player " << (int)currentNode.player << " on depth " << (int)depth << std::endl;
                    if (depth == 0) {
                        break;
                    }

                    int compareVal = currentNode.value;

                    --depth;
                    nodeStack.pop();

                    Node &node = nodeStack.top();
                    // Maximize
                    if (node.player == playerNumber) {
                        if (compareVal > node.value) {
                            node.value = compareVal;
                            if (depth == 0) {
                                x = currentRootMove.x;
                                y = currentRootMove.y;
                            }
                        }
                    }
                        // Minimize
                    else {
                        if (compareVal < node.value) {
                            node.value = compareVal;
                        }
                    }

                    continue;
                }

                // Generate new node
                Move currentMove = currentNode.validMoves.back();
                if (depth == 0) {
                    currentRootMove = currentMove;
                }
                currentNode.validMoves.pop_back();

                Node newNode;
                // new Board
                newNode.board = copy2DArr(currentNode.board, GameDetails::boardHeight, GameDetails::boardWidth);
                Moves::makeMove(newNode.board, currentMove.x, currentMove.y, currentNode.player);

                // new Player + Moves
                newNode.player = nextValidPlayerMoves(newNode.validMoves, newNode.board, currentNode.player);

                bool isLeaf = false;
                // If game ended
                if (newNode.player == 0) {
                    isLeaf = true;

                    // If game ended in depth 0 (needed in iterativeDeepening)
                    if(depth == 0){
                        islastMove = true;
                        std::cout << "Depth: " << int(depth) << std::endl;
                    }

                    // Win
                    if (highestPieceCount(newNode.board) == playerNumber) {
                        newNode.value = INT_MAX;
                    }
                        // Loss
                    else {
                        newNode.value = INT_MIN;
                    }
                    //std::cout << "Leaf node (game ended) after player " << (int)currentNode.player << " on depth " << (int)depth + 1 << std::endl;
                }
                    // If reached max depth
                else if (depth + 1 >= maxDepth) {
                    isLeaf = true;
                    newNode.value = heuristic(newNode.board);
                    //std::cout << "Leaf node (max depth reached) after player " << (int)currentNode.player << " on depth " << (int)depth + 1 << std::endl;
                    continue_calculation = true;
                }

                // Leaf nodes are handled immediately -> aren't added to the stack
                if (isLeaf) {
                    // Maximize
                    if (currentNode.player == playerNumber) {
                        if (newNode.value > currentNode.value) {
                            currentNode.value = newNode.value;
                            if (depth == 0) {
                                x = currentRootMove.x;
                                y = currentRootMove.y;
                            }
                        }
                    }
                        // Minimize
                    else {
                        if (newNode.value < currentNode.value) {
                            currentNode.value = newNode.value;
                        }
                    }
                } else {
                    if (newNode.player == playerNumber) {
                        newNode.value = INT_MIN;
                    } else {
                        newNode.value = INT_MAX;
                    }

                    ++depth;
                    /*
                    std::cout << "Created player " << (int)newNode.player << " node on depth " << (int)depth << std::endl;
                    std::cout << "Moves:" << std::endl;
                    for (auto &element : newNode.validMoves) {
                        std::cout << "X = " << (int)element.x << ", Y = " << (int)element.y << std::endl;
                    }
                    */
                    nodeStack.push(std::move(newNode));
                }
            }
            else{
                std::cout << "No Time left, so break calculation and return with previous iteration" << std::endl;
                continue_calculation = false;
                return continue_calculation;
            }
        }

        return continue_calculation;
    }

    uint8_t highestPieceCount(uint8_t **board) {
        std::vector<int> pieceCounts(GameDetails::playerCount, 0);
        uint8_t boardVal;

        for (uint8_t i = 0; i < GameDetails::boardHeight; ++i) {
            for (uint8_t j = 0; j < GameDetails::boardWidth; ++j) {
                boardVal = board[i][j];
                if (boardVal >= 1 && boardVal <= GameDetails::playerCount) {
                    ++pieceCounts[boardVal - 1];
                }
            }
        }

        auto maxCountIt = std::max_element(pieceCounts.begin(), pieceCounts.end());
        return std::distance(pieceCounts.begin(), maxCountIt) + 1;
    }

    uint8_t nextValidPlayerMoves(std::vector<Move> &validMoves, uint8_t **board, uint8_t currentPlayer) {
        int8_t nextPlayer = currentPlayer + 1;
        if (nextPlayer > GameDetails::playerCount) {
            nextPlayer = 1;
        }

        while (nextPlayer != currentPlayer) {
            if (CurrentState::playerArr[nextPlayer-1].isDisqualified) {
                ++nextPlayer;
                if (nextPlayer > GameDetails::playerCount) {
                    nextPlayer = 1;
                }
                continue;
            }
            Moves::populateValidMoves(validMoves, board, nextPlayer);
            if (validMoves.empty()) {
                ++nextPlayer;
                if (nextPlayer > GameDetails::playerCount) {
                    nextPlayer = 1;
                }
                continue;
            }
            return nextPlayer;
        } 

        return 0;
    }
} // namespace Minimax

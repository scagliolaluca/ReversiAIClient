#include "minimax.hpp"

#include "gamedetails.h"
#include "currentstate.h"
#include "moves.h"
#include "heuristics.h"

#include <limits>
#include <stack>
#include <iostream>

namespace Minimax
{
    Node::Node(Node &&other) {
        board = other.board;
        other.board = nullptr;

        validMoves = std::move(other.validMoves);
        currentMoveIndex = other.currentMoveIndex;
        value = other.value;
        player = other.player;
    }
    Node::~Node() {
        delete2DArr(board, GameDetails::boardHeight);
    }
    bool Node::hasValidMoves() {
        return currentMoveIndex < validMoves.size();
    }
    const Move &Node::nextMove() {
        return validMoves[currentMoveIndex++];
    }

    void getMoveMinimax(uint8_t &x, uint8_t &y, uint8_t **board, uint8_t playerNumber, uint8_t maxDepth, const std::function<float(uint8_t **, uint8_t)> &heuristic) {
        bool reached;
        auto stopTime = std::chrono::steady_clock::now() + std::chrono::hours(24);
        getMoveMinimax(x, y, reached, board, playerNumber, maxDepth, heuristic, stopTime);
        return;
    }

    bool getMoveMinimax(uint8_t &x, uint8_t &y, bool &reachedMaxDepth, uint8_t **board, uint8_t playerNumber, uint8_t maxDepth, const std::function<float(uint8_t **, uint8_t)> &heuristic, const std::chrono::time_point<std::chrono::steady_clock> &stopTime) {

        std::stack<Node> nodeStack;
        nodeStack.push(std::move(Node()));

        Node &root = nodeStack.top();
        root.board = copy2DArr(board, GameDetails::boardHeight, GameDetails::boardWidth);
        Moves::populateValidMoves(root.validMoves, board, playerNumber);
        root.value = std::numeric_limits<float>::lowest();
        root.player = playerNumber;

        uint8_t depth = 0;
        Move currentRootMove = root.validMoves.front();
        x = currentRootMove.x;
        y = currentRootMove.y;
        reachedMaxDepth = false;

        /*
        std::cout << "Moves (root):" << std::endl;
        for (auto &element : nodeStack.top().validMoves) {
            std::cout << "X = " << (int)element.x << ", Y = " << (int)element.y << std::endl;
        }
        */

        // Iterative DFS
        while (!nodeStack.empty()) {

            if(!checkTimeLeft(stopTime)) {
                std::cout << "No Time left, so break calculation and return with previous iteration" << std::endl;
                return false;
            }

            Node &currentNode = nodeStack.top();
            //std::cout << "Current node player " << (int)currentNode.player << " on depth " << (int)depth << std::endl;

            // All moves done
            if (!currentNode.hasValidMoves()) {
                //std::cout << "Node finished for player " << (int)currentNode.player << " on depth " << (int)depth << std::endl;
                if (depth == 0) {
                    break;
                }

                float compareVal = currentNode.value;

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
            const Move &currentMove = currentNode.nextMove();
            if (depth == 0) {
                currentRootMove = currentMove;
            }

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
                newNode.value = Heuristics::evaluateEndState(newNode.board, playerNumber);
                //std::cout << "Leaf node (game ended) after player " << (int)currentNode.player << " on depth " << (int)depth + 1 << std::endl;

                /*
                // If game ended in depth 0 (needed in iterativeDeepening)
                if(depth == 0){
                    islastMove = true;
                    std::cout << "Depth: " << int(depth) << std::endl;
                }
                */
            }
            // If reached max depth
            else if (depth + 1 >= maxDepth) {
                isLeaf = true;
                newNode.value = heuristic(newNode.board, playerNumber);
                //std::cout << "Leaf node (max depth reached) after player " << (int)currentNode.player << " on depth " << (int)depth + 1 << std::endl;
                reachedMaxDepth = true;
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
                    newNode.value = std::numeric_limits<float>::lowest();
                }
                else {
                    newNode.value = std::numeric_limits<float>::max();
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
        return true;
    }

    bool checkTimeLeft(const std::chrono::time_point<std::chrono::steady_clock> &stopTime) {
        auto currentTime = std::chrono::steady_clock::now();
        int64_t timeLeft = std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - currentTime).count();

        // If enough time left
        if(timeLeft > 10){
            return true;
        }
        else{
            return false;
        }
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

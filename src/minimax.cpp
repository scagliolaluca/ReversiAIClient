#include "minimax.hpp"

#include "gamedetails.h"
#include "currentstate.h"
#include "moves.h"
#include "heuristics.h"
#include "debugUtils.hpp"

#include <limits>
#include <stack>
#include <iostream>
#include <algorithm> 
#include <vector> 
#include <numeric>
#include <ranges>

namespace Minimax
{
    Node::Node(Node &&other) {
        board = other.board;
        other.board = nullptr;

        validMoves = std::move(other.validMoves);
        currentMoveIndex = other.currentMoveIndex;
        value = other.value;
        player = other.player;

        alpha = other.alpha;
        beta = other.beta;

        bestFollowing = other.bestFollowing;
        moveToGetHere_x = other.moveToGetHere_x;
        moveToGetHere_y = other.moveToGetHere_y;
        hash = other.hash;
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

        //Pruning -> initialize in root
        root.alpha = std::numeric_limits<float>::lowest();;
        root.beta = std::numeric_limits<float>::max();

        //TT -> calculate completely in root
        root.hash = ZobristKey::generateZobristValue(root.board, root.player);

        uint8_t depth = 0;
        Move currentRootMove = root.validMoves.front();
        x = currentRootMove.x;
        y = currentRootMove.y;
        reachedMaxDepth = false;

        // Iterative DFS
        while (!nodeStack.empty()) {

            if(!checkTimeLeft(stopTime)) {
                std::cout << "No Time left, so break calculation and return with previous iteration" << std::endl;
                return false;
            }

            Node &currentNode = nodeStack.top();

            // check TranspositionTable for this position
            if (TranspositionTable::alreadySeen(currentNode.hash)) {
                TranspositionTable::Entry *prevCalc = TranspositionTable::getEntry(currentNode.hash);
                // The TT entry can be used, don't calculate deeper from this node, update parent nodes
                if (prevCalc->calculatedDepth - prevCalc->depth >= maxDepth - depth) {

                    // if first (root) node -> simply return
                    if (depth == 0) {
                        std::cout << "found replacement at root" << std::endl;
                        x = prevCalc->x;
                        y = prevCalc->y;
                        reachedMaxDepth = true;
                        break;
                    }

                    uint8_t moveToGetHere_x = currentNode.moveToGetHere_x;
                    uint8_t moveToGetHere_y = currentNode.moveToGetHere_y;
                    --depth;
                    nodeStack.pop();
                    Node &node = nodeStack.top();

                    // Maximize
                    if (node.player == playerNumber) {
                        if (prevCalc->value > node.value) {
                            node.value = prevCalc->value;

                            // TT -> set best following value and move to that value
                            node.bestFollowing.x = moveToGetHere_x;
                            node.bestFollowing.y = moveToGetHere_y;
                            node.bestFollowing.value = prevCalc->value;
                            node.bestFollowing.calculatedDepth = prevCalc->calculatedDepth;

                            if (depth == 0) {
                                x = currentRootMove.x;
                                y = currentRootMove.y;
                            }
                        }
                        //Pruning -> update alpha going up
                        if(node.value > node.alpha){
                            node.alpha = node.value;
                        }
                    }
                        // Minimize
                    else {
                        if (prevCalc->value < node.value) {
                            node.value = prevCalc->value;
                        }

                        // TT -> set best following value and move to that value
                        node.bestFollowing.x = moveToGetHere_x;
                        node.bestFollowing.y = moveToGetHere_y;
                        node.bestFollowing.value = prevCalc->value;
                        node.bestFollowing.calculatedDepth = prevCalc->calculatedDepth;

                        //Pruning -> update beta going up
                        if(node.value < node.beta){
                            node.beta = node.value;
                        }

                    }
                    reachedMaxDepth = true;
                    continue;
                }
            }

            // All moves done
            if (!currentNode.hasValidMoves()) {
                if (depth == 0) {
                    //add first Node to TT too???
                    break;
                }

                float compareVal = currentNode.value;
                uint8_t calculatedDepth = currentNode.bestFollowing.calculatedDepth;
                uint8_t moveToGetHere_x = currentNode.moveToGetHere_x;
                uint8_t moveToGetHere_y = currentNode.moveToGetHere_y;

                //add Node to Hashmap
                TranspositionTable::addEntry(currentNode.hash, currentNode.bestFollowing.x, currentNode.bestFollowing.y, depth, calculatedDepth, compareVal);

                --depth;

                nodeStack.pop();

                Node &node = nodeStack.top();

                // Maximize
                if (node.player == playerNumber) {
                    if (compareVal > node.value) {
                        node.value = compareVal;

                        // TT -> set best following value and move to that value
                        node.bestFollowing.x = moveToGetHere_x;
                        node.bestFollowing.y = moveToGetHere_y;
                        node.bestFollowing.value = compareVal;
                        node.bestFollowing.calculatedDepth = calculatedDepth;

                        if (depth == 0) {
                            x = currentRootMove.x;
                            y = currentRootMove.y;
                        }
                    }
                    //Pruning -> update alpha going up
                    if(node.value > node.alpha){
                        node.alpha = node.value;
                    }
                }
                // Minimize
                else {
                    if (compareVal < node.value) {
                        node.value = compareVal;
                    }

                    // TT -> set best following value and move to that value
                    node.bestFollowing.x = moveToGetHere_x;
                    node.bestFollowing.y = moveToGetHere_y;
                    node.bestFollowing.value = compareVal;
                    node.bestFollowing.calculatedDepth = calculatedDepth;

                    //Pruning -> update beta going up
                    if(node.value < node.beta){
                        node.beta = node.value;
                    }

                }
                continue;
            }
            //Pruning Cuttoffs
            if(currentNode.player == playerNumber && currentNode.value >= currentNode.beta){
                //Maximize
                currentNode.currentMoveIndex = currentNode.validMoves.size();
                continue;
            }
            if(currentNode.player != playerNumber && currentNode.value <= currentNode.alpha){
                //Minimize
                currentNode.currentMoveIndex = currentNode.validMoves.size();
                continue;
            }

            // Generate new node
            const Move &currentMove = currentNode.nextMove();
            if (depth == 0) {
                currentRootMove = currentMove;
            }
            Node newNode;

            newNode.hash = currentNode.hash;

            // new Board, update Hash
            newNode.board = copy2DArr(currentNode.board, GameDetails::boardHeight, GameDetails::boardWidth);
            Moves::makeMove(newNode.board, currentMove.x, currentMove.y, currentNode.player, newNode.hash);

            // new Player + Moves
            newNode.player = nextValidPlayerMoves(newNode.validMoves, newNode.board, currentNode.player);

            // refresh movright in Hashvalue
            ZobristKey::xorInOutPlayer(newNode.hash, newNode.player, currentNode.player);


            bool isLeaf = false;
            // If game ended
            if (newNode.player == 0) {
                isLeaf = true;

                // check TranspositionTable for this position
                // This position was seen and calculated before
                if (TranspositionTable::alreadySeen(newNode.hash)) {
                    TranspositionTable::Entry *prevCalc = TranspositionTable::getEntry(newNode.hash);

                    newNode.value = prevCalc->value;
                    // Maximize
                    if (currentNode.player == playerNumber) {
                        if (prevCalc->value > currentNode.value) {
                            currentNode.value = prevCalc->value;
                            if (depth == 0) {
                                x = currentRootMove.x;
                                y = currentRootMove.y;
                            }
                            //Pruning -> set alpha to highest leaf node one depth above
                            currentNode.alpha = prevCalc->value;
                            // TT -> get data for best move from this node
                            currentNode.bestFollowing.x = currentMove.x;
                            currentNode.bestFollowing.y = currentMove.y;
                            currentNode.bestFollowing.value = prevCalc->value;
                            currentNode.bestFollowing.calculatedDepth = depth+1;
                        }
                    }
                    // Minimize
                    else {
                        if (prevCalc->value < currentNode.value) {
                            currentNode.value = prevCalc->value;
                            //Pruning -> set beta to lowest leaf node one depth above
                            currentNode.beta = prevCalc->value;
                            // TT -> get data for best move from this node
                            currentNode.bestFollowing.x = currentMove.x;
                            currentNode.bestFollowing.y = currentMove.y;
                            currentNode.bestFollowing.value = prevCalc->value;
                            currentNode.bestFollowing.calculatedDepth = depth+1;
                        }
                    }
                    continue;
                }
                newNode.value = Heuristics::evaluateEndState(newNode.board, playerNumber);

            }
            // If reached max depth
            else if (depth + 1 >= maxDepth) {
                isLeaf = true;

                // check TranspositionTable for this position
                // This position was seen and calculated before
                if (TranspositionTable::alreadySeen(newNode.hash)) {
                    TranspositionTable::Entry *prevCalc = TranspositionTable::getEntry(newNode.hash);

                    newNode.value = prevCalc->value;
                    // Maximize
                    if (currentNode.player == playerNumber) {
                        if (prevCalc->value > currentNode.value) {
                            currentNode.value = prevCalc->value;
                            if (depth == 0) {
                                x = currentRootMove.x;
                                y = currentRootMove.y;
                            }
                            //Pruning -> set alpha to highest leaf node one depth above
                            currentNode.alpha = prevCalc->value;
                            // TT -> get data for best move from this node
                            currentNode.bestFollowing.x = currentMove.x;
                            currentNode.bestFollowing.y = currentMove.y;
                            currentNode.bestFollowing.value = prevCalc->value;
                            currentNode.bestFollowing.calculatedDepth = depth+1;
                        }
                    }
                    // Minimize
                    else {
                        if (prevCalc->value < currentNode.value) {
                            currentNode.value = prevCalc->value;
                            //Pruning -> set beta to lowest leaf node one depth above
                            currentNode.beta = prevCalc->value;
                            // TT -> get data for best move from this node
                            currentNode.bestFollowing.x = currentMove.x;
                            currentNode.bestFollowing.y = currentMove.y;
                            currentNode.bestFollowing.value = prevCalc->value;
                            currentNode.bestFollowing.calculatedDepth = depth+1;
                        }
                    }
                    reachedMaxDepth = true;
                    continue;
                }

                newNode.value = heuristic(newNode.board, playerNumber);
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
                        //Pruning -> set alpha to highest leaf node one depth above
                        currentNode.alpha = newNode.value;
                        // TT -> get data for best move from this node
                       currentNode.bestFollowing.x = currentMove.x;
                        currentNode.bestFollowing.y = currentMove.y;
                        currentNode.bestFollowing.value = newNode.value;
                        currentNode.bestFollowing.calculatedDepth = depth+1;
                    }
                }
                // Minimize
                else {
                    if (newNode.value < currentNode.value) {
                        currentNode.value = newNode.value;
                        //Pruning -> set beta to lowest leaf node one depth above
                        currentNode.beta = newNode.value;
                        // TT -> get data for best move from this node
                       currentNode.bestFollowing.x = currentMove.x;
                        currentNode.bestFollowing.y = currentMove.y;
                        currentNode.bestFollowing.value = newNode.value;
                        currentNode.bestFollowing.calculatedDepth = depth+1;
                    }
                }
                // Add leave to TT, but not all information available: (nextMove missing)
               TranspositionTable::addLeaveEntry(newNode.hash, depth+1, depth+1, newNode.value);
            } 
            else {
                // sort moves for pruning (not in nodes before leafs)
                if(depth + 2 < maxDepth){
                    sortMoves(newNode.validMoves, board, newNode.player);
                }
                //Pruning -> pass down alpha and beta
                newNode.alpha = currentNode.alpha;
                newNode.beta = currentNode.beta;

                // TT -> save move to get to this node
                newNode.moveToGetHere_x = currentMove.x;
                newNode.moveToGetHere_y = currentMove.y;

                if (newNode.player == playerNumber) {
                    newNode.value = std::numeric_limits<float>::lowest();
                }
                else {
                    newNode.value = std::numeric_limits<float>::max();
                }

                ++depth;

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

    std::vector<int> getEvaluationVector(std::vector<Move> &moves, uint8_t **board, uint8_t player){
        int vectorSize = moves.size();
        std::vector<int> resultVector(vectorSize); 

        for (int i = 0; i < vectorSize; i++) {
            //copy board
            uint8_t** boardCopy = copy2DArr(board, GameDetails::boardHeight, GameDetails::boardWidth);

            //ealuate board after move
            Moves::makeMove(boardCopy, moves[i].x, moves[i].y, player);
            resultVector[i] = Heuristics::getScore(boardCopy, player);

            // Delete copy
            delete2DArr(boardCopy, GameDetails::boardHeight);
        }

        return resultVector;
    }


    void sortMoves(std::vector<Move> &moves, uint8_t **board, uint8_t player){
        //create vector with values from evaluation function
        std::vector<int> resultVector = getEvaluationVector(moves, board, player);

        // Sort the array based on the values of the 'values' vector
        std::sort(moves.begin(), moves.end(), [=](const Move& a, const Move& b) {
            // Find the indices of 'a' and 'b' in resultVector
            auto it_a = std::find(moves.begin(), moves.end(), a);
            auto it_b = std::find(moves.begin(), moves.end(), b);

            return resultVector[std::distance(moves.begin(), it_a)] > resultVector[std::distance(moves.begin(), it_b)];
        });
        return;
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

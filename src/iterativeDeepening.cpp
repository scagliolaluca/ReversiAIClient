#include "iterativeDeepening.h"

#include <math.h>
#include <iostream>


namespace IterativeDeepening
{

    void getMoveIterativeDeepening(uint8_t &x, uint8_t &y, uint8_t **board, uint8_t playerNumber, uint32_t maxTime, const std::chrono::time_point<std::chrono::steady_clock> startTime, const std::function<int(uint8_t **)> &heuristic){

        uint8_t iterationDepth = 1;
        uint8_t tempx = 0;
        uint8_t tempy = 0;

        while(timeForNextMove(iterationDepth, startTime, maxTime, 4.5)){
            x = tempx;
            y = tempy;
            if(Minimax::getMoveMinimaxTimeCheck(tempx, tempy, board, playerNumber, iterationDepth, maxTime, startTime, heuristic)){
                std::cout << "Found x,y: " << int(tempx) << int(tempy) << " in depth " << int(iterationDepth) << std::endl;
                x = tempx;
                y = tempy;
            }
            else{
                std::cout << "Minimax couldn't finish in time" << std::endl;
                return;
            }

            iterationDepth++;
        }

        return;

    }

    bool timeForNextMove(uint8_t iterationDepth, const std::chrono::time_point<std::chrono::steady_clock> startTime, uint32_t maxTime, float branchingAverage){

        //calculate left time
        auto currentTime = std::chrono::steady_clock::now();
        int64_t spentTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
        int64_t timeLeft = maxTime - spentTime;
        std::cout << "Time left before " << int(iterationDepth) << " depth is " << timeLeft << std::endl;


        //calculate estimated Time for next iteration
        int64_t estimatedTime = std::pow(branchingAverage, iterationDepth) * 0.02;
        std::cout << "Estimated time for depth " << int(iterationDepth) << " is " << estimatedTime << std::endl;
        if (estimatedTime < timeLeft){
            return true;
        }
        else {
            std::cout << "No Time left for next Iteration!" << std::endl;
            return false;
        }
    }

    bool checkForTimeEnd(uint32_t maxTime, const std::chrono::time_point<std::chrono::steady_clock> startTime) {

        auto currentTime = std::chrono::steady_clock::now();
        int64_t spentTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
        int64_t timeLeft = maxTime - spentTime;
        if(timeLeft > 1){
            return 1;
        }
        else{
            return 0;
        }
    }
} //namespace IterativeDeepening


#include "iterativeDeepening.h"

#include <math.h>
#include <iostream>


namespace IterativeDeepening
{

    void getMoveIterativeDeepening(uint8_t &x, uint8_t &y, uint8_t **board, uint8_t playerNumber, uint32_t maxTime, const std::chrono::time_point<std::chrono::steady_clock> startTime, const std::function<int(uint8_t **)> &heuristic){

        uint8_t iterationDepth = 1;
        uint8_t tempx;
        uint8_t tempy;

        while(timeForNextIteration(iterationDepth, startTime, maxTime)){
            x = tempx;
            y = tempy;
            if(Minimax::getMoveMinimaxTimeCheck(tempx, tempy, board, playerNumber, iterationDepth, maxTime, startTime, heuristic)){
                std::cout << "Found x,y: " << int(tempx) << int(tempy) << " in depth " << int(iterationDepth) << std::endl;
                x = tempx;
                y = tempy;
            }
            else{
                std::cout << "\nMinimax couldn't finish in time, or game Ended\n" << std::endl;
                return;
            }

            iterationDepth++;
        }

        return;

    }

    bool timeForNextIteration(uint8_t iterationDepth, const std::chrono::time_point<std::chrono::steady_clock> startTime, uint32_t maxTime) {


        //calculate left time
        auto currentTime = std::chrono::steady_clock::now();
        int64_t spentTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
        int64_t timeLeft = maxTime - spentTime;
        std::cout << "\nTime left before " << int(iterationDepth) << " depth is " << timeLeft << std::endl;

        //calculate estimated Time by getting basis for exp-function of previous iterations

        if (iterationDepth - 1 != 0) {
            double basis = std::pow(spentTime / 0.07, (1.0 / (iterationDepth - 1)));
            std::cout << "Branching Faktor so far: \t" << basis << std::endl;
            double estimatedTime = std::pow(basis, iterationDepth);
            estimatedTime = estimatedTime * 0.07;
            std::cout << "Estimated time TEST: \t" << estimatedTime << std::endl;

            if (estimatedTime < timeLeft) {
                return true;
            }
            else {
                std::cout << "No Time left for next Iteration! " << estimatedTime << "\t" << timeLeft << std::endl;
                return false;
            }
        }
        else{
            std::cout << "Branching Faktor: 0 " << std::endl;
            return true;
        }
    }

        //calculate estimated Time for next iteration
        //int64_t estimatedTime = std::pow(branchingAverage, iterationDepth) * 0.02;
        //std::cout << "Estimated time for depth " << int(iterationDepth) << " is " << estimatedTime << std::endl;
        //if (estimatedTime < timeLeft){
        //    return true;
        //}
        //else {
        //    std::cout << "No Time left for next Iteration!" << std::endl;
        //    return false;
        //}

    bool checkForTimeEnd(uint32_t maxTime, const std::chrono::time_point<std::chrono::steady_clock> startTime) {

        auto currentTime = std::chrono::steady_clock::now();
        int64_t spentTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
        int64_t timeLeft = maxTime - spentTime;
        if(timeLeft > 5){
            return 1;
        }
        else{
            return 0;
        }
    }
} //namespace IterativeDeepening


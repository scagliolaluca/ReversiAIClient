#include "iterativeDeepening.h"

#include <math.h>
#include <iostream>


namespace IterativeDeepening
{

    void getMoveIterativeDeepening(uint8_t &x, uint8_t &y, uint8_t **board, uint8_t playerNumber, const std::function<int(uint8_t **, uint8_t)> &heuristic, const std::chrono::time_point<std::chrono::steady_clock> &stopTime){

        uint8_t iterationDepth = 1;
        uint8_t tempx;
        uint8_t tempy;
        bool miniMaxReachedMaxDepth = true;

        std::chrono::duration<double> lastIterationDuration(0);

        while(timeForNextIteration(iterationDepth, stopTime, lastIterationDuration) && miniMaxReachedMaxDepth){
            x = tempx;
            y = tempy;

            auto iterationStart = std::chrono::steady_clock::now();
            // If Minimax returns true --> continueCalculation
            if(Minimax::getMoveMinimax(tempx, tempy, miniMaxReachedMaxDepth, board, playerNumber, iterationDepth, heuristic, stopTime)){
                std::cout << "Found x,y: " << int(tempx) << int(tempy) << " in depth " << int(iterationDepth) << std::endl;
                x = tempx;
                y = tempy;
            }
            else{
                std::cout << "\nMinimax couldn't finish in time\n" << std::endl;
                /*
                //if set, this is the last move
                if(islastMove){
                    std::cout << "\nIt's the last move\n" << std::endl;
                    x = tempx;
                    y = tempy;
                }
                */
                return;
            }
            lastIterationDuration = std::chrono::steady_clock::now() - iterationStart;

            iterationDepth++;
        }
        return;
    }

    bool timeForNextIteration(uint8_t iterationDepth, const std::chrono::time_point<std::chrono::steady_clock> &stopTime, const std::chrono::duration<double> &lastIterationDuration) {

        //calculate left time
        auto currentTime = std::chrono::steady_clock::now();
        int64_t timeLeft = std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - currentTime).count();
        std::cout << "\nTime left before " << int(iterationDepth) << " depth is\t" << timeLeft << std::endl;

        //calculate estimated Time by getting average branching faktor of previous iterations
        if (iterationDepth != 1) {
            int64_t t_previous = std::chrono::duration_cast<std::chrono::nanoseconds>(lastIterationDuration).count();
            std::cout << "Previous duration: " << t_previous << "ns (" << (double)t_previous / 1000000 << "ms)" << std::endl;
            double basis = std::pow(t_previous, (1.0 / (iterationDepth - 1)));
            std::cout << "Branching Faktor so far: \t" << basis << std::endl;
            double estimatedTime = std::pow(basis, iterationDepth);
            estimatedTime = estimatedTime / 1000000;
            std::cout << "Estimated time TEST: \t" << estimatedTime << std::endl;

            // If enough time left
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

} //namespace IterativeDeepening


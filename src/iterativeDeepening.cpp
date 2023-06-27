#include "iterativeDeepening.h"

#include <math.h>
#include <iostream>


namespace IterativeDeepening
{

    void getMoveIterativeDeepening(uint8_t &x, uint8_t &y, uint8_t **board, uint8_t playerNumber, const std::function<float(uint8_t **, uint8_t)> &heuristic, const std::chrono::time_point<std::chrono::steady_clock> &stopTime){
        uint8_t iterationDepth = 1;
        uint8_t tempx;
        uint8_t tempy;
        bool miniMaxReachedMaxDepth = true;

        std::deque<std::chrono::duration<double>> iterationDurations;
        iterationDurations.push_front(std::chrono::duration<double>(0));

        while(timeForNextIteration(iterationDepth, stopTime, iterationDurations) && miniMaxReachedMaxDepth){
            auto iterationStart = std::chrono::steady_clock::now();
            // If Minimax returns true --> continueCalculation
            //if(Minimax::getMoveMinimax(tempx, tempy, miniMaxReachedMaxDepth, board, playerNumber, iterationDepth, heuristic, stopTime)){
            if(BRSP::getMoveBRSP(tempx, tempy, board, playerNumber, iterationDepth, heuristic, stopTime)){
                std::cout << "Found x,y: " << int(tempx) << int(tempy) << " in depth " << int(iterationDepth) << std::endl;
                x = tempx;
                y = tempy;
            }
            else{
                std::cout << "\nMinimax couldn't finish in time\n" << std::endl;
                // Only use move from failed iteration if timeout occurs on first iteration (no move returned yet)
                if (iterationDepth <= 1) {
                    x = tempx;
                    y = tempy;
                }
                return;
            }
            iterationDurations.push_front(std::chrono::steady_clock::now() - iterationStart);

            iterationDepth++;
        }
        return;
    }

    bool timeForNextIteration(uint8_t iterationDepth, const std::chrono::time_point<std::chrono::steady_clock> &stopTime, const std::deque<std::chrono::duration<double>> &iterationDurations) {
        //calculate left time
        auto currentTime = std::chrono::steady_clock::now();
        int64_t timeLeft = std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - currentTime).count();
        std::cout << "\nTime left before " << int(iterationDepth) << " depth is\t" << timeLeft << std::endl;

        std::cout << "Last iteration:\t" << iterationDurations[0].count() * 1000 << "ms" << std::endl;

        // Always returns true for first iteration (iterationDurations starts with one element)
        if (iterationDurations.size() < 2) {
            return true;
        }

        // Estimate the time required per node
        // TODO: average over multiple iterations / init from multiple calls to iterative deepening
        double estimatedTimePerNode = 0;
        if (iterationDurations.size() < 3 || iterationDurations[0].count() < iterationDurations[1].count() || iterationDurations[1].count() < iterationDurations[2].count()) {
            // Use default value (from experiments) for second iteration / inaccurate data for estimation
            estimatedTimePerNode = 5 * std::pow(10, -5);
        }
        else {
            double d = (double)(iterationDepth-1) / (iterationDepth-2);
            double a = std::pow(iterationDurations[1].count() - iterationDurations[2].count(), d);
            estimatedTimePerNode = std::pow(a / (iterationDurations[0].count() - iterationDurations[1].count()), 1 / (d-1));
        }
        std::cout << "EstimatedTimePerNode:\t" << estimatedTimePerNode << "sec" << std::endl;

        double estimatedBranchingFactor = std::pow((iterationDurations[0].count() - iterationDurations[1].count()) / estimatedTimePerNode, (double)1 / (iterationDepth-1));
        if (iterationDurations[0].count() < iterationDurations[1].count() || estimatedBranchingFactor < 1) {
            // Set branching factor to 1 if data for estimation not accurate
            estimatedBranchingFactor = 1;
        }
        std::cout << "EstimatedBranchingFactor:\t" << estimatedBranchingFactor << std::endl;

        double estimatedTime = estimatedTimePerNode * std::pow(estimatedBranchingFactor, iterationDepth) + iterationDurations[0].count();
        estimatedTime *= 1000;
        std::cout << "EstimatedTime:\t" << estimatedTime  << "ms"<< std::endl;

        // Safety buffer of 500ms
        if (estimatedTime + 500 < timeLeft) {
            std::cout << "Enough Time left for next Iteration! " << estimatedTime << " (+500ms)\t" << timeLeft << std::endl;
            return true;
        }
        else {
            std::cout << "No Time left for next Iteration! " << estimatedTime << "\t" << timeLeft << std::endl;
            return false;
        }
    }

} //namespace IterativeDeepening


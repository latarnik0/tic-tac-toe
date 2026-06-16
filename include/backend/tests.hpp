#ifndef TESTS_HPP
#define TESTS_HPP

#include <vector>
#include "/home/latarnik3/piaa2/include/backend/tictactoe.hpp"

void runPerformanceTests();

struct TestResult {
    double totalGameTimeMs;
    std::vector<double> timePerMoveMs;
    int totalMoves;
};

TestResult playAutoGame(int boardSize, int winCondition);

#endif 
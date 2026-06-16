#include "/home/latarnik3/piaa2/include/backend/tests.hpp"
#include "/home/latarnik3/piaa2/include/backend/tictactoe.hpp"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <algorithm>

TestResult playAutoGame(int boardSize, int winCondition) {
    TestResult result;
    result.totalGameTimeMs = 0;
    result.totalMoves = 0;

    Board board = makeBoard(boardSize, winCondition);
    PlayerSymbol currentPlayer = PlayerSymbol::Cross; 
    PlayerSymbol nextPlayer = PlayerSymbol::Circle;
    
    bool gameEnded = false;
    int max_depth = (boardSize > 3) ? 6 : 9;

    auto gameStart = std::chrono::high_resolution_clock::now();

    while (!gameEnded && result.totalMoves < (boardSize * boardSize)) {
        
        auto moveStart = std::chrono::high_resolution_clock::now();
    
        int bestMoveIndex = findBestMove(board, currentPlayer, nextPlayer, max_depth);
        
        auto moveEnd = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double, std::milli> moveDuration = moveEnd - moveStart;
        result.timePerMoveMs.push_back(moveDuration.count());

        makeMove(board, bestMoveIndex, currentPlayer);
        result.totalMoves++;
        
        PlayerSymbol winner = checkForWin(board, bestMoveIndex);
        if (winner != PlayerSymbol::Empty) {
            gameEnded = true;
        }

        // Zamiana graczy miejscami przed następną turą
        std::swap(currentPlayer, nextPlayer);
    }

    auto gameEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> gameDuration = gameEnd - gameStart;
    
    result.totalGameTimeMs = gameDuration.count();

    return result;
}

void runPerformanceTests() {
    std::cout << "--- TESTY WYDAJNOSCIOWE ---" << std::endl;

    std::vector<std::pair<int, int>> testCases = {
        {3, 3}, 
        {4, 4},
        {5, 5},
        {6, 6},
        {7, 7},
        {8, 8},
        {9, 9},
        {10, 10}
    };

    for (const auto& testCase : testCases) {
        int S = testCase.first;
        int condition = testCase.second;
        
        std::cout << "\nTest planszy: " << S << "x" << S << " (Warunek: " << condition << ")" << std::endl;
        
        TestResult singleRun = playAutoGame(S, condition);
        
        std::cout << "Czas calkowity partii: " << std::fixed << std::setprecision(2) << singleRun.totalGameTimeMs << " ms" << std::endl;
        std::cout << "Liczba ruchow w partii: " << singleRun.totalMoves << std::endl;
        
        std::cout << "Czas pierwszych 3 ruchow:" << std::endl;
        for (int i = 0; i < std::min(3, singleRun.totalMoves); ++i) {
            std::cout << " Ruch " << i+1 << ": " << singleRun.timePerMoveMs[i] << " ms" << std::endl;
        }
    }
    std::cout << "\n--- KONIEC TESTOW ---" << std::endl;
}
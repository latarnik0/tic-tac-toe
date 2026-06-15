#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <vector>

enum class GameMode { PlayerVsPlayer, PlayerVsAI };
enum class PlayerSymbol { Empty = 0, Cross = 1, Circle = 2 };


struct Board {
    std::vector<PlayerSymbol> vals;
    int S;
    int win_condition;
};

struct GameSetup {
    Board board;
    GameMode mode;
    PlayerSymbol aiSymbol;
    PlayerSymbol humanSymbol;
};


Board makeBoard(int n, int win_condition);

int radius(const Board& board, int move_index, PlayerSymbol player, int step, int col_step);

PlayerSymbol checkForWin(const Board& board, int move_index);

int makeMove(Board& board, int index, PlayerSymbol move);

bool isBoardFull(const Board& board);

int minimax(Board& board, int depth, bool isMaximizing, int alpha, int beta, int last_move_index, 
            int max_depth, PlayerSymbol aiSymbol, PlayerSymbol humanSymbol);

int findBestMove(Board& board, PlayerSymbol aiSymbol, PlayerSymbol humanSymbol, int max_depth);

#endif
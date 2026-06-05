#include "/include/backend/tictactoe.h"
#include <algorithm>

Board makeBoard(int n, int win_condition) {
    Board b;
    b.vals = std::vector<PlayerSymbol>(n * n, PlayerSymbol::Empty); 
    b.S = n;
    b.win_condition = win_condition;
    return b;
}

int radius(const Board& board, int move_index, PlayerSymbol player, int step, int col_step) {
    int result = 0;
    int current_index = move_index;
    
    while(true) {
        int next_index = current_index + step; 

        if(next_index < 0 || next_index >= board.vals.size()) {
            break; 
        }
        
        int current_col = current_index % board.S;
        int next_col = next_index % board.S;
        
        if(next_col - current_col != col_step) {
            break;
        }
        
        if(board.vals[next_index] != player) {
            break;
        }

        result++; 
        current_index = next_index;
    }
    
    return result;
}

PlayerSymbol checkForWin(const Board& board, int move_index) {
    PlayerSymbol player = board.vals[move_index];

    if (player == PlayerSymbol::Empty) return PlayerSymbol::Empty; 

    // POZIOM
    int hor = 1 + radius(board, move_index, player, -1, -1) + radius(board, move_index, player, 1, 1);
    if(hor >= board.win_condition) return player;
    
    // PION
    int ver = 1 + radius(board, move_index, player, -board.S, 0) + radius(board, move_index, player, board.S, 0);
    if(ver >= board.win_condition) return player;

    // PRZEKĄTNA lewa gora-->prawy dol
    int diag1 = 1 + radius(board, move_index, player, -board.S - 1, -1) + radius(board, move_index, player, board.S + 1, 1);
    if(diag1 >= board.win_condition) return player;

    // PRZEKĄTNA lewy dol-->prawa gora
    int diag2 = 1 + radius(board, move_index, player, -board.S + 1, 1) + radius(board, move_index, player, board.S - 1, -1);
    if(diag2 >= board.win_condition) return player;

    return PlayerSymbol::Empty;
}

int makeMove(Board& board, int index, PlayerSymbol move) {
    // Kody błędów zamiast std::cout
    if(index < 0 || index >= board.S * board.S) {
        return -1; // Nielegalny ruch (poza planszą)
    }

    if(board.vals[index] != PlayerSymbol::Empty) {
       return -2; // Pole zajęte
    }

    // Wykonanie ruchu
    board.vals[index] = move;

    // Sprawdzenie wygranej
    PlayerSymbol winner = checkForWin(board, index);
    
    if(winner != PlayerSymbol::Empty) {
        return static_cast<int>(winner);
    }
    
    return 0; // Brak wygranej, gra toczy się dalej
}

bool isBoardFull(const Board& board) {
    for (PlayerSymbol val : board.vals) {
        if (val == PlayerSymbol::Empty) return false;
    }
    return true;
}

int minimax(Board& board, int depth, bool isMaximizing, int alpha, int beta, int last_move_index, 
            int max_depth, PlayerSymbol aiSymbol, PlayerSymbol humanSymbol) {
    
    if (last_move_index != -1) {
        PlayerSymbol winner = checkForWin(board, last_move_index);
        
        if (winner == aiSymbol) return 10 - depth;     
        if (winner == humanSymbol) return depth - 10;   
    }

    if (isBoardFull(board) || depth == max_depth) {
        return 0; 
    }

    if (isMaximizing) {
        int bestScore = -1000;
        for (size_t i = 0; i < board.vals.size(); i++) {
            if (board.vals[i] == PlayerSymbol::Empty) {
                board.vals[i] = aiSymbol;
                int score = minimax(board, depth + 1, false, alpha, beta, i, max_depth, aiSymbol, humanSymbol);
                board.vals[i] = PlayerSymbol::Empty; 
                
                bestScore = std::max(score, bestScore);
                alpha = std::max(alpha, bestScore);
                if (beta <= alpha) break; 
            }
        }
        return bestScore;
    } else {
        int bestScore = 1000;
        for (size_t i = 0; i < board.vals.size(); i++) {
            if (board.vals[i] == PlayerSymbol::Empty) {
                board.vals[i] = humanSymbol; 
                int score = minimax(board, depth + 1, true, alpha, beta, i, max_depth, aiSymbol, humanSymbol);
                board.vals[i] = PlayerSymbol::Empty;
                
                bestScore = std::min(score, bestScore);
                beta = std::min(beta, bestScore);
                if (beta <= alpha) break;
            }
        }
        return bestScore;
    }
}

int findBestMove(Board& board, PlayerSymbol aiSymbol, PlayerSymbol humanSymbol, int max_depth) {
    int bestScore = -1000;
    int bestMove = -1;
    
    for (size_t i = 0; i < board.vals.size(); i++) {
        if (board.vals[i] == PlayerSymbol::Empty) {
            board.vals[i] = aiSymbol; 
            int score = minimax(board, 0, false, -1000, 1000, i, max_depth, aiSymbol, humanSymbol);
            board.vals[i] = PlayerSymbol::Empty; 
            
            if (score > bestScore) {
                bestScore = score;
                bestMove = i;
            }
        }
    }
    return bestMove;
}
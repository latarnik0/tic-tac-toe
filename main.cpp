#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>

/*
0 - puste miejsce
1 - gracz 1 np. kółko
2 - gracz 2 np. krzyżyk
*/

/*

==== TODO ====
- rozdzielic na plki nagłówkowe
- algorytm minmax

*/

struct Board {
    std::vector<int> vals;
    int S;
    int win_condition;
};

Board makeBoard(int n, int win_condition) {
    Board b;
    b.vals = std::vector<int>(n * n, 0); 
    b.S = n;
    b.win_condition = win_condition;
    return b;
}

int radius(const Board& board, int move_index, int player, int step, int col_step) {
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


int checkForWin(const Board& board, int move_index) {
    int player = board.vals[move_index];

    if (player == 0) return 0; 

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

    // No winner yet
    return 0;
}

void makeMove(Board& board, int index, int move) {

    if(move != 1 && move != 2){
        throw std::invalid_argument("Illegal move");
    }

    if(board.vals[index] == 0){
        board.vals[index] = move;
    }
    else if(board.vals[index] == 1 || board.vals[index] == 2){
        throw std::invalid_argument("Spot already taken");
    }
    else{
        std::cout << "Something weird is here: " << board.vals[index] << std::endl;
    }

    int winner = checkForWin(board, index);
    
    if(winner != 0){
        std::cout << "Winner: " << winner << std::endl;
    }
    else{
        std::cout << "No winner yet" << std::endl;
    }
}

int main() {
    Board myBoard = makeBoard(3, 3);
    
    makeMove(myBoard, 0, 1);
    makeMove(myBoard, 1, 1);
    makeMove(myBoard, 2, 1); 
    return 0;
}
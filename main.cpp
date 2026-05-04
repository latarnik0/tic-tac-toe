#include <iostream>
#include <vector>
#include <cmath>

/*
0 - wolne miejsce
1 - miejsce zajęte przez kółko
2 - miejsce zajęte przez krzyżyk
*/

/*
[0  1   2]
[3  4   5]
[6  7   8]
*/
std::vector<int> makeBoard(int n){
    std::vector<int> Board = {};
    for(int i=0; i<n*n; ++i){
        Board.push_back(0);
    }
    return Board;
}

int checkForWinHor(const std::vector<int>& Board, int move_index, int S) {
    int player = Board[move_index];

    if (player == 0) return 0; 

    int counter = 1; 
    
    // w którym wierszu został wykonany ruch.
    int row = move_index / S; 

    // W LEWO
    int current_index_left = move_index - 1;
    
    while(current_index_left >= 0 && (current_index_left / S) == row && Board[current_index_left] == player){
        counter++;
        current_index_left--; // dalej w lewo
    }

    // W PRAWO
    int current_index_right = move_index + 1;
    
    while(current_index_right < Board.size() && (current_index_right / S) == row && Board[current_index_right] == player){
        counter++;
        current_index_right++; // dalej w prawo
    }

    if(counter >= S){
        return player;
    }

    return 0;
}

int checkForWinVer(const std::vector<int>& Board, int move_index, int S) {
    int player = Board[move_index];

    if (player == 0) return 0; 

    int counter = 1; 

    // W GÓRĘ
    int current_index_up = move_index - S;
    
    while(current_index_up >= 0 && Board[current_index_up] == player){
        counter++;
        current_index_up -= S;
    }

    // W DÓŁ
    int current_index_down = move_index + S;
    
    while(current_index_down < Board.size() && Board[current_index_down] == player){
        counter++;
        current_index_down += S;
    }

    if(counter >= S){
        return player;
    }

    return 0;
}

void makeMove(std::vector<int>& Board, int index, int move){

    if(move != 1 && move != 2){
        throw std::invalid_argument("Illegal move");
    }

    if(Board[index] == 0){
        Board[index] = move;
    }
    else if(Board[index] == 1 || Board[index] == 2){
        throw std::invalid_argument("Spot already taken");
    }
    else{
        std::cout<<"Something weird is here: "<<Board[index]<<std::endl;
    }

    // tutaj sprawdzanie wygranej zaraz po wykonaniu ruchu
}

int main(){
    std::vector<int> Board = makeBoard(3);
    makeMove(Board, 0, 1);
    makeMove(Board, 1, 1);
    makeMove(Board, 3, 2);
    makeMove(Board, 2, 1);
}


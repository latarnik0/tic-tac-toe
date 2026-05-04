#include <iostream>
#include <vector>
#include <cmath>

/*
0 - wolne miejsce
1 - miejsce zajęte przez kółko
2 - miejsce zajęte przez krzyżyk
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
    int current_indexl = move_index - 1;
    
    while(current_indexl >= 0 && (current_indexl / S) == row && Board[current_indexl] == player){
        counter++;
        current_indexl--; // dalej w lewo
    }

    // W PRAWO
    int current_index = move_index + 1;
    
    while(current_index < Board.size() && (current_index / S) == row && Board[current_index] == player){
        counter++;
        current_index++; // dalej w prawo
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

    int winner = checkForWinHor(Board, index, sqrt(Board.size()));
    if(winner != 0){
        std::cout<<"Winner: "<<winner<<std::endl;
        return;
    }
    else{
        std::cout<<"No winner yet"<<std::endl;
    }
}

int main(){
    std::vector<int> Board = makeBoard(3);
    makeMove(Board, 0, 1);
    makeMove(Board, 1, 1);
    makeMove(Board, 3, 2);
    makeMove(Board, 2, 1);
}


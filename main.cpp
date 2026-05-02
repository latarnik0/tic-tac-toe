#include <iostream>
#include <vector>
#include <cmath>

/*
0 - wolne miejsce
1 - miejsce zajęte przez kółko
2 - miejsce zajęte przez krzyżyk
*/

/*
[0 1 2 3 4 5 6 7 8]
[0  1   2]
[3  4   5]
[6  7   8]
*/

int vertIndex(int k, int m, int S){
    // S = sqrt(rozmiar_planszy)
    return k + m*S;
}

std::vector<int> makeBoard(int n){
    std::vector<int> Board = {};
    for(int i=0; i<n*n; ++i){
        Board.push_back(0);
    }
    return Board;
}

void checkForWinVer(std::vector<int>& Board){
    // TODO
}

void checkForWinHor(std::vector<int>& Board){
    // TODO
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
    checkForWinHor(Board);
}

int main(){
    std::vector<int> Board = makeBoard(3);
    makeMove(Board, 0, 1);
    makeMove(Board, 1, 2);
    makeMove(Board, 2, 1);
    makeMove(Board, 3, 2);
}


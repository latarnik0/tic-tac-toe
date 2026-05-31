#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>


enum class GameMode { PlayerVsPlayer, PlayerVsAI };
enum class PlayerSymbol { Empty = 0, Circle = 2, Cross = 1 };

struct Board {
    std::vector<PlayerSymbol> vals;
    int S;
    int win_condition;
};

Board makeBoard(int n, int win_condition) {
    Board b;
    b.vals = std::vector<PlayerSymbol>(n * n, PlayerSymbol::Empty); 
    b.S = n;
    b.win_condition = win_condition;
    return b;
}

struct GameSetup {
    Board board;
    GameMode mode;
    PlayerSymbol aiSymbol;
    PlayerSymbol humanSymbol;
};

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

    // No winner yet
    return PlayerSymbol::Empty;
}

void makeMove(Board& board, int index, PlayerSymbol move) {

    if(move != PlayerSymbol::Cross && move != PlayerSymbol::Circle){
        std::cout<<"Illegal move";
    }

    if(board.vals[index] == PlayerSymbol::Cross || board.vals[index] == PlayerSymbol::Circle){
       std::cout<<"Spot already taken";
    }

    if(board.vals[index] == PlayerSymbol::Empty){
        board.vals[index] = move;
    }

    PlayerSymbol winner = checkForWin(board, index);
    
    if(winner != PlayerSymbol::Empty){
        std::cout << "Winner: " << int(winner) << std::endl;
    }
    else{
        std::cout << "No winner yet" << std::endl;
    }
}


bool isBoardFull(const Board& board) {
    for (PlayerSymbol val : board.vals) {
        if (val == PlayerSymbol::Empty) return false;
    }
    return true;
}

// Główna funkcja rekurencyjna Minimax z cięciami Alfa-Beta
int minimax(Board& board, int depth, bool isMaximizing, int alpha, int beta, int last_move_index, 
    int max_depth, PlayerSymbol aiSymbol, PlayerSymbol humanSymbol) {
    // Sprawdź czy poprzedni ruch zakończył grę
    if (last_move_index != -1) {
        PlayerSymbol winner = checkForWin(board, last_move_index);
        // Zwracamy wartości uwzględniające głębokość, aby AI wolało wygrać szybciej lub przegrać później
        if (winner == PlayerSymbol::Cross) return 10 - depth;      // Wygrana AI
        if (winner == PlayerSymbol::Circle) return depth - 10;      // Wygrana Gracza  
    }

    // Sprawdź czy jest remis lub osiągnięto limit głębokości przeszukiwania
    if (isBoardFull(board) || depth == max_depth) {
        return 0; 
    }

    // szuka najwyższego wyniku
    if (isMaximizing) {
        int bestScore = -1000;
        for (int i = 0; i < board.vals.size(); i++) {
            if (board.vals[i] == PlayerSymbol::Empty) {
                board.vals[i] = aiSymbol; // Symuluj ruch
                int score = minimax(board, depth + 1, false, alpha, beta, i, max_depth, aiSymbol, humanSymbol);
                board.vals[i] = PlayerSymbol::Empty; // Cofnij ruch po symulacji
                
                bestScore = std::max(score, bestScore);
                alpha = std::max(alpha, bestScore);
                if (beta <= alpha) break; // Cięcie Alfa-Beta 
            }
        }
        return bestScore;
    } 
    // szuka najniższego wyniku 
    else {
        int bestScore = 1000;
        for (int i = 0; i < board.vals.size(); i++) {
            if (board.vals[i] == PlayerSymbol::Empty) {
                board.vals[i] = humanSymbol; // Symuluj ruch 
                int score = minimax(board, depth + 1, true, alpha, beta, i, max_depth, aiSymbol, humanSymbol);
                board.vals[i] = PlayerSymbol::Empty; // Cofnij ruch po symulacji
                
                bestScore = std::min(score, bestScore);
                beta = std::min(beta, bestScore);
                if (beta <= alpha) break; // Cięcie Alfa-Beta
            }
        }
        return bestScore;
    }
}

// Funkcja zwracająca najlepszy indeks dla ruchu AI
int findBestMove(Board& board, PlayerSymbol aiSymbol, PlayerSymbol humanSybol, int max_depth) {
    int bestScore = -1000;
    int bestMove = -1;
    
    for (int i = 0; i < board.vals.size(); i++) {
        if (board.vals[i] == PlayerSymbol::Empty) {
            board.vals[i] = aiSymbol; // Sprawdź ten ruch dla AI
            // Wywołaj minimax zakładając, że teraz tura minimalizującego Gracza 1
            int score = minimax(board, 0, false, -1000, 1000, i, max_depth, aiSymbol, humanSybol);
            board.vals[i] = PlayerSymbol::Empty; // Cofnij ruch
            
            if (score > bestScore) {
                bestScore = score;
                bestMove = i;
            }
        }
    }
    return bestMove;
}


GameSetup getConfig(){

    GameSetup currentSetup;
    bool settingMode = true, settingSymbol = true, settingBoard = true;

    std::cout<<"============ TIC-TAC-TOE GAME =========="<<std::endl;

    while(settingMode){
        int gmode;
        std::cout<<"Choose game mode: (1) Player-Vs-Player, (2) Player-Vs-AI"<<std::endl;
        std::cin>>gmode;

        if(gmode == 1) {
            currentSetup.mode = GameMode::PlayerVsPlayer;
            settingMode = false;
            break;
        } else if(gmode == 2) {
            currentSetup.mode = GameMode::PlayerVsAI;
            settingMode = false;
            break;
        } else {
            std::cout<<"Invalid mode! Try again."<<std::endl;
        }
    }

    while(settingSymbol && !settingMode){
        int psymbol;
        std::cout<<"Choose your symbol (Cross starts first) : (1) Cross, (2) Circle"<<std::endl;
        std::cin>>psymbol;

        if(psymbol == 1) {
            currentSetup.humanSymbol = PlayerSymbol::Cross;
            currentSetup.aiSymbol = PlayerSymbol::Circle;
            settingSymbol = false;
            break;
        } else if(psymbol == 2) {
            currentSetup.humanSymbol = PlayerSymbol::Circle;
            currentSetup.aiSymbol = PlayerSymbol::Cross;
            settingSymbol = false;
            break;
        } else {
            std::cout<<"Invalid symbol! Try again."<<std::endl;
        }
    }

    while(settingBoard && !settingSymbol && !settingMode){
        int boardS;
        int winC;
        std::cout<<"Define board size and win condition (in that order!): "<<std::endl;
        std::cin>>boardS>>winC;

        if(boardS < 0 || winC < 0){
            std::cout<<"Board and win condition must be positive integers! Try again."<<std::endl;
        }

        if(winC > boardS) {
            std::cout<<"Win condition cannot be larger than board size! Try again."<<std::endl;
        }

        if(boardS > 0 && winC > 0 && winC <= boardS){
            currentSetup.board = makeBoard(boardS, winC);
            settingBoard = false;
        }
    }
    return currentSetup;
}

int main() {
    GameSetup s = getConfig();
    int mindex;

    if(s.humanSymbol == PlayerSymbol::Cross){
        // player moves first
        std::cout<<"Make your move: ";
        std::cin>>mindex;
        makeMove(s.board, mindex, s.humanSymbol);

        while(1){
            // AI moves
            mindex = findBestMove(s.board, s.aiSymbol, s.humanSymbol, 6);
            makeMove(s.board, mindex, s.aiSymbol);
            std::cout<<"AI made a move on index "<<mindex<<std::endl;
            
            // player moves
            std::cout<<"Make your move: ";
            std::cin>>mindex;
            makeMove(s.board, mindex, s.humanSymbol);
        }
    }
    else{
        // AI moves first
        mindex = findBestMove(s.board, s.aiSymbol, s.humanSymbol, 6);
        makeMove(s.board, mindex, s.aiSymbol);
        std::cout<<"AI made a move on index "<<mindex<<std::endl;

        while(1){
            // player moves
            std::cout<<"Make your move: ";
            std::cin>>mindex;
            makeMove(s.board, mindex, s.humanSymbol);
            
            // AI moves
            mindex = findBestMove(s.board, s.aiSymbol, s.humanSymbol, 6);
            makeMove(s.board, mindex, s.aiSymbol);
            std::cout<<"AI made a move on index "<<mindex<<std::endl;
        }
    }
    return 0;
}
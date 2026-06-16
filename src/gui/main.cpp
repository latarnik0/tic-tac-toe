#include <raylib.h>
#include "/home/latarnik3/piaa2/include/backend/tictactoe.hpp"
#include "/home/latarnik3/piaa2/include/backend/tests.hpp"
#include <string>
#include <algorithm>
#include <chrono>
#include <iostream>

enum class GameState { MENU, GAME, GAME_OVER };


int main() {
    
    const int screenWidth = 800;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Kółko i Krzyżyk - Raylib");
    SetTargetFPS(60);

    GameState state = GameState::MENU;
    int boardSize = 3;
    int winCondition = 3;
    GameMode currentMode = GameMode::PlayerVsAI;
    PlayerSymbol humanSymbol = PlayerSymbol::Cross;
    
    GameSetup setup;
    PlayerSymbol currentTurn = PlayerSymbol::Cross; // Krzyżyk zawsze zaczyna
    int matchResult = 0; // 1 = Krzyżyk, 2 = Kółko, 3 = Remis
    bool first_move = true;

    // --- Definicje przycisków ---
    Rectangle btnSizeMinus  = { 250, 150, 40, 40 };
    Rectangle btnSizePlus   = { 500, 150, 40, 40 };
    Rectangle btnWinMinus   = { 250, 250, 40, 40 };
    Rectangle btnWinPlus    = { 500, 250, 40, 40 };
    Rectangle btnMode       = { 250, 350, 290, 40 };
    Rectangle btnSymbol     = { 250, 450, 290, 40 };
    Rectangle btnStart      = { 300, 550, 200, 60 };
    Rectangle btnRestart    = { 300, 500, 200, 60 };

    auto start = std::chrono::high_resolution_clock::now();

    while (!WindowShouldClose()) {
        Vector2 mousePoint = GetMousePosition();
        bool clicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

        //LOGIKA 
        if (state == GameState::MENU) {
            if (clicked) {
                if (CheckCollisionPointRec(mousePoint, btnSizeMinus) && boardSize > 3) boardSize--;
                if (CheckCollisionPointRec(mousePoint, btnSizePlus) && boardSize < 10) boardSize++;
            
                if (CheckCollisionPointRec(mousePoint, btnWinMinus) && winCondition > 3) winCondition--;
                if (CheckCollisionPointRec(mousePoint, btnWinPlus) && winCondition < boardSize) winCondition++;
                if (winCondition > boardSize) winCondition = boardSize;

                if (CheckCollisionPointRec(mousePoint, btnMode)) {
                    currentMode = (currentMode == GameMode::PlayerVsAI) ? GameMode::PlayerVsPlayer : GameMode::PlayerVsAI;
                }

                if (currentMode == GameMode::PlayerVsAI && CheckCollisionPointRec(mousePoint, btnSymbol)) {
                    humanSymbol = (humanSymbol == PlayerSymbol::Cross) ? PlayerSymbol::Circle : PlayerSymbol::Cross;
                }

                if (CheckCollisionPointRec(mousePoint, btnStart)) {
                    setup.board = makeBoard(boardSize, winCondition);
                    setup.mode = currentMode;
                    setup.humanSymbol = humanSymbol;
                    //setup.aiSymbol = (humanSymbol == PlayerSymbol::Cross) ? PlayerSymbol::Circle : PlayerSymbol::Cross;
                    
                    if(setup.humanSymbol == PlayerSymbol::Cross){
                        setup.aiSymbol = PlayerSymbol::Circle;
                    }
                    else{
                        setup.aiSymbol = PlayerSymbol::Cross;
                    }
                    
                    currentTurn = PlayerSymbol::Cross; // Krzyżyk zawsze zaczyna
                    matchResult = 0;
                    state = GameState::GAME;
                }
            }
        } 
        else if (state == GameState::GAME) {
            // TURA AI jako krzyzyk
            if(setup.mode == GameMode::PlayerVsAI && setup.aiSymbol == PlayerSymbol::Cross && first_move == true){
                int maxDepth = (boardSize > 3) ? 6 : 9; 

                int moveIndex = findBestMove(setup.board, setup.aiSymbol, setup.humanSymbol, maxDepth);
                int tmp = makeMove(setup.board, moveIndex, setup.aiSymbol);
                
                first_move = false;
                currentTurn = setup.humanSymbol; // Przekazanie tury
            }

            // TURA AI kolejna
            if(setup.mode == GameMode::PlayerVsAI && currentTurn == setup.aiSymbol && first_move == false) {
                int maxDepth = boardSize; 

                int moveIndex = findBestMove(setup.board, setup.aiSymbol, setup.humanSymbol, maxDepth);
                int tmp = makeMove(setup.board, moveIndex, setup.aiSymbol);
                
                if (tmp > 0) {
                    matchResult = static_cast<int>(setup.aiSymbol); // AI wygrywa
                    state = GameState::GAME_OVER;
                } else if (isBoardFull(setup.board)) {
                    matchResult = 3; // Remis
                    state = GameState::GAME_OVER;
                } else {
                    currentTurn = setup.humanSymbol; // Przekazanie tury
                }
            } 

            // TURA GRACZA pierwsza jako krzyzyk
            if(setup.mode == GameMode::PlayerVsAI && setup.humanSymbol == PlayerSymbol::Cross && first_move == true){
                if (clicked) {
                    float cellSize = (float)screenWidth / setup.board.S;
                    int col = GetMouseX() / cellSize;
                    int row = GetMouseY() / cellSize;
                    int index = row * setup.board.S + col;

                    int tmp = makeMove(setup.board, index, currentTurn);
                }
                
                first_move = false;
                currentTurn = setup.aiSymbol; // Przekazanie tury
            }

            // TURA GRACZA kolejna
            if(setup.mode == GameMode::PlayerVsAI && currentTurn == setup.humanSymbol && first_move == false){
                if (clicked) {
                    float cellSize = (float)screenWidth / setup.board.S;
                    int col = GetMouseX() / cellSize;
                    int row = GetMouseY() / cellSize;
                    int index = row * setup.board.S + col;

                    int tmp = makeMove(setup.board, index, currentTurn);
                    
                    if (tmp >= 0) { 
                        if (tmp > 0) {
                            matchResult = static_cast<int>(currentTurn); // Obecny gracz wygrywa
                            state = GameState::GAME_OVER;
                        } else if (isBoardFull(setup.board)) {
                            matchResult = 3; // Remis
                            state = GameState::GAME_OVER;
                        } else {
                            // Zmiana tury na przeciwną
                            currentTurn = (currentTurn == PlayerSymbol::Cross) ? PlayerSymbol::Circle : PlayerSymbol::Cross;
                        }
                    }
                }
            }
        }
        else if (state == GameState::GAME_OVER) {
            if (clicked && CheckCollisionPointRec(mousePoint, btnRestart)) {
                state = GameState::MENU;
            }
        }

        //GRAFIKA
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (state == GameState::MENU) {
            DrawText("USTAWIENIA GRY", 260, 50, 30, DARKGRAY);

            DrawText("Rozmiar planszy:", 300, 120, 20, DARKGRAY);
            DrawRectangleRec(btnSizeMinus, LIGHTGRAY); DrawText("-", 265, 155, 30, BLACK);
            DrawRectangleRec(btnSizePlus, LIGHTGRAY);  DrawText("+", 512, 155, 30, BLACK);
            DrawText(TextFormat("%d x %d", boardSize, boardSize), 350, 155, 30, MAROON);

            DrawText("Warunek wygranej:", 295, 220, 20, DARKGRAY);
            DrawRectangleRec(btnWinMinus, LIGHTGRAY); DrawText("-", 265, 255, 30, BLACK);
            DrawRectangleRec(btnWinPlus, LIGHTGRAY);  DrawText("+", 512, 255, 30, BLACK);
            DrawText(TextFormat("%d", winCondition), 390, 255, 30, MAROON);

            DrawText("Tryb gry:", 355, 320, 20, DARKGRAY);
            DrawRectangleRec(btnMode, LIGHTGRAY);
            const char* modeText = (currentMode == GameMode::PlayerVsAI) ? "Gracz vs AI" : "Gracz vs Gracz";
            DrawText(modeText, 320, 360, 20, BLACK);

            if (currentMode == GameMode::PlayerVsAI) {
                DrawText("Grasz jako (Krzyzyk zaczyna):", 240, 420, 20, DARKGRAY);
                DrawRectangleRec(btnSymbol, LIGHTGRAY);
                const char* symbolText = (humanSymbol == PlayerSymbol::Cross) ? "KRZYZYK (X)" : "KOLKO (O)";
                DrawText(symbolText, 330, 460, 20, BLACK);
            }

            DrawRectangleRec(btnStart, DARKGREEN);
            DrawText("START", 355, 565, 30, WHITE);
        }
        else if (state == GameState::GAME || state == GameState::GAME_OVER) {
            // Rysowanie siatki
            float cellSize = (float)screenWidth / setup.board.S;
            for (int i = 1; i < setup.board.S; i++) {
                DrawLineEx({ i * cellSize, 0 }, { i * cellSize, (float)screenHeight }, 4.0f, DARKGRAY);
                DrawLineEx({ 0, i * cellSize }, { (float)screenWidth, i * cellSize }, 4.0f, DARKGRAY);
            }

            // Rysowanie kółek i krzyżyków
            for (int i = 0; i < setup.board.vals.size(); i++) {
                if (setup.board.vals[i] != PlayerSymbol::Empty) {
                    int col = i % setup.board.S;
                    int row = i / setup.board.S;
                    
                    float centerX = col * cellSize + cellSize / 2.0f;
                    float centerY = row * cellSize + cellSize / 2.0f;
                    float padding = cellSize * 0.2f;

                    if (setup.board.vals[i] == PlayerSymbol::Cross) {
                        DrawLineEx({ centerX - cellSize/2 + padding, centerY - cellSize/2 + padding }, 
                                   { centerX + cellSize/2 - padding, centerY + cellSize/2 - padding }, 10.0f, RED);
                        DrawLineEx({ centerX + cellSize/2 - padding, centerY - cellSize/2 + padding }, 
                                   { centerX - cellSize/2 + padding, centerY + cellSize/2 - padding }, 10.0f, RED);
                    } else if (setup.board.vals[i] == PlayerSymbol::Circle) {
                        DrawCircleLines(centerX, centerY, cellSize / 2.0f - padding, BLUE);
                        // Pogrubienie kółka
                        DrawCircleLines(centerX, centerY, cellSize / 2.0f - padding - 1, BLUE);
                        DrawCircleLines(centerX, centerY, cellSize / 2.0f - padding + 1, BLUE);
                    }
                }
            }

            // Nakładka GAME OVER
            if (state == GameState::GAME_OVER) {
                DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.8f));
                
                const char* endMsg = "";
                if (matchResult == 3) endMsg = "REMIS!";
                else if (matchResult == 1) endMsg = "WYGRYWA KRZYZYK!";
                else if (matchResult == 2) endMsg = "WYGRYWA KOLKO!";

                DrawText(endMsg, screenWidth/2 - MeasureText(endMsg, 50)/2, 300, 50, WHITE);

                DrawRectangleRec(btnRestart, DARKBLUE);
                DrawText("MENU", 355, 515, 30, WHITE);

            }
        }
        EndDrawing();
    }

    CloseWindow();
    
    //runPerformanceTests();
    return 0;
}
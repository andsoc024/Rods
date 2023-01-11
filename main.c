// ============================================================================
// RODS
// Main
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    The main function of the program.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>


// ============================================================================ MAIN

// The main function of the programn
int main(__attribute__((unused)) int argc, __attribute__((unused)) char** argv){
    const int winW = 800;
    const int winH = 600;

    InitWindow(winW, winH, "Test");
    SetTargetFPS(60);

    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Testing...", 10, 10, 80, RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
}


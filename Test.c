// ============================================================================
// RODS
// Test
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Function for testing and debugging. Compiled and executed only when in 
    DEBUG_MODE.
*/


// ============================================================================ DEPENDENCIES

// Included in main.c, only in DEBUG_MODE

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "Mods/Public/Public.h"
#include "Mods/Fund/Fund.h"


// ============================================================================ TEST

// Function for testing and debugging
int Test(UNUSED int argc, UNUSED char** argv){

    Window_PrintAppInfo();

    Window_Init();

    while (!WindowShouldClose()){
        if (IsWindowResized()){
            Window_UpdateWinSize();
            Window_PrintSize(WITHOUT_NEW_LINE);
            printf(" ");
            Geo_PrintSize(Glo_WinSize, WITH_NEW_LINE);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        EndDrawing();
    }

    Window_Close();

    return 0;
}



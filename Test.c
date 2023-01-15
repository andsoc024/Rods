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
#include "Mods/Logic/Logic.h"
#include "Mods/Graph/Graph.h"


// ============================================================================ TEST

// Function for testing and debugging
int Test(UNUSED int argc, UNUSED char** argv){

    Window_Init();

    FRects* fRects = FRects_Make();

    FRects_Print(fRects);

    while (!WindowShouldClose()){
        if (IsWindowResized()){
            Window_UpdateWinSize();
            FRects_Resize(fRects);
        }

        FRects_Update(fRects);

        BeginDrawing();
        ClearBackground(COL_BG);
        FRects_Draw(fRects);
        EndDrawing();
    }

    fRects = FRects_Free(fRects);
    Window_Close();

    return 0;
}



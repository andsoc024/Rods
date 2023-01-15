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

    Point center = Geo_RectPoint(TO_RECT(Glo_WinSize), RP_CENTER);
    float radius = MIN_DIM(Glo_WinSize) * 0.45f;

    MCol_MakeDefault();

    while (!WindowShouldClose()){
        if (IsWindowResized()){
            Window_UpdateWinSize();
            center = Geo_RectPoint(TO_RECT(Glo_WinSize), RP_CENTER);
            radius = MIN_DIM(Glo_WinSize) * 0.45f;
        }

        MCol_Update(Glo_MCol);

        BeginDrawing();
        ClearBackground(COL_BG);
        DrawCircleV(center, radius, MCol(Glo_MCol));
        EndDrawing();
    }

    MCol_FreeDefault();
    CloseWindow();

    return 0;
}



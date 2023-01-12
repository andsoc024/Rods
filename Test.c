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

    InitWindow(100, 100, "Dir Test");
    SetTargetFPS(60);

    while (!WindowShouldClose()){
        KeyboardKey key = GetKeyPressed();
        E_Direction dir = Direction_FromKey(key);
        if (dir != DIR_NONE){
            E_Direction opp = Direction_Opposite(dir);
            printf("%s %s\n", Direction_ToString(dir, SHORT_FORM), 
                              Direction_ToString(opp, LONG_FORM));
        }

        BeginDrawing();
        ClearBackground(COL_NULL);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}



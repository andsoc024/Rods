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

    const Grid grid = GRID0(25, 15);
    Size tileSize = Grid_CalcCellSize(grid, Glo_WinSize);

    Color** colors = NULL;
    MAKE_2D_ARR(colors, grid.nCols, grid.nRows, Color, ZEROVAL_ALL)

    for (int y = 0; y < grid.nRows; y++){
        Color baseColor = Color_Random(0x33, 0xEE);
        for (int x = 0; x < grid.nCols; x++){
            colors[x][y] = Color_ChangeBrightness(baseColor, x - grid.nCols / 2);
        }
    }

    while (!WindowShouldClose()){
        if (IsWindowResized()){
            Window_UpdateWinSize();
            tileSize = Grid_CalcCellSize(grid, Glo_WinSize);
        }

        BeginDrawing();
        ClearBackground(COL_BG);
        Point cursor = POINT_NULL;
        for (int y = 0; y < grid.nCols; y++){
            for (int x = 0; x < grid.nCols; x++){
                DrawRectangleRec(RECT(cursor.x, cursor.y, tileSize.width, tileSize.height), colors[x][y]);
                cursor.x += tileSize.width;
            }
            cursor.x = 0;
            cursor.y += tileSize.height;
        }
        EndDrawing();
    }

    FREE_2D_ARR(colors, grid.nCols)
    CloseWindow();

    return 0;
}



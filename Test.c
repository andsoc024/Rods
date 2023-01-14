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

    Size winSize = SIZE(800, 600);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(winSize.width, winSize.height, "Test");
    SetTargetFPS(60);

    const Grid totalGrid = GRID0(30, 20);
    const float vCellSize = 50.0f;
    const Size vScreen = Grid_ToSize(totalGrid, vCellSize);

    const float winMargin = 50.0f;
    Rect rScreen = Geo_ProjectSizeInSize(vScreen, winSize, winMargin);

    float scaleF = rScreen.width / vScreen.width;
    Point origin = RORIGIN(rScreen);

    Point mousePos = GetMousePosition();
    const Size cursorRectSize = SIZE(400, 250);
    Rect cursorRect = Geo_SetRectPS(mousePos, cursorRectSize, RP_CENTER);

    Rect vCursorRect = Geo_UnprojectRect(cursorRect, scaleF, origin);
    Grid grid = Grid_SectionFromRect(vCursorRect, vCellSize, totalGrid);
    Rect vGridRect = Grid_ToRect(grid, vCellSize);
    Rect gridRect = Geo_ProjectRect(vGridRect, scaleF, origin);

    Point vMousePos = Geo_UnprojectPoint(mousePos, scaleF, origin);
    GNode mouseNode = Grid_PointToNode(vMousePos, vCellSize);
    Rect vMouseRect = Geo_SetRectPS(Grid_NodeToPoint(mouseNode, vCellSize), SIZE_SQR(vCellSize), RP_TOP_LEFT);
    Rect mouseRect = Geo_ProjectRect(vMouseRect, scaleF, origin);

    const Color colBG = COLOR(0x23, 0x23, 0x23, 0xFF);
    const Color colGrid = RED;
    const Color colCursor = BLUE;
    const Color colActive = YELLOW;
    const Color colMouse = GREEN;

    while (!WindowShouldClose()){
        if (IsWindowResized()){
            winSize = SIZE(GetScreenWidth(), GetScreenHeight());
            rScreen = Geo_ProjectSizeInSize(vScreen, winSize, winMargin);
            scaleF = rScreen.width / vScreen.width;
            origin = RORIGIN(rScreen);
        }

        mousePos = GetMousePosition();
        vMousePos = Geo_UnprojectPoint(mousePos, scaleF, origin);

        cursorRect = Geo_SetRectPS(mousePos, cursorRectSize, RP_CENTER);
        vCursorRect = Geo_UnprojectRect(cursorRect, scaleF, origin);

        grid = Grid_SectionFromRect(vCursorRect, vCellSize, totalGrid);
        vGridRect = Grid_ToRect(grid, vCellSize);
        gridRect = Geo_ProjectRect(vGridRect, scaleF, origin);

        mouseNode = Grid_PointToNode(vMousePos, vCellSize);
        vMouseRect = Geo_SetRectPS(Grid_NodeToPoint(mouseNode, vCellSize), SIZE_SQR(vCellSize), RP_TOP_LEFT);
        mouseRect = Geo_ProjectRect(vMouseRect, scaleF, origin);

        BeginDrawing();
        ClearBackground(colBG);
        DrawRectangleRec(gridRect, colActive);
        if (Grid_NodeIsInGrid(mouseNode, totalGrid)){
            DrawRectangleRec(mouseRect, colMouse);
        }
        DrawRectangleLines(cursorRect.x, cursorRect.y, cursorRect.width, cursorRect.height, colCursor);
        for (int x = 0; x <= totalGrid.nCols; x++){
            DrawLineV(Geo_ProjectPoint(Grid_NodeToPoint(GNODE(x, 0), vCellSize), scaleF, origin), 
                                       Geo_ProjectPoint(Grid_NodeToPoint(GNODE(x, totalGrid.nRows), vCellSize), scaleF, origin), 
                                       colGrid);
        }
        for (int y = 0; y <= totalGrid.nRows; y++){
            DrawLineV(Geo_ProjectPoint(Grid_NodeToPoint(GNODE(0, y), vCellSize), scaleF, origin), 
                                       Geo_ProjectPoint(Grid_NodeToPoint(GNODE(totalGrid.nCols, y), vCellSize), scaleF, origin), 
                                       colGrid);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}



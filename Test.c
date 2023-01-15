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

#include "Mods/Assets/Assets.h"

#include "Mods/Public/Public.h"
#include "Mods/Fund/Fund.h"
#include "Mods/Logic/Logic.h"
#include "Mods/Graph/Graph.h"


// ============================================================================ TEST

// Function for testing and debugging
int Test(UNUSED int argc, UNUSED char** argv){

    Window_Init();

    const Size vScreen = SIZE_SQR(1000);
    const Point vCenter = POINT(500, 500);
    const float vRadius = 500;

    VGraph* vg = VGraph_Make(vScreen, TO_RECT(Glo_WinSize), 25.0f);

    PRINT_LINE
    VGraph_Print(vg);
    PRINT_LINE


    bool isSelected = false;
    while (!WindowShouldClose()){
        if (IsWindowResized()){
            Window_UpdateWinSize();
            VGraph_Resize(vg, TO_RECT(Glo_WinSize));
            PRINT_LINE 
            VGraph_Print(vg);
            PRINT_LINE
        }

        Point mousePos = GetMousePosition();
        mousePos = VGraph_UnprojectPoint(mousePos, vg);
        isSelected = (Geo_PointsDistance(mousePos, vCenter) <= vRadius);

        BeginDrawing();
        ClearBackground(COL_BG);
        DrawCircleV(VGraph_ProjectPoint(vCenter, vg), VGraph_ProjectLength(vRadius, vg), 
                    isSelected ? COL_ELECTRIC_2 : COL_ROD);
        EndDrawing();
    }

    vg = VGraph_Free(vg);

    Window_Close();

    return 0;
}



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

    MCol_MakeDefault();

    Texture_LoadAll();

    RGrid* rGrid = RGrid_MakeEmpty(100, 100);
    RGrid_CreateRandom(rGrid);
    RGrid_Shuffle(rGrid);

    SGraph* sg = SGraph_MakeFromGrid(RGrid_GetSize(rGrid), ROD_DEF_TEXTURE_SIZE, TO_RECT(Glo_WinSize), 
                                     SGRAPH_DEF_MARGIN, ROD_MIN_TEXTURE_SIZE, ROD_MAX_TEXTURE_SIZE);

    float tileSize = Grid_CalcSqrCellSize(RGrid_GetSize(rGrid), SGraph_GetVScreen(sg));

    RodModel* rodModel = RGraph_MakeRodModel(tileSize);

    while (!WindowShouldClose()){
        if (IsWindowResized()){
            Window_UpdateWinSize();
            SGraph_SetView(sg, TO_RECT(Glo_WinSize));
            tileSize = Grid_CalcSqrCellSize(RGrid_GetSize(rGrid), SGraph_GetVScreen(sg));
            RGraph_ResizeRodModel(rodModel, tileSize);
        }

        MCol_Update(Glo_MCol);
        RGrid_Update(rGrid);

        KeyboardKey key = GetKeyPressed();
        switch (key){
            case WKEY_RIGHT: case WKEY_DOWN: case WKEY_LEFT: case WKEY_UP:{
                SGraph_MoveViewportToDir(sg, Direction_FromKey(key), MIN_DIM(Glo_WinSize) * 0.07f);
                break;
            }

            case WKEY_PLUS: case WKEY_MINUS:{
                SGraph_Zoom(sg, (key == WKEY_PLUS) ? 1.1f : 1.0f / 1.1f);
                tileSize = Grid_CalcSqrCellSize(RGrid_GetSize(rGrid), SGraph_GetVScreen(sg));
                RGraph_ResizeRodModel(rodModel, tileSize);
                break;
            }

            default: {break;}
        }

        Point mousePos = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            mousePos = SGraph_UnprojectPoint(mousePos, sg);
            GNode node = Grid_PointToNode(mousePos, tileSize);
            RGrid_RotateRod(rGrid, node);
        }

        Grid visible = Grid_SectionFromRect(SGraph_GetViewport(sg), tileSize, RGrid_GetSize(rGrid));
        Point origin = SGraph_ProjectPoint(Grid_NodeToPoint(visible.origin, tileSize), sg);

        BeginDrawing();
        ClearBackground(COL_BG);
        RGraph_DrawRGrid(rGrid, visible, origin, rodModel);
        EndDrawing();
    }

    MCol_FreeDefault();
    Texture_UnloadAll();
    rGrid = RGrid_Free(rGrid);
    sg = SGraph_Free(sg);
    rodModel = RGraph_FreeRodModel(rodModel);

    Window_Close();

    return 0;
}



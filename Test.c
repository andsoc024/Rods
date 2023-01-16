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

    Texture_LoadAll();
    PRINT_LINE
    Texture_PrintAll();
    PRINT_LINE

    int iconID = 0;
    while (!WindowShouldClose()){
        if (IsWindowResized()){
            Window_UpdateWinSize();
        }

        KeyboardKey key = GetKeyPressed();
        switch (key){
            case WKEY_RIGHT: case WKEY_LEFT:{
                iconID += ((key == WKEY_RIGHT) ? 1 : -1);
                iconID = PUT_IN_RANGE(iconID, 0, ICONS_N - 1);
                break;
            }

            default: {break;}
        }

        Point pos = POINT((Glo_WinSize.width  - ICON_DEF_TEXTURE_SIZE) * 0.5f, 
                          (Glo_WinSize.height - ICON_DEF_TEXTURE_SIZE) * 0.5f);

        BeginDrawing();
        ClearBackground(COL_BG);
        DrawTexture(Glo_Textures.icons[iconID], pos.x, pos.y, COL_UI_FG_SECONDARY);
        EndDrawing();
    }

    Texture_UnloadAll();

    return 0;
}



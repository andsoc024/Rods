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
    
    RodModel* rodModel = RGraph_MakeRodModel(ROD_DEF_TEXTURE_SIZE);
    PRINT_LINE
    RGraph_PrintRodModel(rodModel);
    PRINT_LINE 

    float tileSize = ROD_DEF_TEXTURE_SIZE;

    Rod rod = ROD_NULL;
    Rod_Set(&rod, LEGDIR_UP | LEGDIR_RIGHT);


    bool isSelected = false;
    while (!WindowShouldClose()){
        if (IsWindowResized()){
            Window_UpdateWinSize();
        }

        MCol_Update(Glo_MCol);
        Rod_Update(&rod);

        KeyboardKey key = GetKeyPressed();
        switch (key){
            case WKEY_RIGHT: case WKEY_DOWN: case WKEY_LEFT: case WKEY_UP:{
                int legDir = Direction_ToLegDir(Direction_FromKey(key));
                if (rod.legs & legDir){
                    rod.legs ^= legDir;
                }else{
                    rod.legs |= legDir;
                }
                break;
            }

            case WKEY_PLUS: case WKEY_MINUS:{
                float zoom = (key == WKEY_PLUS) ? 1.1f : 1.0f / 1.1f;
                tileSize *= zoom;
                tileSize = PUT_IN_RANGE(tileSize, ROD_MIN_TEXTURE_SIZE, ROD_MAX_TEXTURE_SIZE);
                RGraph_ResizeRodModel(rodModel, tileSize);
                break;
            }

            case WKEY_TAB:{
                TOGGLE(isSelected)
                break;
            }

            case WKEY_SPACE:{
                Rod_Rotate(&rod, 1, WITH_ANIM);
                break;
            }

            case WKEY_ENTER:{
                TOGGLE(rod.isElectrified)
                break;
            }

            default: {break;}
        }

        Point pos = POINT((Glo_WinSize.width - tileSize)  * 0.5f, 
                          (Glo_WinSize.height - tileSize) * 0.5f);

        BeginDrawing();
        ClearBackground(COL_BG);
        RGraph_DrawRod(&rod, pos, rodModel);
        if (isSelected){
            RGraph_DrawSelBox(pos, rodModel);
        }
        EndDrawing();
    }

    MCol_FreeDefault();
    Texture_UnloadAll();
    rodModel = RGraph_FreeRodModel(rodModel);

    Window_Close();

    return 0;
}



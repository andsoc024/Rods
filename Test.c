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

    const float txtrSize = 300.0f;

    Texture2D txtrs[0x10];
    for (int legs = 0x0; legs < 0x10; legs++){
        Image im = GenImageColor(txtrSize, txtrSize, COL_NULL);
        Shape_DrawRodInImage(&im, legs, WHITE);
        txtrs[legs] = LoadTextureFromImage(im);
        UnloadImage(im);
        im = (Image) {0};
    }

    Image im = GenImageColor(txtrSize, txtrSize, COL_NULL);
    Shape_DrawSelBoxInImage(&im, WHITE);
    Texture2D txtrSelBox = LoadTextureFromImage(im);
    UnloadImage(im);
    im = (Image) {0};


    Point pos = POINT((Glo_WinSize.width - txtrSize)  * 0.5f, 
                      (Glo_WinSize.height - txtrSize) * 0.5f);
    
    int legs = 0;
    bool isSource = false;
    bool isElectrified = false;
    bool isSelected = false;

    while (!WindowShouldClose()){
        if (IsWindowResized()){
            Window_UpdateWinSize();
            pos = POINT((Glo_WinSize.width - txtrSize)  * 0.5f, 
                        (Glo_WinSize.height - txtrSize) * 0.5f);
        }

        MCol_Update(Glo_MCol);

        KeyboardKey key = GetKeyPressed();
        switch (key){
            case WKEY_RIGHT: case WKEY_DOWN: case WKEY_LEFT: case WKEY_UP:{
                int legDir = Direction_ToLegDir(Direction_FromKey(key));
                if (legs & legDir){
                    legs ^= legDir;
                }else{
                    legs |= legDir;
                }
                break;
            }

            case WKEY_ENTER:{
                TOGGLE(isSource)
                break;
            }

            case WKEY_SPACE:{
                TOGGLE(isElectrified)
                break;
            }

            case WKEY_S:{
                TOGGLE(isSelected)
                break;
            }

            default: {break;}
        }

        BeginDrawing();
        ClearBackground(COL_BG);
        DrawTexture(txtrs[legs], pos.x, pos.y, isElectrified ? MCol(Glo_MCol) : COL_ROD);
        if (isSource){
            Shape_DrawSource(pos, txtrSize, isElectrified ? MCol(Glo_MCol) : COL_ROD);
        }
        if (isSelected){
            DrawTexture(txtrSelBox, pos.x, pos.y, COL_SELBOX);
        }
        EndDrawing();
    }

    MCol_FreeDefault();

    for (int legs = 0x0; legs < 0x10; legs++){
        UnloadTexture(txtrs[legs]);
    }
    UnloadTexture(txtrSelBox);

    Window_Close();

    return 0;
}



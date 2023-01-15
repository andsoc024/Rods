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

    Rect rect = Geo_AlignRect(RECT(0, 0, 500, 300), TO_RECT(Glo_WinSize), RP_CENTER);
    Size minSize = SIZE(100, 100);
    Size maxSize = Geo_ApplySizeMargins(Glo_WinSize, 10.0f);

    Font_LoadDefault();

    const char* txt = "Testing...";
    float fontSize = Font_FitTextInSize(txt, RSIZE(rect));
    Point pos = Font_CalcTextPos(txt, fontSize, Geo_RectPoint(TO_RECT(Glo_WinSize), RP_CENTER), RP_CENTER);

    while (!WindowShouldClose()){
        if (IsWindowResized()){
            Window_UpdateWinSize();
            maxSize = Glo_WinSize;
            rect = Geo_ResizeRect(rect, Geo_PutSizeInRange(RSIZE(rect), &minSize, &maxSize), RP_CENTER);
            rect = Geo_AlignRect(rect, TO_RECT(Glo_WinSize), RP_CENTER);
            fontSize = Font_FitTextInSize(txt, RSIZE(rect));
            pos = Font_CalcTextPos(txt, fontSize, Geo_RectPoint(TO_RECT(Glo_WinSize), RP_CENTER), RP_CENTER);
        }

        KeyboardKey key = GetKeyPressed();
        switch (key){
            case WKEY_RIGHT: case WKEY_DOWN: case WKEY_LEFT: case WKEY_UP:{
                E_Direction dir = Direction_FromKey(key);
                Vector2 d = Geo_MovePointToDir(VECTOR_NULL, dir, 10.0f);
                d.y *= (-1.0f);
                Size newSize = SIZE(rect.width + d.x, rect.height + d.y);
                newSize = Geo_PutSizeInRange(newSize, &minSize, &maxSize);
                rect = Geo_ResizeRect(rect, newSize, RP_CENTER);
                fontSize = Font_FitTextInSize(txt, RSIZE(rect));
                pos = Font_CalcTextPos(txt, fontSize, Geo_RectPoint(TO_RECT(Glo_WinSize), RP_CENTER), RP_CENTER);
                break;
            }

            default: {break;}
        }

        BeginDrawing();
        ClearBackground(COL_BG);
        Font_DrawText(txt, fontSize, pos, COL_UI_FG_SECONDARY);
        DrawRectangleLines(rect.x, rect.y, rect.width, rect.height, RED);
        EndDrawing();
    } 

    Font_UnloadDefault();
    Window_Close();

    return 0;
}



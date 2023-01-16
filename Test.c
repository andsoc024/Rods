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

    Font_LoadDefault();

    Time t = TIME_NULL;

    Rect rect = Geo_AlignRect(TO_RECT(SIZE(300, 50)), TO_RECT(Glo_WinSize), RP_CENTER);

    Size minSize = SIZE(70, 40);
    const float winMargin = 30.0f;
    Size maxSize = Geo_ApplySizeMargins(Glo_WinSize, winMargin);

    TimDisp* td = TimDisp_Make(RSIZE(rect), RP_CENTER);

    while (!WindowShouldClose()){
        if (IsWindowResized()){
            Window_UpdateWinSize();
            maxSize = Geo_ApplySizeMargins(Glo_WinSize, winMargin);
            Size rectSize = RSIZE(rect);
            rectSize = Geo_PutSizeInRange(rectSize, &minSize, &maxSize);
            rect = Geo_SetRectPS(Geo_RectPoint(TO_RECT(Glo_WinSize), RP_CENTER), rectSize, RP_CENTER);
            TimDisp_Resize(td, RSIZE(rect));
        }

        KeyboardKey key = GetKeyPressed();
        switch (key){
            case WKEY_RIGHT: case WKEY_DOWN: case WKEY_LEFT: case WKEY_UP:{
                Vector2 d = Geo_MovePointToDir(VECTOR_NULL, Direction_FromKey(key), 10.0f);
                Size rectSize = RSIZE(rect);
                rectSize.width += d.x;
                rectSize.height += d.y;
                rectSize = Geo_PutSizeInRange(rectSize, &minSize, &maxSize);
                rect = Geo_ResizeRect(rect, rectSize, RP_CENTER);
                TimDisp_Resize(td, RSIZE(rect));
                break;
            }

            case WKEY_D:{
                t = Time_Shift(t, 500);
                break;
            }

            case WKEY_A:{
                t = Time_Shift(t, -500);
                break;
            }

            case WKEY_W:{
                t = Time_Increment(t);
                break;
            }

            case WKEY_S:{
                t = Time_Shift(t, -1);
                break;
            }

            case WKEY_SPACE:{
                if (Time_IsValid(t)){
                    t = TIME_INVALID;
                }else{
                    t = TIME_NULL;
                }
                break;
            }

            default: {break;}
        }

        BeginDrawing();
        ClearBackground(COL_BG);
        TimDisp_DrawTime(t, RORIGIN(rect), td, COL_UI_FG_PRIMARY);
        Shape_DrawRectOutline(rect, 3, RED);
        EndDrawing();
    }

    Font_UnloadDefault();
    td = TimDisp_Free(td);

    Window_Close();

    return 0;
}



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

    typedef struct Circle{
        Point center;
        float radius;
        Color color;
    }Circle;

    int nCircles = 300;
    Circle* circles = Memory_Allocate(NULL, sizeof(Circle) * nCircles, ZEROVAL_ALL);
    for (int i = 0; i < nCircles; i++){
        circles[i].radius = Math_RandomFloat(10.0f, 35.0f);
        circles[i].center = Geo_RandomPoint(Geo_ApplyRectMargins(TO_RECT(vScreen), circles[i].radius));
        circles[i].color = Color_SetAlpha(Color_Random(0x33, 0xEE), Math_RandomInt(0x50, 0x90));
    }

    SGraph* sg = SGraph_Make(vScreen, TO_RECT(Glo_WinSize), 30.0f, SIZE_SQR(100), SIZE_SQR(100000));

    PRINT_LINE
    SGraph_Print(sg);
    PRINT_LINE

    VGraph* vg = VGraph_Make(vScreen, TO_RECT(SGraph_GetVScreen(sg)), 0.0f);

    const float moveDistance = 20.0f;
    const float defZoom = 1.1f;

    while (!WindowShouldClose()){
        if (IsWindowResized()){
            Window_UpdateWinSize();
            SGraph_SetView(sg, TO_RECT(Glo_WinSize));
            VGraph_Resize(vg, TO_RECT(SGraph_GetVScreen(sg)));
        }

        KeyboardKey key = GetKeyPressed();
        switch (key){
            case WKEY_RIGHT: case WKEY_DOWN: case WKEY_LEFT: case WKEY_UP:{
                E_Direction dir = Direction_FromKey(key);
                SGraph_MoveViewportToDir(sg, dir, moveDistance);
                PRINT_LINE3
                printf("Pos: %.3f, %.3f  ||| ", SGraph_CalcViewportPosRatio(sg, OR_HORISONTAL), SGraph_CalcViewportPosRatio(sg, OR_VERTICAL));
                printf("Size: %.3f, %.3f\n", SGraph_CalcViewportSizeRatio(sg, OR_HORISONTAL), SGraph_CalcViewportSizeRatio(sg, OR_VERTICAL));
                PRINT_LINE3
                break;
            }

            case WKEY_PLUS: case WKEY_MINUS:{
                float zoom = (key == WKEY_PLUS) ? defZoom : 1.0f / defZoom;
                SGraph_Zoom(sg, zoom);
                VGraph_Resize(vg, TO_RECT(SGraph_GetVScreen(sg)));
                PRINT_LINE3
                printf("Pos: %.3f, %.3f  ||| ", SGraph_CalcViewportPosRatio(sg, OR_HORISONTAL), SGraph_CalcViewportPosRatio(sg, OR_VERTICAL));
                printf("Size: %.3f, %.3f\n", SGraph_CalcViewportSizeRatio(sg, OR_HORISONTAL), SGraph_CalcViewportSizeRatio(sg, OR_VERTICAL));
                PRINT_LINE3
                break;
            }

            case WKEY_SPACE:{
                SGraph_SetDefault(sg);
                VGraph_Resize(vg, TO_RECT(SGraph_GetVScreen(sg)));
                PRINT_LINE3
                printf("Pos: %.3f, %.3f  ||| ", SGraph_CalcViewportPosRatio(sg, OR_HORISONTAL), SGraph_CalcViewportPosRatio(sg, OR_VERTICAL));
                printf("Size: %.3f, %.3f\n", SGraph_CalcViewportSizeRatio(sg, OR_HORISONTAL), SGraph_CalcViewportSizeRatio(sg, OR_VERTICAL));
                PRINT_LINE3
                break;
            }

            default: {break;}
        }

        Point mousePos = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            mousePos = VGraph_UnprojectPoint(SGraph_UnprojectPoint(mousePos, sg), vg);
            for (int i = 0; i < nCircles; i++){
                float d = Geo_PointsDistance(circles[i].center, mousePos);
                if (d <= circles[i].radius){
                    circles[i].color = GREEN;
                }
            }
        }

        BeginDrawing();
        ClearBackground(COL_BG);
        for (int i = 0; i < nCircles; i++){
            DrawCircleV(SGraph_ProjectPoint(VGraph_ProjectPoint(circles[i].center, vg), sg), 
                        VGraph_ProjectLength(circles[i].radius, vg), circles[i].color);
        }
        EndDrawing();
    }

    circles = Memory_Free(circles);
    vg = VGraph_Free(vg);
    sg = SGraph_Free(sg);

    Window_Close();

    return 0;
}



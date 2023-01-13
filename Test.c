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

    const Size vScreen = SIZE(1000, 1000);
    const float winMargin = 10.0f;
    Rect rScreen = Geo_ProjectSizeInSize(vScreen, winSize, winMargin);
    float scaleF = rScreen.width / vScreen.width;
    Point origin = RORIGIN(rScreen);

    Rect defVRect = Geo_AlignRect(TO_RECT(SIZE(800, 600)), TO_RECT(vScreen), RP_CENTER);
    Rect vRect = defVRect;
    Point vCenters[RECT_POINT_TYPES_N];
    for (int i = 0; i < RECT_POINT_TYPES_N; i++){
        vCenters[i] = Geo_RectPoint(vRect, i);
    }
    const float vRadius = 15.0f;

    Rect rRect = Geo_ProjectRect(vRect, scaleF, origin);

    Point mousePos = GetMousePosition();
    const Size cursorSize = SIZE_SQR(50.0f);
    Rect cursorRect = Geo_SetRectPS(mousePos, cursorSize, RP_CENTER);

    const float reflMargin = -20.0f;
    Rect reflRect = Geo_PutRectInRect(cursorRect, rRect, reflMargin);

    const Color colBG = COLOR(0x23, 0x23, 0x23, 0xFF);
    const Color colInactive = RED;
    const Color colReady = ORANGE;
    const Color colActive = GREEN;
    const Color colCircle = PINK;
    const Color colCursor = GRAY;
    Color colRect = colInactive;

    const float moveDist = 10.0f;

    while (!WindowShouldClose()){
        if (IsWindowResized()){
            winSize = SIZE(GetScreenWidth(), GetScreenHeight());
            rScreen = Geo_ProjectSizeInSize(vScreen, winSize, winMargin);
            scaleF = rScreen.width / vScreen.width;
            origin = RORIGIN(rScreen);
        }

        KeyboardKey key = GetKeyPressed();
        switch (key){
            case WKEY_RIGHT: case WKEY_DOWN: case WKEY_LEFT: case WKEY_UP:{
                E_Direction dir = Direction_FromKey(key);
                vRect = Geo_MoveRectToDir(vRect, dir, moveDist);
                for (int i = 0; i < RECT_POINT_TYPES_N; i++){
                    vCenters[i] = Geo_MovePointToDir(vCenters[i], dir, moveDist);
                }
                break;
            }

            case WKEY_SPACE:{
                vRect = defVRect;
                for (int i = 0; i < RECT_POINT_TYPES_N; i++){
                    vCenters[i] = Geo_RectPoint(vRect, i);
                }
                break;
            }

            default: {break;}
        }

        rRect = Geo_ProjectRect(vRect, scaleF, origin);

        mousePos = GetMousePosition();
        cursorRect = Geo_SetRectPS(mousePos, cursorSize, RP_CENTER);

        reflRect = Geo_PutRectInRect(cursorRect, rRect, reflMargin);

        colRect = Geo_PointIsInRect(mousePos, rRect) ? colActive :
                  Geo_RectsOverlap(cursorRect, rRect) ? colReady : colInactive;

        BeginDrawing();
        ClearBackground(colBG);
        DrawRectangleRec(rRect, colRect);
        for (int i = 0; i < RECT_POINT_TYPES_N; i++){
            DrawCircleV(Geo_ProjectPoint(vCenters[i], scaleF, origin), vRadius * scaleF, colCircle);
        }
        DrawRectangleRec(cursorRect, colCursor);
        DrawRectangleRec(reflRect, colCursor);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}



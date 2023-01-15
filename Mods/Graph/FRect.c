// ============================================================================
// RODS
// Flying Rectangles
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions for managing the Flying Rectangles.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"
#include "../Fund/Fund.h"
#include "Graph.h"


// ============================================================================ PRIVATE CONSTANTS

#define FRECT_MIN_N                         5
#define FRECT_MAX_N                         30
#define FRECT_DEF_SPACING                   100.0f
#define FRECT_MIN_SIZE                      SIZE( 75.0f, 10.0f)
#define FRECT_MAX_SIZE                      SIZE(250.0f, 25.0f)
#define FRECT_MIN_SPEED                     1.0f
#define FRECT_MAX_SPEED                     3.0f
#define FRECT_SPEEDS_N                      15
#define FRECT_MIN_ALPHA                     0x20
#define FRECT_MAX_ALPHA                     0x55


// ============================================================================ PRIVATE STRUCTURES

// **************************************************************************** FRect
// Flying rectangles are the semitransparent rectangles that move from left to 
// right, at different speeds, in the background
typedef struct FRect{
    Rect rect;
    float speed;
    Color color;
    float minY;
    float maxY;
}FRect;


// ============================================================================ OPAQUE STRUCTURES

// **************************************************************************** FRects
// Flying rectangles are the semitransparent rectangles that move from left to 
// right, at different speeds, in the background. FRects is a collection of 
// flying rectangles.
struct FRects{
    FRect rects[FRECT_MAX_N];
    int n;
};


// ============================================================================ PRIVATE FUNC DECL

void            FRect_Init(FRect* fRect, float minY, float maxY);
void            FRect_Reset(FRect* fRect);
void            FRect_PlaceRandomX(FRect* fRect);
void            FRect_Update(FRect* fRect);
#ifdef DEBUG_MODE
    void        FRect_Print(FRect* fRect, bool withNewLine);
#endif






// ============================================================================ FUNC DEF

// **************************************************************************** FRects_Make

// Make a flying rectangles collection, adjusted for the current window size
FRects* FRects_Make(void){
    FRects* fRects = Memory_Allocate(NULL, sizeof(FRects), ZEROVAL_ALL);

    FRects_Resize(fRects);

    return fRects;
}


// **************************************************************************** FRects_Free

// Free the memory of the flying rectangles collection
FRects* FRects_Free(FRects* fRects){
    return Memory_Free(fRects);
}


// **************************************************************************** FRects_Resize

// Adjust the flying rectangles for the current window size
void FRects_Resize(FRects* fRects){
    int n = Glo_WinSize.height / FRECT_DEF_SPACING;
    n = PUT_IN_RANGE(n, FRECT_MIN_N, FRECT_MAX_N);
    float spacing = Glo_WinSize.height / (float) n;

    fRects->n = n;

    const float EXTRA_RATIO = 0.5f;

    for (int i = 0; i < n; i++){
        float minY = (float) (i + 0) * spacing - EXTRA_RATIO * spacing;
        float maxY = (float) (i + 1) * spacing + EXTRA_RATIO * spacing; 
        minY = MAX(minY, 0.0f);
        maxY = MIN(maxY, Glo_WinSize.height);

        FRect_Init(&(fRects->rects[i]), minY, maxY);

        FRect_PlaceRandomX(&(fRects->rects[i]));
    }
}


// **************************************************************************** FRects_Update

// Update all the flying rectangles in the collection
void FRects_Update(FRects* fRects){
    for (int i = 0; i < fRects->n; i++){
        FRect_Update(&(fRects->rects[i]));
    }
}


// **************************************************************************** FRects_Draw

// Draw all the flying rectangles in the collection
void FRects_Draw(FRects* fRects){
    for (int i = 0; i < fRects->n; i++){
        DrawRectangleRec(fRects->rects[i].rect, fRects->rects[i].color);
    }
}


#ifdef DEBUG_MODE
// **************************************************************************** FRects_Print

    // Multiline print of the flying rectangles collection
    void FRects_Print(FRects* fRects){
        CHECK_NULL(fRects, WITH_NEW_LINE)

        printf("Flying Rectangles (%d):\n", fRects->n);

        for (int i = 0; i < fRects->n; i++){
            printf("   %2d) ", i);
            FRect_Print(&(fRects->rects[i]), WITH_NEW_LINE);
        }
    }
#endif






// ============================================================================ PRIVATE FUNC DEF

// **************************************************************************** FRect_Init

// Initialize the flying rectangle by setting its y-axis range and resetting 
// it, with a random size, speed and alpha value. Place it with its right edge 
// at the left edge of the window
void FRect_Init(FRect* fRect, float minY, float maxY){
    fRect->minY = minY;
    fRect->maxY = maxY;

    FRect_Reset(fRect);
}


// **************************************************************************** FRect_Reset

// Give the flying rectangle a random size, speed and alpha value. Place it 
// with its right edge at the left edge of the window
void FRect_Reset(FRect* fRect){
    fRect->rect.width  = Math_RandomFloat(FRECT_MIN_SIZE.width,  FRECT_MAX_SIZE.width);
    fRect->rect.height = Math_RandomFloat(FRECT_MIN_SIZE.height, FRECT_MAX_SIZE.height);

    fRect->rect.x = -fRect->rect.width;
    fRect->rect.y = Math_RandomFloat(fRect->minY, fRect->maxY);

    const float SPEED_INCR = (FRECT_MAX_SPEED - FRECT_MIN_SPEED) / (float) FRECT_SPEEDS_N;
    fRect->speed = FRECT_MIN_SPEED + SPEED_INCR * (float) Math_RandomInt(0, FRECT_SPEEDS_N);

    fRect->color = Color_SetAlpha(COL_FRECT, Math_RandomInt(FRECT_MIN_ALPHA, FRECT_MAX_ALPHA));
}


// **************************************************************************** FRect_PlaceRandomX

// Place the flying rectangle at a random X position in the window
void FRect_PlaceRandomX(FRect* fRect){
    fRect->rect.x = Math_RandomFloat(-fRect->rect.width, Glo_WinSize.width);
}


// **************************************************************************** FRect_Update

// Move the flying rectangle to the right, according to its speed. If it passes 
// completely the right edge of the window, reset it
void FRect_Update(FRect* fRect){
    fRect->rect.x += fRect->speed;

    if (fRect->rect.x > Glo_WinSize.width){
        FRect_Reset(fRect);
    }
}


#ifdef DEBUG_MODE
// **************************************************************************** FRect_Print

    // Single line print of the parameters of the flying rectangle. Optionally 
    // with new line
    void FRect_Print(FRect* fRect, bool withNewLine){
        CHECK_NULL(fRect, withNewLine)

        Geo_PrintRect(fRect->rect, WITHOUT_NEW_LINE);
        printf(" | Speed: %.3f", fRect->speed);
        printf(" | Alpha: %02X", fRect->color.a);
        printf(" | Y: %.3f - %.3f", fRect->minY, fRect->minY);

        if (withNewLine) {printf("\n");}
    }
#endif



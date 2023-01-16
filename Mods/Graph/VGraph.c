// ============================================================================
// RODS
// Virtual Graphics
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions for managing Virtual Graphics.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"
#include "../Fund/Fund.h"
#include "Graph.h"


// ============================================================================ OPAQUE STRUCTURES

// **************************************************************************** VGraph

// Virtual Graphics transforms points by scaling and translating, so that a 
// virtual screen is projected in a real screen
struct VGraph{
    Size vScreen;
    Rect rScreen;

    float margin;

    float scaleF;
    Point origin;
};






// ============================================================================ FUNC DEF

// **************************************************************************** VGraph_Make

// Make a virtual graphics object
VGraph* VGraph_Make(Size vScreen, Rect cRect, float margin){
    VGraph* vg = Memory_Allocate(NULL, sizeof(VGraph), ZEROVAL_ALL);

    vg->vScreen = vScreen;
    vg->margin = margin;

    VGraph_Resize(vg, cRect);

    return vg;
}


// **************************************************************************** VGraph_Free

// Free the memory of the virtual graphics object. Return NULL
VGraph* VGraph_Free(VGraph* vg){
    return Memory_Free(vg);
}


// **************************************************************************** VGraph_Resize

// Resize the virtual graphics by projecting the virtual screen in the given 
// containing rectangle
void VGraph_Resize(VGraph* vg, Rect cRect){
    vg->rScreen = Geo_ProjectSizeInRect(vg->vScreen, cRect, vg->margin);

    vg->scaleF = vg->rScreen.width / vg->vScreen.width;
    vg->origin = RORIGIN(vg->rScreen);
}


// **************************************************************************** VGraph_ProjectPoint

// Project the point from the virtual to the real screen
Point VGraph_ProjectPoint(Point p, const VGraph* vg){
    return Geo_ProjectPoint(p, vg->scaleF, vg->origin);
}


// **************************************************************************** VGraph_UnprojectPoint

// Project the point from the real to the virtual screen
Point VGraph_UnprojectPoint(Point p, const VGraph* vg){
    return Geo_UnprojectPoint(p, vg->scaleF, vg->origin);
}


// **************************************************************************** VGraph_ProjectRect

// Project the rectangle from the virtual to the real screen
Rect VGraph_ProjectRect(Rect rect, const VGraph* vg){
    return Geo_ProjectRect(rect, vg->scaleF, vg->origin);
}


// **************************************************************************** VGraph_UnprojectRect

// Project the rectangle from the real to the virtual screen
Rect VGraph_UnprojectRect(Rect rect, const VGraph* vg){
    return Geo_UnprojectRect(rect, vg->scaleF, vg->origin);
}


// **************************************************************************** VGraph_ProjectLength

// Scale the length so that it is projected from the virtual to the real screen
float VGraph_ProjectLength(float length, const VGraph* vg){
    return length * vg->scaleF;
}


// **************************************************************************** VGraph_UnprojectLength

// Scale the length do that it is projected from the real to the virtual screen
float VGraph_UnprojectLength(float length, const VGraph* vg){
    return length * (1.0f / vg->scaleF);
}


// **************************************************************************** VGraph_GetVScreen

// Get the virtual screen as Size
Size VGraph_GetVScreen(const VGraph* vg){
    return vg->vScreen;
}


// **************************************************************************** VGraph_GetRScreen

// Get the real screen as Rect
Rect VGraph_GetRScreen(const VGraph* vg){
    return vg->rScreen;
}


// **************************************************************************** VGraph_GetMargin

// Get the margin
float VGraph_GetMargin(const VGraph* vg){
    return vg->margin;
}


// **************************************************************************** VGraph_GetScaleF

// Get the scale factor, for drawing graphics
float VGraph_GetScaleF(const VGraph* vg){
    return vg->scaleF;
}


// **************************************************************************** VGraph_GetOrigin

// Get the origin, for drawing graphics
Point VGraph_GetOrigin(const VGraph* vg){
    return vg->origin;
}


#ifdef DEBUG_MODE
// **************************************************************************** VGraph_Print

    // Multiline print of the parameters of the virtual graphics object
    void VGraph_Print(const VGraph* vg){
        CHECK_NULL(vg, WITH_NEW_LINE)

        printf("Virtual Screen: "); Geo_PrintSize(vg->vScreen, WITH_NEW_LINE);
        printf("Real Screen:    "); Geo_PrintRect(vg->rScreen, WITH_NEW_LINE);
        printf("Margin:         %.3f\n", vg->margin);
        printf("Scale Factor:   %.3f\n", vg->scaleF);
        printf("Origin:         "); Geo_PrintPoint(vg->origin, WITH_NEW_LINE);
    }
#endif



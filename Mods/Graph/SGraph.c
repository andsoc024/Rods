// ============================================================================
// RODS
// Scroll Graphics
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions for managing Scroll Graphics.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"
#include "../Fund/Fund.h"
#include "Graph.h"


// ============================================================================ PRIVATE CONSTANTS

#define SGRAPH_DEF_VIEW_MARGIN              30.0f


// ============================================================================ OPAQUE STRUCTURES

// **************************************************************************** SGraph

// Scroll Graphics transforms points by translating, so that only the part of 
// a virtual screen that is inside its viewport is visible in the view in the 
// screen. The viewport and view have always the same size
struct SGraph{
    Size vScreen;
    Rect viewport;
    Point viewOrigin;

    float margin;

    Size minScreen;
    Size maxScreen;

    Point origin;
};


// ============================================================================ PRIVATE FUNC DECL

static void     SGraph_Recalc(SGraph* sg);






// ============================================================================ FUNC DEF

// **************************************************************************** SGraph_Make

// Make a scroll graphics object
SGraph* SGraph_Make(Size vScreen, Rect view, float margin, Size minScreen, Size maxScreen){
    SGraph* sg = Memory_Allocate(NULL, sizeof(SGraph), ZEROVAL_ALL);

    maxScreen.width = MAX(maxScreen.width, minScreen.width);
    minScreen.height = minScreen.width / (vScreen.width / vScreen.height);
    maxScreen.height = maxScreen.width / (vScreen.width / vScreen.height);

    vScreen = Geo_PutSizeInRange(vScreen, &minScreen, &maxScreen);

    sg->vScreen = vScreen;
    sg->margin = margin;
    sg->viewport = TO_RECT(RSIZE(view));
    sg->viewOrigin = RORIGIN(view);
    sg->minScreen = minScreen;
    sg->maxScreen = maxScreen;

    SGraph_SetDefault(sg);

    return sg;
}


// **************************************************************************** SGraph_MakeFromGrid

// Make a scroll graphics object based on the given grid
SGraph* SGraph_MakeFromGrid(Grid grid, float cellSize, Rect view, float margin, float minCellSize, float maxCellSize){
    Size vScreen = Grid_ToSize(grid, cellSize);
    Size minScreen = Grid_ToSize(grid, minCellSize);
    Size maxScreen = Grid_ToSize(grid, maxCellSize);

    return SGraph_Make(vScreen, view, margin, minScreen, maxScreen);    
}


// **************************************************************************** SGraph_Copy

// Copy the scroll graphics object from src to dst
SGraph* SGraph_Copy(SGraph* dst, const SGraph* src){
    return Memory_Copy(dst, src, sizeof(SGraph));
}


// **************************************************************************** SGraph_Free

// Free the memory of the scroll graphics object. Return NULL
SGraph* SGraph_Free(SGraph* sg){
    return Memory_Free(sg);
}


// **************************************************************************** SGraph_SetDefault

// Resize the virtual screen so that it fits inside the view and place the 
// viewport in the center
void SGraph_SetDefault(SGraph* sg){
    sg->vScreen = Geo_FitSizeInSize(sg->vScreen, RSIZE(sg->viewport), SGRAPH_DEF_VIEW_MARGIN);
    sg->vScreen = Geo_PutSizeInRange(sg->vScreen, &(sg->minScreen), &(sg->maxScreen));

    sg->viewport = Geo_AlignRect(sg->viewport, TO_RECT(sg->vScreen), RP_CENTER);

    SGraph_Recalc(sg);
}


// **************************************************************************** SGraph_SetView

// Set the view. Resize the viewport, so that it has the same size as the view. 
// Resize the virtual screen so that approximately the same part of the virtual 
// screen is inside the viewport
void SGraph_SetView(SGraph* sg, Rect view){
    sg->viewOrigin = RORIGIN(view);

    float zoomX = view.width / sg->viewport.width;
    float zoomY = view.height / sg->viewport.height;
    float zoom = AVG(zoomX, zoomY);

    SGraph_Zoom(sg, zoom);

    SGraph_ResizeViewport(sg, RSIZE(view), RP_CENTER);
}


// **************************************************************************** SGraph_SetViewport

// Set the viewport
void SGraph_SetViewport(SGraph* sg, Rect viewport){
    sg->viewport = viewport;
    SGraph_Recalc(sg);
}


// **************************************************************************** SGraph_MoveViewport

// Move the viewport by dx and dy
void SGraph_MoveViewport(SGraph* sg, float dx, float dy){
    sg->viewport = Geo_MoveRect(sg->viewport, dx, dy);
    SGraph_Recalc(sg);
}


// **************************************************************************** SGraph_MoveViewportToDir

// Move the viewport towards the given direction by the given distance
void SGraph_MoveViewportToDir(SGraph* sg, E_Direction dir, float distance){
    sg->viewport = Geo_MoveRectToDir(sg->viewport, dir, distance);
    SGraph_Recalc(sg);
}


// **************************************************************************** SGraph_TranslateViewport

// Translate the viewport by the given vector
void SGraph_TranslateViewport(SGraph* sg, Vector2 vector){
    sg->viewport = Geo_TranslateRect(sg->viewport, vector);
    SGraph_Recalc(sg);
}


// **************************************************************************** SGraph_PlaceViewport

// Place the viewport at the given point
void SGraph_PlaceViewport(SGraph* sg, Point pos, E_RectPointType pointType){
    sg->viewport = Geo_PlaceRect(sg->viewport, pos, pointType);
    SGraph_Recalc(sg);
}


// **************************************************************************** SGraph_ResizeViewport

// Resize the viewport, retaining its position at the given point
void SGraph_ResizeViewport(SGraph* sg, Size newSize, E_RectPointType pointType){
    sg->viewport = Geo_ResizeRect(sg->viewport, newSize, pointType);
    SGraph_Recalc(sg);
}


// **************************************************************************** SGraph_SetVScreen

// Set the virtual screen, retaining the viewport's center at the same relative 
// position. Ensure that the virtual screen is within limits
void SGraph_SetVScreen(SGraph* sg, Size vScreen){
    vScreen = Geo_PutSizeInRange(vScreen, &(sg->minScreen), &(sg->maxScreen));

    Point center = Geo_RectPoint(sg->viewport, RP_CENTER);
    center.x *= (vScreen.width  / sg->vScreen.width);
    center.y *= (vScreen.height / sg->vScreen.height);

    sg->vScreen = vScreen;
    sg->viewport = Geo_PlaceRect(sg->viewport, center, RP_CENTER);

    SGraph_Recalc(sg);
}


// **************************************************************************** SGraph_Zoom

// Zoom in (> 1.0f) or out (< 1.0f)
void SGraph_Zoom(SGraph* sg, float zoom){
    zoom = (zoom <= 0.0f) ? 1.0f : zoom;
    Size vScreen = Geo_ScaleSize(sg->vScreen, zoom);
    SGraph_SetVScreen(sg, vScreen);
}


// **************************************************************************** SGraph_SetMinScreen

// Set the minimum screen size
void SGraph_SetMinScreen(SGraph* sg, Size minScreen){
    float scaleF_X = minScreen.width / sg->vScreen.width;
    float scaleF_Y = minScreen.height / sg->vScreen.height;
    float scaleF = AVG(scaleF_X, scaleF_Y);
    sg->minScreen = minScreen = Geo_ScaleSize(sg->vScreen, scaleF);
    sg->maxScreen = Geo_PutSizeInRange(sg->maxScreen, &minScreen, NULL);

    Size vScreen = Geo_PutSizeInRange(sg->vScreen, &minScreen, &(sg->maxScreen));
    SGraph_SetVScreen(sg, vScreen);
}


// **************************************************************************** SGraph_SetMaxScreen

// Set the maximum screen size
void SGraph_SetMaxScreen(SGraph* sg, Size maxScreen){
    float scaleF_X = maxScreen.width / sg->vScreen.width;
    float scaleF_Y = maxScreen.height / sg->vScreen.height;
    float scaleF = AVG(scaleF_X, scaleF_Y);
    sg->maxScreen = maxScreen = Geo_ScaleSize(sg->vScreen, scaleF);
    sg->minScreen = Geo_PutSizeInRange(sg->minScreen, NULL, &maxScreen);

    Size vScreen = Geo_PutSizeInRange(sg->vScreen, &(sg->minScreen), &maxScreen);
    SGraph_SetVScreen(sg, vScreen);
}


// **************************************************************************** SGraph_ProjectPoint

// Project the point from the virtual screen to the view in the window
Point SGraph_ProjectPoint(Point p, const SGraph* sg){
    return Geo_TranslatePoint(p, sg->origin);
}


// **************************************************************************** SGraph_UnprojectPoint

// Project the point from the view in the window to the virtual screen
Point SGraph_UnprojectPoint(Point p, const SGraph* sg){
    return Geo_TranslatePoint(p, Geo_OppositePoint(sg->origin));
}


// **************************************************************************** SGraph_ProjectRect

// Project the rectangle from the virtual screen to the view in the window
Rect SGraph_ProjectRect(Rect rect, const SGraph* sg){
    return Geo_TranslateRect(rect, sg->origin);
}


// **************************************************************************** SGraph_UnprojectRect

// Project the rectangle from the view in the window to the virtual screen
Rect SGraph_UnprojectRect(Rect rect, const SGraph* sg){
    return Geo_TranslateRect(rect, Geo_OppositePoint(sg->origin));
}


// **************************************************************************** SGraph_CalcViewportPosRatio

// The ratio of the viewport position to the total width or height of the 
// virtual screen, including the negative margins but excluding the width or 
// height of the viewport
float SGraph_CalcViewportPosRatio(const SGraph* sg, E_Orientation orientation){
    switch (orientation){
        case OR_HORISONTAL:{
            if (sg->viewport.width >= sg->vScreen.width + 2.0f * sg->margin){
                return 0.0;
            }
            return (sg->viewport.x + sg->margin) / (sg->vScreen.width + 2.0f * sg->margin - sg->viewport.width);
        }

        case OR_VERTICAL:{
            if (sg->viewport.height >= sg->vScreen.height + 2.0f * sg->margin){
                return 0.0f;
            }
            return (sg->viewport.y + sg->margin) / (sg->vScreen.height + 2.0f * sg->margin - sg->viewport.height);
        }

        default: {break;}
    }

    return INVALID;
}


// **************************************************************************** SGraph_CalcViewportSizeRatio

// The ratio of the width or height of the viewport to the width or height of 
// the virtual screen, including the negative margins
float SGraph_CalcViewportSizeRatio(const SGraph* sg, E_Orientation orientation){
    switch (orientation){
        case OR_HORISONTAL:{
            return sg->viewport.width / (sg->vScreen.width + 2.0f * sg->margin);
        }

        case OR_VERTICAL:{
            return sg->viewport.height / (sg->vScreen.height + 2.0f * sg->margin);
        }

        default: {break;}
    }

    return INVALID;
}


// **************************************************************************** SGraph_SetViewportPosFromRatio

// Place the viewport at the position indicated by the position ratio
void SGraph_SetViewportPosFromRatio(SGraph* sg, float posRatio, E_Orientation orientation){
    Point pos = RORIGIN(sg->viewport);

    switch (orientation){
        case OR_HORISONTAL:{
            pos.x = posRatio * (sg->vScreen.width + 2.0f * sg->margin - sg->viewport.width) - sg->margin;
            break;
        }

        case OR_VERTICAL:{
            pos.y = posRatio * (sg->vScreen.height + 2.0f * sg->margin - sg->viewport.height) - sg->margin;
            break;
        }
    }

    SGraph_PlaceViewport(sg, pos, RP_TOP_LEFT);
}


// **************************************************************************** SGraph_GetVScreen

// Get the virtual screen
Size SGraph_GetVScreen(const SGraph* sg){
    return sg->vScreen;
}


// **************************************************************************** SGraph_GetViewport

// Get the viewport
Rect SGraph_GetViewport(const SGraph* sg){
    return sg->viewport;
}


// **************************************************************************** SGraph_GetViewOrigin

// Get the view origin
Point SGraph_GetViewOrigin(const SGraph* sg){
    return sg->viewOrigin;
}


// **************************************************************************** SGraph_GetMargin

// Get the edge margin
float SGraph_GetMargin(const SGraph* sg){
    return sg->margin;
}


// **************************************************************************** SGraph_GetMinScreen

// Get the minimum screen size
Size SGraph_GetMinScreen(const SGraph* sg){
    return sg->minScreen;
}


// **************************************************************************** SGraph_GetMaxScreen

// Get the maximum screen size
Size SGraph_GetMaxScreen(const SGraph* sg){
    return sg->maxScreen;
}


// **************************************************************************** SGraph_GetOrigin

// Get the origin, for translating graphics
Point SGraph_GetOrigin(const SGraph* sg){
    return sg->origin;
}


#ifdef DEBUG_MODE
// **************************************************************************** SGraph_Print

    // Multiline print of the parameters of the scroll object
    void SGraph_Print(const SGraph* sg){
        CHECK_NULL(sg, WITH_NEW_LINE)

        printf("Virtual Screen: "); Geo_PrintSize(sg->vScreen, WITH_NEW_LINE);
        printf("Viewport:       "); Geo_PrintRect(sg->viewport, WITH_NEW_LINE);
        printf("View Origin:    "); Geo_PrintPoint(sg->viewOrigin, WITH_NEW_LINE);

        printf("Margin:         %.3f\n", sg->margin);

        printf("Min Screen:     "); Geo_PrintSize(sg->minScreen, WITH_NEW_LINE);
        printf("Max Screen:     "); Geo_PrintSize(sg->maxScreen, WITH_NEW_LINE);

        printf("Origin:         "); Geo_PrintPoint(sg->origin, WITH_NEW_LINE);
    }
#endif






// ============================================================================ PRIVATE FUNC DEF

// **************************************************************************** SGraph_Recalc

// Ensure that the viewport is inside the virtual screen and calculate the 
// origin, for drawing graphics
static void SGraph_Recalc(SGraph* sg){
    sg->viewport = Geo_PutRectInRect(sg->viewport, TO_RECT(sg->vScreen), -sg->margin);

    sg->origin = Geo_TranslatePoint(sg->viewOrigin, Geo_OppositePoint(RORIGIN(sg->viewport)));
}






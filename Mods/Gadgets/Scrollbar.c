// ============================================================================
// RODS
// Scrollbar
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions and definitions of structures and constants for the scrollbar 
    gadget.

    The scrollbar can be horisontal or vertical. When changed it emits 
    EVENT_SCROLLBAR_CHANGED. The event EVENT_UPDATE_SCROLLBAR causes the 
    scrollbar to change.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"
#include "../Fund/Fund.h"
#include "../Graph/Graph.h"
#include "../GUI/GUI.h"
#include "Gadgets.h"


// ============================================================================ PRIVATE MACROS

// **************************************************************************** SBDATA

// Pointer to the data object of the scrollbar
#define SBDATA \
    ((ScrollbarData*) sbar->data)


// **************************************************************************** TRANSPOSE_SBAR_IF_VERTICAL

// If the scrollbar is vertical transpose the handle and bar rectangles
#define TRANSPOSE_SBAR_IF_VERTICAL \
    if (SBDATA->orientation == OR_VERTICAL){ \
        SBDATA->handleRect = Geo_TransposeRect(SBDATA->handleRect); \
        SBDATA->barRect = Geo_TransposeRect(SBDATA->barRect); \
    }


// **************************************************************************** TRANSPOSE_SBAR_ALL_IF_VERTICAL

// If the scrollbar is vertical transpose the handle and bar rectangles as 
// well as the expanded and collapsed bar rectangles
#define TRANSPOSE_SBAR_ALL_IF_VERTICAL \
    if (SBDATA->orientation == OR_VERTICAL){ \
        SBDATA->handleRect = Geo_TransposeRect(SBDATA->handleRect); \
        SBDATA->barRect = Geo_TransposeRect(SBDATA->barRect); \
        SBDATA->collapsedRect = Geo_TransposeRect(SBDATA->collapsedRect); \
        SBDATA->expandedRect = Geo_TransposeRect(SBDATA->expandedRect); \
    }


// ============================================================================ PRIVATE CONSTANTS

#define SBAR_EXPANDED_WIDTH                 30.0f
#define SBAR_COLLAPSED_WIDTH                10.0f
#define SBAR_THICKNESS                      2.0f

#define COL_SBAR_HANDLE_BG                  COL_UI_BG_PRIMARY
#define COL_SBAR_BG                         COL_UI_BG_SECONDARY
#define COL_SBAR_OUTLINE                    COL_UI_FG_PRIMARY
#define COL_SBAR_EMPH                       COL_UI_FG_EMPH


// ============================================================================ PRIVATE STRUCTURES

// **************************************************************************** ScrollbarData

// The data object of the scrollbar gadget
typedef struct ScrollbarData{
    E_Orientation orientation;
    Rect handleRect;
    Rect barRect;
    Rect collapsedRect;
    Rect expandedRect;
}ScrollbarData;


// ============================================================================ PRIVATE FUNC DECL

static void     Scrollbar_Resize(Gadget* sbar);
static void     Scrollbar_ReactToEvent(Gadget* sbar, Event event, EventQueue* queue);
static void     Scrollbar_Draw(const Gadget* sbar, Vector2 shift);
#ifdef DEBUG_MODE
    static void Scrollbar_PrintData(const Gadget* sbar);
#endif






// ============================================================================ FUNC DEF

// **************************************************************************** Scrollbar_Make

// Make a horisontal or vertical scrollbar
Gadget* Scrollbar_Make(E_GadgetID id, E_Orientation orientation){
    Gadget* sbar = Gadget_Make(id, GT_SCROLLBAR, IS_NOT_SELECTABLE, 0);

    sbar->Resize        = Scrollbar_Resize;
    sbar->ReactToEvent  = Scrollbar_ReactToEvent;
    sbar->Draw          = Scrollbar_Draw;
    #ifdef DEBUG_MODE
        sbar->PrintData = Scrollbar_PrintData;
    #endif

    ScrollbarData* data = Memory_Allocate(NULL, sizeof(ScrollbarData), ZEROVAL_ALL);
    data->orientation = orientation;

    sbar->data = data;

    return sbar;
}


// **************************************************************************** Scrollbar_SetPosFromRatio

// Place the handle at the position indicated by the position ratio (0.0-1.0)
void Scrollbar_SetPosFromRatio(Gadget* sbar, float posRatio){
    posRatio = PUT_IN_RANGE(posRatio, 0.0f, 1.0f);

    TRANSPOSE_SBAR_IF_VERTICAL

    float totalLength = SBDATA->barRect.width - SBDATA->handleRect.width;
    SBDATA->handleRect.x = SBDATA->barRect.x + posRatio * totalLength;

    TRANSPOSE_SBAR_IF_VERTICAL
}


// **************************************************************************** Scrollbar_SetSizeFromRatio

// Set the handle size length as a fraction of the total bar length
void Scrollbar_SetSizeFromRatio(Gadget* sbar, float sizeRatio){
    sizeRatio = PUT_IN_RANGE(sizeRatio, 0.0f, 1.0f);

    TRANSPOSE_SBAR_IF_VERTICAL

    SBDATA->handleRect.width = sizeRatio * SBDATA->barRect.width;

    TRANSPOSE_SBAR_IF_VERTICAL
}


// **************************************************************************** Scrollbar_GetPosRatio

// Get the ratio of the position of the handle to the total length of the bar, 
// excluding the length of the handle
float Scrollbar_GetPosRatio(const Gadget* sbar){
    TRANSPOSE_SBAR_IF_VERTICAL

    float res = (SBDATA->handleRect.x - SBDATA->barRect.x) / 
                (SBDATA->barRect.width - SBDATA->handleRect.width);

    TRANSPOSE_SBAR_IF_VERTICAL

    return res;
}


// **************************************************************************** Scrollbar_GetSizeRatio

// Get the ratio of the handle length to the total length of the bar
float Scrollbar_GetSizeRatio(const Gadget* sbar){
    TRANSPOSE_SBAR_IF_VERTICAL

    float res = SBDATA->handleRect.width / SBDATA->barRect.width;

    TRANSPOSE_SBAR_IF_VERTICAL

    return res;
}


// **************************************************************************** Scrollbar_GetBarRect

// Return the bar rectangle
Rect Scrollbar_GetBarRect(const Gadget* sbar){
    return SBDATA->barRect;
}


// **************************************************************************** Scrollbar_GetExpandedRect

// Return the expanded bar rectangle
Rect Scrollbar_GetExpandedRect(const Gadget* sbar){
    return SBDATA->expandedRect;
}


// **************************************************************************** Scrollbar_GetCollapsedRect

// Return the collapsed bar rectangle
Rect Scrollbar_GetCollapsedRect(const Gadget* sbar){
    return SBDATA->collapsedRect;
}






// ============================================================================ PRIVATE FUNC DEF

// **************************************************************************** Scrollbar_Resize

// Resize the scrollbar within its containing rectangle. Set it as collapsed
static void Scrollbar_Resize(Gadget* sbar){
    sbar->isSelected = false;
    sbar->isPressed = false;
    sbar->isExpanded = false;

    float posRatio = Scrollbar_GetPosRatio(sbar);
    float sizeRatio = Scrollbar_GetSizeRatio(sbar);

    TRANSPOSE_SBAR_ALL_IF_VERTICAL
    if (SBDATA->orientation == OR_VERTICAL){
        sbar->cRect = Geo_TransposeRect(sbar->cRect);
    }

    Size barExpandedSize = SIZE(sbar->cRect.width, SBAR_EXPANDED_WIDTH);
    Size barCollapsedSize = SIZE(sbar->cRect.width, SBAR_COLLAPSED_WIDTH);
    Point bottomLeftP = Geo_RectPoint(sbar->cRect, RP_BOTTOM_LEFT);
    SBDATA->expandedRect = Geo_SetRectPS(bottomLeftP, barExpandedSize, RP_BOTTOM_LEFT);
    SBDATA->collapsedRect = Geo_SetRectPS(bottomLeftP, barCollapsedSize, RP_BOTTOM_LEFT);

    SBDATA->barRect = SBDATA->collapsedRect;

    SBDATA->handleRect.height = SBDATA->expandedRect.height;
    SBDATA->handleRect.y = SBDATA->expandedRect.y;

    TRANSPOSE_SBAR_ALL_IF_VERTICAL
    if (SBDATA->orientation == OR_VERTICAL){
        sbar->cRect = Geo_TransposeRect(sbar->cRect);
    }

    Scrollbar_SetPosFromRatio(sbar, posRatio);
    Scrollbar_SetSizeFromRatio(sbar, sizeRatio);
}


// **************************************************************************** Scrollbar_ReactToEvent

// The scrollbar reacts to mouse, keyboard or other GUI events
static void Scrollbar_ReactToEvent(Gadget* sbar, Event event, EventQueue* queue){
    switch (event.id){
        case EVENT_MOUSE_MOVE:{
            if (!Geo_PointIsInRect(event.data.mouse.pos, SBDATA->barRect)){
                if (sbar->isExpanded){
                    sbar->isExpanded = false;
                    sbar->isSelected = false;
                    sbar->isPressed = false;
                    SBDATA->barRect = SBDATA->collapsedRect;
                    Queue_AddEvent(queue, Event_SetAsGadgetCollapsed(sbar->id));
                }
            }else{
                if (!sbar->isExpanded){
                    sbar->isExpanded = true;
                    SBDATA->barRect = SBDATA->expandedRect;
                    sbar->isSelected = Geo_PointIsInRect(event.data.mouse.pos, SBDATA->handleRect);
                    sbar->isPressed = false;
                    Queue_AddEvent(queue, Event_SetAsGadgetExpanded(sbar->id));
                }else{
                    sbar->isSelected = Geo_PointIsInRect(event.data.mouse.pos, SBDATA->handleRect);
                }
            }
            break;
        }

        case EVENT_MOUSE_DRAG:{
            if (!sbar->isExpanded || !sbar->isPressed) {break;}
            if (event.data.mouse.delta.x == 0.0 && event.data.mouse.delta.y == 0.0){
                break;
            }
            if (SBDATA->orientation == OR_HORISONTAL){
                SBDATA->handleRect.x += event.data.mouse.delta.x;
                SBDATA->handleRect.x = PUT_IN_RANGE(SBDATA->handleRect.x, SBDATA->barRect.x, 
                                                    SBDATA->barRect.x + SBDATA->barRect.width - SBDATA->handleRect.width);
            }else{
                SBDATA->handleRect.y += event.data.mouse.delta.y;
                SBDATA->handleRect.y = PUT_IN_RANGE(SBDATA->handleRect.y, SBDATA->barRect.y, 
                                                    SBDATA->barRect.y + SBDATA->barRect.height - SBDATA->handleRect.height);
            }
            Queue_AddEvent(queue, Event_SetAsScrollbarChanged(sbar->id, Scrollbar_GetPosRatio(sbar), Scrollbar_GetSizeRatio(sbar)));
            break;
        }

        case EVENT_MOUSE_PRESSED:{
            if (sbar->isExpanded){
                if (Geo_PointIsInRect(event.data.mouse.pos, SBDATA->handleRect)){
                    sbar->isPressed = true;
                }else if (Geo_PointIsInRect(event.data.mouse.pos, SBDATA->barRect)){
                    if (SBDATA->orientation == OR_HORISONTAL){
                        SBDATA->handleRect.x = event.data.mouse.pos.x - SBDATA->handleRect.width * 0.5f;
                        float minX = SBDATA->barRect.x;
                        float maxX = SBDATA->barRect.x + SBDATA->barRect.width - SBDATA->handleRect.width;
                        SBDATA->handleRect.x = PUT_IN_RANGE(SBDATA->handleRect.x, minX, maxX);
                    }else{
                        SBDATA->handleRect.y = event.data.mouse.pos.y - SBDATA->handleRect.height * 0.5f;
                        float minY = SBDATA->barRect.y;
                        float maxY = SBDATA->barRect.y + SBDATA->barRect.height - SBDATA->handleRect.height;
                        SBDATA->handleRect.y = PUT_IN_RANGE(SBDATA->handleRect.y, minY, maxY);
                    }
                    Queue_AddEvent(queue, Event_SetAsScrollbarChanged(sbar->id, Scrollbar_GetPosRatio(sbar), Scrollbar_GetSizeRatio(sbar)));
                    sbar->isSelected = true;
                }
            }
            break;
        }

        case EVENT_MOUSE_RELEASED:{
            sbar->isPressed = false;
            break;
        }

        case EVENT_UPDATE_SCROLLBAR:{
            if (event.target == (int) sbar->id){
                Scrollbar_SetSizeFromRatio(sbar, event.data.scrollbar.sizeRatio);
                Scrollbar_SetPosFromRatio(sbar, event.data.scrollbar.posRatio);
            }
            break;
        }

        default: {break;}
    }
}


// **************************************************************************** Scrollbar_Draw

// Draw the scrollbar, if expanded
static void Scrollbar_Draw(const Gadget* sbar, Vector2 shift){
    if (!sbar->isExpanded) {return;}

    Shape_DrawOutlinedRect(Geo_TranslateRect(SBDATA->barRect, shift), 
                           SBAR_THICKNESS, 
                           COL_SBAR_BG, 
                           COL_SBAR_OUTLINE);

    Color handleOutlineColor = sbar->isPressed ? COL_SBAR_HANDLE_BG : 
                               sbar->isSelected ? COL_SBAR_EMPH     : COL_SBAR_OUTLINE;
    
    Shape_DrawOutlinedRect(Geo_TranslateRect(SBDATA->handleRect, shift), 
                           SBAR_THICKNESS, 
                           COL_SBAR_HANDLE_BG, 
                           handleOutlineColor);
}


#ifdef DEBUG_MODE
// **************************************************************************** Scrollbar_PrintData

    // Multiline print of the parameters of the data object of the scrollbar
    static void Scrollbar_PrintData(const Gadget* sbar){
        CHECK_NULL(sbar, WITH_NEW_LINE)
        CHECK_NULL(sbar->data, WITH_NEW_LINE)

        printf("Orientation:    %s\n", Orientation_ToString(SBDATA->orientation));
        printf("Handle Rect:    "); Geo_PrintRect(SBDATA->handleRect, WITH_NEW_LINE);
        printf("Bar Rect:       "); Geo_PrintRect(SBDATA->barRect, WITH_NEW_LINE);
        printf("Collapsed Rect: "); Geo_PrintRect(SBDATA->collapsedRect, WITH_NEW_LINE);
        printf("Expanded Rect:  "); Geo_PrintRect(SBDATA->expandedRect, WITH_NEW_LINE);
    }
#endif





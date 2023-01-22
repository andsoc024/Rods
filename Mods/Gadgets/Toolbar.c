// ============================================================================
// RODS
// Toolbar
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions and definitions of constants and structures for the Toolbar 
    gadget.

    The toolbar contains the timer, a label which shows how many unelectrified 
    rods are left, buttons and the sound switch. It sits always at the top, 
    collapsed and is expanded only when the mouse cursor hovers over it.
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

// **************************************************************************** TBDATA

// Pointer to the data object of the toolbar
#define TBDATA \
    ((ToolbarData*) toolbar->data)


// ============================================================================ PRIVATE CONSTANTS

#define TB_WIN_MARGIN                       10.0f

#define TB_MAX_BTN_SIZE                     85.0f

#define TB_TIMER_RATIO                      2.5f
#define TB_RODS_LEFT_RATIO                  2.0f
#define TB_ICON_RATIO                       0.75f
#define TB_SWITCH_RATIO                     1.5f
#define TB_SMALL_SEP_RATIO                  0.25f
#define TB_BIG_SEP_RATIO                    1.5f
#define TB_HOR_MARGIN_RATIO                 1.0f
#define TB_VER_MARGIN_RATIO                 0.5f

#define TB_BUTTONS_N                        6
#define TB_SMALL_SEPS_N                     5
#define TB_BIG_SEPS_N                       4

#define TB_TOTAL_WIDTH_RATIO                (2.0f                    * TB_HOR_MARGIN_RATIO + \
                                             1.0f                    * TB_TIMER_RATIO      + \
                                             (float) TB_SMALL_SEPS_N * TB_SMALL_SEP_RATIO  + \
                                             1.0f                    * TB_RODS_LEFT_RATIO  + \
                                             (float) TB_BIG_SEPS_N   * TB_BIG_SEP_RATIO    + \
                                             1.0f                    * TB_ICON_RATIO       + \
                                             1.0f                    * TB_SWITCH_RATIO)

#define TB_TOTAL_HEIGHT_RATIO               (1.0f + 2.0f * TB_VER_MARGIN_RATIO)


// ============================================================================ PRIVATE STRUCTURES

// **************************************************************************** ToolbarData

// The data object of the toolbar gadget
typedef struct ToolbarData{
    Rect toolbarRect;
    Rect activeRect;
    float shiftY;
}ToolbarData;


// ============================================================================ PRIVATE FUNC DECL

void            Toolbar_PrepareToFree(Gadget* toolbar);
void            Toolbar_Resize(Gadget* toolbar);
void            Toolbar_ReactToEvent(Gadget* toolbar, Event event, EventQueue* queue);
void            Toolbar_Update(Gadget* toolbar, EventQueue* queue);
Vector2         Toolbar_ChangeShift(const Gadget* toolbar, Vector2 shift);
void            Toolbar_Draw(const Gadget* toolbar, Vector2 shift);
#ifdef DEBUG_MODE
    void        Toolbar_PrintData(const Gadget* toolbar);
#endif


// ============================================================================ FUNC DEF



// ============================================================================ PRIVATE FUNC DEF




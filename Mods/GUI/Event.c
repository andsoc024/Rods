// ============================================================================
// RODS
// Event
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions for setting events.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"
#include "../Fund/Fund.h"
#include "GUI.h"


// ============================================================================ FUNC DEF

// **************************************************************************** Event_SetAsMouseMove

// Set a mouse move event
Event Event_SetAsMouseMove(Point pos, Vector2 delta){
    return ((Event) {.id = EVENT_MOUSE_MOVE, .source = GDG_NONE, .target = GDG_NONE, .isProcessed = false,
                     .data.mouse.pos = pos, .data.mouse.delta = delta});
}


// **************************************************************************** Event_SetAsMouseDrag

// Set a mouse drag event
Event Event_SetAsMouseDrag(Point pos, Vector2 delta){
    return ((Event) {.id = EVENT_MOUSE_DRAG, .source = GDG_NONE, .target = GDG_NONE, .isProcessed = false,
                     .data.mouse.pos = pos, .data.mouse.delta = delta});
}


// **************************************************************************** Event_SetAsMousePressed

// Set a left mouse button pressed event
Event Event_SetAsMousePressed(Point pos){
    return ((Event) {.id = EVENT_MOUSE_PRESSED, .source = GDG_NONE, .target = GDG_NONE, .isProcessed = false,
                     .data.mouse.pos = pos, .data.mouse.delta = VECTOR_NULL});
}


// **************************************************************************** Event_SetAsMouseReleased

// Set a left mouse button released event
Event Event_SetAsMouseReleased(Point pos){
    return ((Event) {.id = EVENT_MOUSE_RELEASED, .source = GDG_NONE, .target = GDG_NONE, .isProcessed = false,
                     .data.mouse.pos = pos, .data.mouse.delta = VECTOR_NULL});
}


// **************************************************************************** Event_SetAsMouseWheel

// Set a mouse wheel moved event
Event Event_SetAsMouseWheel(float move, Point pos){
    return ((Event) {.id = EVENT_MOUSE_WHEEL, .source = GDG_NONE, .target = GDG_NONE, .isProcessed = false,
                     .data.wheel.move = move, .data.wheel.pos = pos});
}


// **************************************************************************** Event_SetAsKeyPressed

// Set a keyboard key pressed event
Event Event_SetAsKeyPressed(KeyboardKey key){
    return ((Event) {.id = EVENT_KEY_PRESSED, .source = GDG_NONE, .target = GDG_NONE, .isProcessed = false,
                     .data.key = key});
}


// **************************************************************************** Event_SetAsKeyReleased

// Set a keyboard key released event
Event Event_SetAsKeyReleased(KeyboardKey key){
    return ((Event) {.id = EVENT_KEY_RELEASED, .source = GDG_NONE, .target = GDG_NONE, .isProcessed = false,
                     .data.key = key});
}


// **************************************************************************** Event_SetAsButtonPressed

// Set a button pressed event
Event Event_SetAsButtonPressed(E_GadgetID source){
    return ((Event) {.id = EVENT_BUTTON_PRESSED, .source = source, .target = GDG_NONE, .isProcessed = false});
}


// **************************************************************************** Event_SetAsButtonReleased

// set a button released event
Event Event_SetAsButtonReleased(E_GadgetID source){
    return ((Event) {.id = EVENT_BUTTON_RELEASED, .source = source, .target = GDG_NONE, .isProcessed = false});
}


// **************************************************************************** Event_SetAsScrollbarChanged

// Set a scrollbar changed event
Event Event_SetAsScrollbarChanged(E_GadgetID source, float posRatio, float sizeRatio){
    return ((Event) {.id = EVENT_SCROLLBAR_CHANGED, .source = source, .target = GDG_NONE, .isProcessed = false,
                     .data.scrollbar.posRatio = posRatio, .data.scrollbar.sizeRatio = sizeRatio});
}


// **************************************************************************** Event_SetAsUpdateScrollbar

// Set an update scrollbar event, which causes a scrollbar to change to the 
// given values
Event Event_SetAsUpdateScrollbar(E_GuiID source, E_GadgetID target, float posRatio, float sizeRatio){
    return ((Event) {.id = EVENT_UPDATE_SCROLLBAR, .source = source, .target = target, .isProcessed = false,
                     .data.scrollbar.posRatio = posRatio, .data.scrollbar.sizeRatio = sizeRatio});
}


// **************************************************************************** Event_SetAsSwitchChanged 

// Set a switch changed event
Event Event_SetAsSwitchChanged(E_GadgetID source, bool value){
    return ((Event) {.id = EVENT_SWITCH_CHANGED, .source = source, .target = GDG_NONE, .isProcessed = false,
                     .data.switchValue = value});
}


// **************************************************************************** Event_SetAsUpdateSwitch

// Set an update switch event, which causes a switch to change to the given 
// boolean value
Event Event_SetAsUpdateSwitch(E_GuiID source, E_GadgetID target, bool value){
    return ((Event) {.id = EVENT_UPDATE_SWITCH, .source = source, .target = target, .isProcessed = false,
                     .data.switchValue = value});
}


// **************************************************************************** Event_SetAsNumBoxChanged

// Set a number box changed event
Event Event_SetAsNumBoxChanged(E_GadgetID source, int value){
    return ((Event) {.id = EVENT_NUMBOX_CHANGED, .source = source, .target = GDG_NONE, .isProcessed = false,
                     .data.numboxValue = value});
}


// **************************************************************************** Event_SetAsShowPage

// Set a show page event
Event Event_SetAsShowPage(E_GuiID source, E_PageID page, bool withAnim){
    return ((Event) {.id = EVENT_SHOW_PAGE, .source = source, .target = GDG_NONE, .isProcessed = false,
                     .data.page.id = page, .data.page.withAnim = withAnim});
}


// **************************************************************************** Event_SetAsHidePage

// Set a hide page event
Event Event_SetAsHidePage(E_GuiID source, E_PageID page, bool withAnim){
    return ((Event) {.id = EVENT_HIDE_PAGE, .source = source, .target = GDG_NONE, .isProcessed = false,
                     .data.page.id = page, .data.page.withAnim = withAnim});
}


// **************************************************************************** Event_AddPageData
void Event_AddPageData(Event* event, bool victory, Time time, int nCols, int nRows){
    event->data.page.data.victory = victory;
    event->data.page.data.time    = time;
    event->data.page.data.nCols   = nCols;
    event->data.page.data.nRows   = nRows;
}


// **************************************************************************** Event_SetAsGadgetExpanded

// Set a gadget expanded event
Event Event_SetAsGadgetExpanded(E_GadgetID source){
    return ((Event) {.id = EVENT_GADGET_EXPANDED, .source = source, .target = GDG_NONE, .isProcessed = false});
}


// **************************************************************************** Event_SetAsGadgetCollapsed

// Set a gadget collapsed event
Event Event_SetAsGadgetCollapsed(E_GadgetID source){
    return ((Event) {.id = EVENT_GADGET_COLLAPSED, .source = source, .target = GDG_NONE, .isProcessed = false});
}


// **************************************************************************** Event_SetAsMakeNewGrid

// Set a make new grid event, that causes the creation of a new rod grid
Event Event_SetAsMakeNewGrid(E_GuiID source, E_GuiID target, int nCols, int nRows){
    return ((Event) {.id = EVENT_MAKE_NEW_GRID, .source = source, .target = target, .isProcessed = false,
                     .data.newGrid.nCols = nCols, .data.newGrid.nRows = nRows});
}


// **************************************************************************** Event_SetAsVictory

// Set a victory event, meant to be emitted when the grid is completed
Event Event_SetAsVictory(E_GuiID source){
    return ((Event) {.id = EVENT_VICTORY, .source = source, .target = GDG_NONE, .isProcessed = false});
}


#ifdef DEBUG_MODE
// **************************************************************************** Event_Print

    // Single line print of the event. Optionally with new line
    void Event_Print(Event event, bool withNewLine){
        if (event.id == EVENT_NONE){
            printf("%s", STR_NONE);
            if (withNewLine) {printf("\n");}
            return;
        }

        printf("%s | ", EventID_ToString(event.id));
        printf("SRC: %s | ", GuiID_ToString(event.source));
        printf("TRG: %s", GuiID_ToString(event.target));

        #define PRINT_DATA_SEP printf(" ||| ");

        switch (event.id){
            case EVENT_MOUSE_MOVE: case EVENT_MOUSE_DRAG: case EVENT_MOUSE_PRESSED:
            case EVENT_MOUSE_RELEASED:{
                PRINT_DATA_SEP
                printf("Pos: "); Geo_PrintPoint(event.data.mouse.pos, WITHOUT_NEW_LINE);
                printf(", Delta: "); Geo_PrintVector(event.data.mouse.delta, WITHOUT_NEW_LINE);
                break; 
            }

            case EVENT_MOUSE_WHEEL:{
                PRINT_DATA_SEP
                printf("Move: %.3f", event.data.wheel.move);
                printf(", Pos: "); Geo_PrintPoint(event.data.wheel.pos, WITHOUT_NEW_LINE);
                break;
            }

            case EVENT_KEY_PRESSED: case EVENT_KEY_RELEASED:{
                PRINT_DATA_SEP
                printf("Key: %s", KeyboardKey_ToString(event.data.key));
                break;
            }

            case EVENT_SCROLLBAR_CHANGED: case EVENT_UPDATE_SCROLLBAR:{
                PRINT_DATA_SEP
                printf("Pos Ratio: %.3f, ", event.data.scrollbar.posRatio);
                printf("Size Ratio: %.3f", event.data.scrollbar.sizeRatio);
                break;
            }

            case EVENT_SWITCH_CHANGED: case EVENT_UPDATE_SWITCH:{
                PRINT_DATA_SEP
                printf("Bool: %s", Bool_ToString(event.data.switchValue, LONG_FORM));
                break;
            }

            case EVENT_NUMBOX_CHANGED:{
                PRINT_DATA_SEP
                printf("Value: %d", event.data.numboxValue);
                break;
            }

            case EVENT_SHOW_PAGE: case EVENT_HIDE_PAGE:{
                PRINT_DATA_SEP
                printf("Page: %s, ", PageID_ToString(event.data.page.id));
                printf("Anim: %s", Bool_ToString(event.data.page.withAnim, LONG_FORM));
                if (event.data.page.id == PAGE_SETUP){
                    PRINT_DATA_SEP
                    printf("Victory: %s, ", Bool_ToString(event.data.page.data.victory, LONG_FORM));
                    printf("Time: "); Time_Print(event.data.page.data.time, WITHOUT_NEW_LINE);
                    printf(", Size: %d x %d", event.data.page.data.nCols, event.data.page.data.nRows);
                }
                break;
            }

            case EVENT_MAKE_NEW_GRID:{
                PRINT_DATA_SEP
                printf("Size: %d x %d", event.data.newGrid.nCols, event.data.newGrid.nRows);
                break;
            }

            default: {break;}
        }

        if (withNewLine) {printf("\n");}

        #undef PRINT_DATA_SEP
    }

#endif



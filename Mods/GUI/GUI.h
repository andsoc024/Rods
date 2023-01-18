// ============================================================================
// RODS
// GUI Header
// by Andreas Socratous
// Jan 2023
// ============================================================================


#ifndef GUI_GUARD
#define GUI_GUARD


// ============================================================================ INFO
/*
    Generic functions for working with the GUI structures.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"


// ============================================================================ FUNC DECL

// ---------------------------------------------------------------------------- Event Functions

Event           Event_SetAsMouseMove(Point pos, Vector2 delta);
Event           Event_SetAsMouseDrag(Point pos, Vector2 delta);
Event           Event_SetAsMousePressed(Point pos);
Event           Event_SetAsMouseReleased(Point pos);
Event           Event_SetAsMouseWheel(float move, Point pos);
Event           Event_SetAsKeyPressed(KeyboardKey key);
Event           Event_SetAsKeyReleased(KeyboardKey key);
Event           Event_SetAsButtonPressed(E_GadgetID source);
Event           Event_SetAsButtonReleased(E_GadgetID source);
Event           Event_SetAsScrollbarChanged(E_GadgetID source, float posRatio, float sizeRatio);
Event           Event_SetAsUpdateScrollbar(E_GuiID source, E_GadgetID target, float posRatio, float sizeRatio);
Event           Event_SetAsSwitchChanged(E_GadgetID source, bool value);
Event           Event_SetAsUpdateSwitch(E_GuiID source, E_GadgetID target, bool value);
Event           Event_SetAsNumBoxChanged(E_GadgetID source, int value);
Event           Event_SetAsShowPage(E_GuiID source, E_PageID page, bool withAnim);
Event           Event_SetAsHidePage(E_GuiID source, E_PageID page, bool withAnim);
void            Event_AddPageData(Event* event, bool victory, Time time, int nCols, int nRows);
Event           Event_SetAsGadgetExpanded(E_GadgetID source);
Event           Event_SetAsGadgetCollapsed(E_GadgetID source);
Event           Event_SetAsMakeNewGrid(E_GuiID source, E_GuiID target, int nCols, int nRows);
Event           Event_SetAsVictory(E_GuiID source);
#ifdef DEBUG_MODE
    void        Event_Print(Event event, bool withNewLine);
#endif

// ---------------------------------------------------------------------------- Event Queue Functions

EventQueue*     Queue_Make(void);
EventQueue*     Queue_Free(EventQueue* queue);
void            Queue_Clear(EventQueue* queue);
Event           Queue_GetNext(EventQueue* queue);
void            Queue_AddEvent(EventQueue* queue, Event event);
void            Queue_RemoveForTarget(EventQueue* queue, E_GuiID target);
void            Queue_SetUserInput(EventQueue* queue, Event event);
#ifdef DEBUG_MODE
    void        Queue_Print(EventQueue* queue);
#endif



#endif // GUI_GUARD


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

// ---------------------------------------------------------------------------- Input Functions

Event           Event_GetUserInput(void);

// ---------------------------------------------------------------------------- Gadget Functions

Gadget*         Gadget_Make(E_GadgetID id, E_GadgetType type, bool isSelectable, int nSubGadgets);
Gadget*         Gadget_Free(Gadget* gadget);
void            Gadget_Resize(Gadget* gadget);
void            Gadget_ReactToEvent(Gadget* gadget, Event event, EventQueue* queue);
void            Gadget_Update(Gadget* gadget, EventQueue* queue);
void            Gadget_Draw(Gadget* gadget, Vector2 shift);
bool            Gadget_SelectNext(Gadget* gadget);
void            Gadget_Deselect(Gadget* gadget);
#ifdef DEBUG_MODE
    void        Gadget_Print(Gadget* gadget);
#endif

// ---------------------------------------------------------------------------- Page Functions

Page*           Page_Make(E_PageID id);
Page*           Page_Free(Page* page);
bool            Page_AddGadget(Page* page, Gadget* gadget);
void            Page_Resize(Page* page);
void            Page_ReactToEvent(Page* page, Event event, EventQueue* queue);
void            Page_Update(Page* page, EventQueue* queue);
void            Page_Draw(Page* page);
void            Page_SelectNextGadget(Page* page);
void            Page_DeselectAllGadgets(Page* page);
void            Page_Show(Page* page, bool withAnim);
void            Page_Hide(Page* page, bool withAnim);
bool            Page_IsAnimating(Page* page);
void            Page_FinishTransitionAnim(Page* page);
#ifdef DEBUG_MODE
    void        Page_Print(Page* page);
#endif



#endif // GUI_GUARD


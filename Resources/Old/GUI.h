// ============================================================================
// RODS
// GUI Header
// by Andreas Socratous
// Sep 2022
// ============================================================================


#ifndef GUI_GUARD
#define GUI_GUARD


// ============================================================================ INFO
/*
    Functions for getting user input and managing events as well as generic 
    functions for managing the GUI structures: Gadgets, Pages and the Router.

    Gadget is the elementary unit of the GUI. They are grouped in Pages. Pages 
    are controlled by the Router.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"


// ============================================================================ FUNC DECL

// ---------------------------------------------------------------------------- Event Functions

Event         Event_SetAsMouse(E_EventID id, Point pos, Vector2 delta);
Event         Event_SetAsWheel(float move, Point pos);
Event         Event_SetAsKey(E_EventID id, KeyboardKey key);
Event         Event_SetAsPage(E_EventID id, E_GadgetID source, E_PageID pageID, bool withAnim);
Event         Event_SetAsButton(E_EventID id, E_GadgetID source);
Event         Event_SetAsScrollbar(E_GadgetID source, float posRatio, float barSizeRatio);
Event         Event_SetAsScrollbarUpdate(E_GadgetID source, E_GadgetID target, float posRatio, float barSizeRatio);
Event         Event_SetAsSwitch(E_GadgetID source, bool value);
Event         Event_SetAsSwitchUpdate(E_GadgetID source, E_GadgetID target, bool value);
Event         Event_SetAsNumBox(E_GadgetID source, int value);
Event         Event_SetAsExpand(E_GadgetID source);
Event         Event_SetAsCollapse(E_GadgetID source);
Event         Event_SetAsMakeNewGrid(E_GadgetID source);
#ifdef DEBUG_MODE
    void      Event_Print(Event event, bool withNewLine);
#endif

// ---------------------------------------------------------------------------- Input Functions

Event         Event_GetUserInput(void);

// ---------------------------------------------------------------------------- Event Queue Functions

EventQueue*   Queue_Make(void);
EventQueue*   Queue_Free(EventQueue* queue);
void          Queue_AddEvent(EventQueue* queue, Event event);
void          Queue_Clear(EventQueue* queue);
Event         Queue_GetNextEvent(EventQueue* queue);
#ifdef DEBUG_MODE
    void      Queue_Print(EventQueue* queue);
#endif

// ---------------------------------------------------------------------------- Gadget Functions

Gadget*       Gadget_Make(E_GadgetID id, Rect cRect, bool isInSelectQueue, int nSubGadgets);
Gadget*       Gadget_Free(Gadget* gadget);
bool          Gadget_SelectNext(Gadget* gadget);
void          Gadget_Deselect(Gadget* gadget);
#ifdef DEBUG_MODE
    void      Gadget_Print(Gadget* gadget);
#endif

// ---------------------------------------------------------------------------- Page Functions

Page*         Page_Free(Page* page);
void          Page_AddGadget(Page* page, Gadget* gadget);
void          Page_Reset(Page* page);
void          Page_Resize(Page* page);
void          Page_ReactToEvent(Page* page, EventQueue* queue, Event event);
void          Page_Update(Page* page, EventQueue* queue);
void          Page_Draw(Page* page);
void          Page_SelectNext(Page* page);
void          Page_DeselectAll(Page* page);
void          Page_Show(Page* page, bool withAnim);
void          Page_Hide(Page* page, bool withAnim);
void          Page_UpdateTransitionAnim(Page* page);
void          Page_FinishTransitionAnim(Page* page);
#ifdef DEBUG_MODE
    void      Page_Print(Page* page);
#endif

// ---------------------------------------------------------------------------- Router Functions

Router*       Router_Make(void);
Router*       Router_Free(Router* router);
void          Router_InitWindow(void);
void          Router_AddPage(Router* router, Page* page);
void          Router_Resize(Router* router);
void          Router_ReactToEvents(Router* router);
void          Router_Update(Router* router);
void          Router_Draw(Router* router);
void          Router_Loop(Router* router);
void          Router_SelectNext(Router* router);
void          Router_DeselectAll(Router* router);
E_PageID      Router_FindActivePage(Router* router);
#ifdef DEBUG_MODE
    void      Router_Print(Router* router);
#endif


#endif // GUI_GUARD



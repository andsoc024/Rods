// ============================================================================
// RODS
// Gadgets Header
// by Andreas Socratous
// Sep 2022
// ============================================================================


#ifndef GDG_GUARD
#define GDG_GUARD


// ============================================================================ INFO
/*
    Functions for managing specific GUI Gadgets.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"


// ============================================================================ FUNC DECL

// ---------------------------------------------------------------------------- Label Functions

Gadget*       Label_Make(E_GadgetID id, const char* txt, Rect cRect, Color col, E_RectPointType alignment);
Gadget*       MLabel_Make(E_GadgetID id, const char* txt, Rect cRect, E_RectPointType alignment);
void          Label_PrepareToFree(Gadget* label);
void          Label_Resize(Gadget* label);
void          Label_Draw(Gadget* label, Vector2 shift);
void          MLabel_Draw(Gadget* label, Vector2 shift);
void          Label_TurnToNormal(Gadget* label, Color col);
void          Label_TurnToMagic(Gadget* label);
bool          Label_IsMagic(Gadget* label);
void          Label_SetText(Gadget* label, const char* txt);
void          Label_SetAlignment(Gadget* label, E_RectPointType alignment);
void          Label_SetFontSize(Gadget* label, float fontSize);
void          Label_SetColor(Gadget* label, Color col);
#ifdef DEBUG_MODE
    void      Label_PrintData(Gadget* label);
#endif

// ---------------------------------------------------------------------------- Icon Label Functions

Gadget*       IconLabel_Make(E_GadgetID id, E_IconID icon, Rect cRect, Color col, E_RectPointType alignment);
void          IconLabel_Resize(Gadget* icLabel);
void          IconLabel_Draw(Gadget* icLabel, Vector2 shift);
void          IconLabel_SetIcon(Gadget* icLabel, E_IconID icon);
void          IconLabel_SetAlignment(Gadget* icLabel, E_RectPointType alignment);
void          IconLabel_SetColor(Gadget* icLabel, Color col);
#ifdef DEBUG_MODE
    void      IconLabel_PrintData(Gadget* icLabel);
#endif

// ---------------------------------------------------------------------------- Button Functions

Gadget*       TxtBtn_Make(E_GadgetID id, const char* txt, Rect cRect);
Gadget*       IconBtn_Make(E_GadgetID id, E_IconID icon, Rect cRect);
void          TxtBtn_PrepareToFree(Gadget* btn);
void          Btn_Reset(Gadget* btn);
void          TxtBtn_Resize(Gadget* btn);
void          IconBtn_Resize(Gadget* btn);
void          Btn_ReactToEvent(Gadget* btn, EventQueue* queue, Event event);
void          Btn_Update(Gadget* btn, EventQueue* queue);
void          TxtBtn_Draw(Gadget* btn, Vector2 shift);
void          IconBtn_Draw(Gadget* btn, Vector2 shift);
void          Btn_SetColors(Gadget* btn, Color fgCol, Color bgCol, Color selectCol);
void          Btn_SetText(Gadget* btn, const char* txt);
void          Btn_SetIcon(Gadget* btn, E_IconID icon);
void          TxtBtn_SetFontSize(Gadget* btn, float fontSize);
bool          Btn_IsIconBtn(Gadget* btn);
#ifdef DEBUG_MODE
    void      Btn_PrintData(Gadget* btn);
#endif

// ---------------------------------------------------------------------------- Switch Functions

Gadget*       Switch_Make(E_GadgetID id, Rect cRect, bool value);
void          Switch_Reset(Gadget* sw);
void          Switch_Resize(Gadget* sw);
void          Switch_ReactToEvent(Gadget* sw, EventQueue* queue, Event event);
void          Switch_Update(Gadget* sw, EventQueue* queue);
void          Switch_Draw(Gadget* sw, Vector2 shift);
void          Switch_SetValue(Gadget* sw, bool value, bool withAnim);
bool          Switch_Toggle(Gadget* sw, bool withAnim);
bool          Switch_GetValue(Gadget* sw);
void          Switch_SetColors(Gadget* sw, Color fg, Color handle, Color select, Color active, Color inactive);
#ifdef DEBUG_MODE
    void      Switch_PrintData(Gadget* sw);
#endif

// ---------------------------------------------------------------------------- Number Box Functions

Gadget*       NumBox_Make(E_GadgetID id, E_GadgetID btnUpID, E_GadgetID btnDownID, Rect cRect, int value, int minV, int maxV);
void          NumBox_PrepareToFree(Gadget* numbox);
void          NumBox_Reset(Gadget* numbox);
void          NumBox_Resize(Gadget* numbox);
void          NumBox_ReactToEvents(Gadget* numbox, EventQueue* queue, Event event);
void          NumBox_Update(Gadget* numbox, EventQueue* queue);
void          NumBox_Draw(Gadget* numbox, Vector2 shift);
void          NumBox_SetValue(Gadget* numbox, int value);
int           NumBox_GetValue(Gadget* numbox);
void          NumBox_SetLimits(Gadget* numbox, int minV, int maxV);
void          NumBox_SetColors(Gadget* numbox, Color fg, Color txt, Color bg, Color btnTxt, Color btn, Color select);
#ifdef DEBUG_MODE
    void      NumBox_PrintData(Gadget* numbox);
#endif

// ---------------------------------------------------------------------------- Timer Functions

Gadget*       Timer_MakeShort(E_GadgetID id, ElTime elTime, Rect cRect);
Gadget*       Timer_MakeLong(E_GadgetID id, ElTime elTime, ElTime recordElTime, Rect cRect);
void          Timer_PrepareToFree(Gadget* timer);
void          Timer_ResizeShort(Gadget* timer);
void          Timer_ResizeLong(Gadget* timer);
void          Timer_Update(Gadget* timer, EventQueue* queue);
void          Timer_DrawShort(Gadget* timer, Vector2 shift);
void          Timer_DrawLong(Gadget* timer, Vector2 shift);
void          Timer_SetCurrent(Gadget* timer, ElTime t);
void          Timer_SetRecord(Gadget* timer, ElTime t);
ElTime        Timer_GetCurrent(Gadget* timer);
ElTime        Timer_GetRecord(Gadget* timer);
void          Timer_Pause(Gadget* timer);
void          Timer_Unpause(Gadget* timer);
bool          Timer_IsPaused(Gadget* timer);
void          Timer_SetForm(Gadget* timer, bool form);
bool          Timer_IsShortForm(Gadget* timer);
void          Timer_SetColors(Gadget* timer, Color current, Color record, Color medal);
#ifdef DEBUG_MODE
    void      Timer_PrintData(Gadget* timer);
#endif  

// ---------------------------------------------------------------------------- Scrollbar Functions

Gadget*       SBar_Make(E_GadgetID id, E_GadgetID btnLeft, E_GadgetID btnRight, E_Oriantation orientation,
                        float minX, float maxX, float handleSizeRatio, float handlePosRatio);
void          SBar_Reset(Gadget* sbar);
void          SBar_Resize(Gadget* sbar);
void          SBar_ReactToEvent(Gadget* sbar, EventQueue* queue, Event event);
void          SBar_Update(Gadget* sbar, EventQueue* queue);
void          SBar_Draw(Gadget* sbar, Vector2 shift);
void          SBar_SetLimits(Gadget* sbar, float minX, float maxX);
void          SBar_Expand(Gadget* sbar);
void          SBar_Collapse(Gadget* sbar);
bool          SBar_IsExpanded(Gadget* sbar);
float         SBar_GetPosRatio(Gadget* sbar);
float         SBar_GetSizeRatio(Gadget* sbar);
void          SBar_Set(Gadget* sbar, float posRatio, float handleRatio);
void          SBar_SetColors(Gadget* sbar, Color fg, Color handle, Color bg, Color select);
#ifdef DEBUG_MODE
    void      SBar_PrintData(Gadget* sbar);
#endif

// ---------------------------------------------------------------------------- Toolbar Functions

Gadget*       Toolbar_Make(void);
void          Toolbar_Reset(Gadget* toolbar);
void          Toolbar_Resize(Gadget* toolbar);
void          Toolbar_ReactToEvent(Gadget* toolbar, EventQueue* queue, Event event);
void          Toolbar_Update(Gadget* toolbar, EventQueue* queue);
void          Toolbar_Draw(Gadget* toolbar, Vector2 shift);
void          Toolbar_Expand(Gadget* toolbar, bool withAnim);
void          Toolbar_Collapse(Gadget* toolbar, bool withAnim);
void          Toolbar_FinishAnim(Gadget* toolbar);
#ifdef DEBUG_MODE
    void      Toolbar_PrintData(Gadget* toolbar);
#endif

// ---------------------------------------------------------------------------- Table Functions

Gadget*       Table_Make(E_GadgetID id, Rect cRect, Color col1, Color col2, int nCells, ...);
void          Table_PrepareToFree(Gadget* table);
void          Table_Resize(Gadget* table);
void          Table_Draw(Gadget* table, Vector2 shift);
void          Table_SetCellContent(Gadget* table, int column, int row, const char* str);
void          Table_SetFontSize(Gadget* table, float fontSize);
void          Table_SetColumnAlignment(Gadget* gadget, E_RectPointType alignment1, E_RectPointType alignment2);
void          Table_SetColors(Gadget* table, Color col1, Color col2);
#ifdef DEBUG_MODE
    void      Table_PrintData(Gadget* table);
#endif

// ---------------------------------------------------------------------------- Board Functions

Gadget*       Board_MakeEmpty(E_GadgetID id, int nCols, int nRows);
void          Board_CreateMap(Gadget* board, int nCols, int nRows);
void          Board_SetOnSource(Gadget* board);
void          Board_PrepareToFree(Gadget* board);
void          Board_Reset(Gadget* board);
void          Board_Resize(Gadget* board);
void          Board_ReactToEvent(Gadget* board, EventQueue* queue, Event event);
void          Board_Update(Gadget* board, EventQueue* queue);
void          Board_Draw(Gadget* board, Vector2 shift);
Grid          Board_GetGridSize(Gadget* board);
int           Board_GetNumUnelectrifiedRods(Gadget* board);
bool          Board_GridIsCompleted(Gadget* board);
void          Board_UpdateScrollbars(Gadget* board, Gadget* horSB, Gadget* verSB);
void          Board_Recalc(Gadget* board);
void          Board_AddScrollbarUpdateEvents(Gadget* board, EventQueue* queue);
#ifdef DEBUG_MODE
    void      Board_PrintData(Gadget* board);
#endif


#endif // GDG_GUARD



// ============================================================================
// RODS
// Label
// by Andreas Socratous
// Jan 2023
// ============================================================================


#ifndef GADGETS_GUARD
#define GADGETS_GUARD


// ============================================================================ INFO
/*
    Functions for managing the various types of gadgets.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"


// ============================================================================ FUNC DECL

// ---------------------------------------------------------------------------- Label Functions

Gadget*         Label_Make(E_GadgetID id, const char* txt, Color color, E_RectPointType alignment);
Gadget*         Label_MakeMagic(E_GadgetID id, const char* txt, E_RectPointType alignment);
void            Label_SetColorAsMagic(Gadget* label);
void            Label_SetColor(Gadget* label, Color color);
void            Label_SetText(Gadget* label, const char* txt);
void            Label_SetFontSize(Gadget* label, float fontSize);
void            Label_SetAlignment(Gadget* label, E_RectPointType alignment);
const char*     Label_GetText(const Gadget* label);
Color           Label_GetColor(const Gadget* label);
bool            Label_IsMagic(const Gadget* label);
int             Label_GetAlignment(const Gadget* label);
int             Label_GetFontSize(const Gadget* label);
Point           Label_GetTextPos(const Gadget* label);

// ---------------------------------------------------------------------------- Icon Label Functions

Gadget*         IconLabel_Make(E_GadgetID id, E_IconID icon, Color color, E_RectPointType alignment);
void            IconLabel_SetIcon(Gadget* iconLabel, E_IconID icon);
void            IconLabel_SetColor(Gadget* iconLabel, Color color);
void            IconLabel_SetSize(Gadget* iconLabel, float size);
void            IconLabel_SetAlignment(Gadget* iconLabel, E_RectPointType alignment);
int             IconLabel_GetIconID(const Gadget* iconLabel);
Color           IconLabel_GetColor(const Gadget* iconLabel);
float           IconLabel_GetScaleF(const Gadget* iconLabel);
float           IconLabel_GetSize(const Gadget* iconLabel);
int             IconLabel_GetAlignment(const Gadget* iconLabel);

// ---------------------------------------------------------------------------- Button Functions

Gadget*         Button_MakeAsText(E_GadgetID id, const char* txt);
Gadget*         Button_MakeAsIcon(E_GadgetID id, E_IconID icon);
void            Button_SetText(Gadget* button, const char* txt);
void            Button_SetIcon(Gadget* button, E_IconID icon);
void            Button_SetColors(Gadget* button, Color colBG, Color colFG, Color colEmph);
void            Button_SetRoundness(Gadget* button, float roundness);
void            Button_SetOutlineThickness(Gadget* button, float thickness);
bool            Button_HasText(const Gadget* button);
bool            Button_HasIcon(const Gadget* button);
float           Button_GetRoundness(const Gadget* button);
float           Button_GetOutlineThickness(const Gadget* button);
Gadget*         Button_GetContent(const Gadget* button);

// ---------------------------------------------------------------------------- Number Box Functions

Gadget*         NumBox_Make(E_GadgetID id, E_GadgetID upButtonID, E_GadgetID downButtonID);
void            NumBox_SetValue(Gadget* numbox, int value);
void            NumBox_SetLimits(Gadget* numbox, int minValue, int maxValue);
void            NumBox_SetAlignment(Gadget* numbox, E_RectPointType alignment);
void            NumBox_SetOutlineThickness(Gadget* numbox, float thickness);
int             NumBox_GetValue(const Gadget* numbox);
int             NumBox_GetMinValue(const Gadget* numbox);
int             NumBox_GetMaxValue(const Gadget* numbox);
int             NumBox_GetAlignment(const Gadget* numbox);
Gadget*         NumBox_GetUpButton(const Gadget* numbox);
Gadget*         NumBox_GetDownButton(const Gadget* numbox);

// ---------------------------------------------------------------------------- Switch Functions

Gadget*         Switch_Make(E_GadgetID id, bool value);
void            Switch_SetValue(Gadget* sw, bool value, bool withAnim);
void            Switch_ToggleValue(Gadget* sw, bool withAnim);
void            Switch_FinishAnim(Gadget* sw);
void            Switch_SetAlignment(Gadget* sw, E_RectPointType alignment);
bool            Switch_GetValue(const Gadget* sw);
bool            Switch_IsAnimating(const Gadget* sw);
int             Switch_GetAlignment(const Gadget* sw);

// ---------------------------------------------------------------------------- Scrollbar Functions

Gadget*         Scrollbar_Make(E_GadgetID id, E_Orientation orientation);
void            Scrollbar_SetPosFromRatio(Gadget* sbar, float posRatio);
void            Scrollbar_SetSizeFromRatio(Gadget* sbar, float sizeRatio);
float           Scrollbar_GetPosRatio(const Gadget* sbar);
float           Scrollbar_GetSizeRatio(const Gadget* sbar);
Rect            Scrollbar_GetBarRect(const Gadget* sbar);
Rect            Scrollbar_GetExpandedRect(const Gadget* sbar);
Rect            Scrollbar_GetCollapsedRect(const Gadget* sbar);

// ---------------------------------------------------------------------------- Timer Functions

Gadget*         Timer_Make(E_GadgetID id, Time currentTime, Time recordTime);
void            Timer_Pause(Gadget* timer);
void            Timer_Resume(Gadget* timer);
bool            Timer_Toggle(Gadget* timer);
void            Timer_SetTime(Gadget* timer, Time time);
void            Timer_SetRecordTime(Gadget* timer, Time recordTime);
void            Timer_Expand(Gadget* timer);
void            Timer_Collapse(Gadget* timer);
void            Timer_SetColors(Gadget* timer, Color currentColor, Color recordColor, Color iconColor);
bool            Timer_IsPaused(const Gadget* timer);
Time            Timer_GetTime(const Gadget* timer);
Time            Timer_GetRecordTime(const Gadget* timer);

// ---------------------------------------------------------------------------- Toolbar Functions

Gadget*         Toolbar_Make(void);
void            Toolbar_Expand(Gadget* toolbar, bool withAnim);
void            Toolbar_Collapse(Gadget* toolbar, bool withAnim);
void            Toolbar_FinishAnim(Gadget* toolbar);
void            Toolbar_SetTime(Gadget* toolbar, Time time);
void            Toolbar_SetRecordTime(Gadget* toolbar, Time time);
void            Toolbar_SetRodsLeft(Gadget* toolbar, int rodsLeft);
void            Toolbar_SetSoundSwitch(Gadget* toolbar, bool switchState);
bool            Toolbar_IsAnimating(const Gadget* toolbar);
Time            Toolbar_GetTime(const Gadget* toolbar);
Time            Toolbar_GetRecordTime(const Gadget* toolbar);
bool            Toolbar_GetSoundSwitchState(const Gadget* toolbar);
Gadget*         Toolbar_GetGadget(const Gadget* toolbar, int index);

// ---------------------------------------------------------------------------- Table Functions

Gadget*         Table_Make(E_GadgetID id, int nCols, int nRows, const char** strings);
void            Table_SetText(Gadget* table, int nCols, int nRows, const char* txt);
void            Table_SetFontSize(Gadget* table, float fontSize, int indicator, ...);
void            Table_SetColor(Gadget* table, Color color, int indicator, ...);
void            Table_SetAlignment(Gadget* table, E_RectPointType alignment, int indicator, ...);
const char*     Table_GetText(const Gadget* table, int nCols, int nRows);
float           Table_GetFontSize(const Gadget* table, int nCols, int nRows);
Color           Table_GetColor(const Gadget* table, int nCols, int nRows);
int             Table_GetAlignment(const Gadget* table, int nCols, int nRows);

// ---------------------------------------------------------------------------- Board Functions

Gadget*         Board_Make(E_GadgetID id, const PData* pData);
void            Board_CreateNewRodGrid(Gadget* board, int nCols, int nRows);
void            Board_SetAsReactive(Gadget* board);
void            Board_SetAsNotReactive(Gadget* board);
int             Board_GetNumElectrifiedRods(const Gadget* board);
int             Board_GetNumUnelectrifiedRodsLeft(const Gadget* board);
int             Board_GetTotalNumRods(const Gadget* board);
Grid            Board_GetGridSize(const Gadget* board);
GNode           Board_GetSelBox(const Gadget* board);
RGrid*          Board_GetRGrid(const Gadget* board);
SGraph*         Board_GetSGraph(const Gadget* board);
RodModel*       Board_GetRodModel(const Gadget* board);
bool            Board_IsReactive(const Gadget* board);


#endif // GADGETS_GUARD


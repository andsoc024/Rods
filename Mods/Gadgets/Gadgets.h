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





#endif // GADGETS_GUARD


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







#endif // GADGETS_GUARD


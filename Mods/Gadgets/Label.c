// ============================================================================
// RODS
// Label
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions as well as structure and constants definitions for the label 
    gadget.

    The label draws static text. The color can be constant or it can be the 
    current color of the default magic color Glo_MCol.
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

// **************************************************************************** LDATA

// Pointer to the label's data object
#define LDATA \
    ((LabelData*) (label->data))


// ============================================================================ PRIVATE CONSTANTS

#define MIN_FONT_SIZE                       5.0f


// ============================================================================ PRIVATE STRUCTURES

// **************************************************************************** LabelData

// The data object of the label
typedef struct LabelData{
    char* txt;
    float fontSize;
    Point pos;
    E_RectPointType alignment;
    Color color;
}LabelData;


// ============================================================================ PRIVATE FUNC DECL

static void     Label_PrepareToFree(Gadget* label);
static void     Label_Resize(Gadget* label);
static void     Label_Draw(const Gadget* label, Vector2 shift);
static void     Label_DrawMagic(const Gadget* label, Vector2 shift);
#ifdef DEBUG_MODE
    static void Label_PrintData(const Gadget* label);
#endif






// ============================================================================ FUNC DEF

// **************************************************************************** Label_Make

// Make a label. It needs to be resized before its first use
Gadget* Label_Make(E_GadgetID id, const char* txt, Color color, E_RectPointType alignment){
    Gadget* label = Gadget_Make(id, GT_LABEL, IS_NOT_SELECTABLE, 0);

    label->PrepareToFree = Label_PrepareToFree;
    label->Resize        = Label_Resize;
    label->Draw          = Label_Draw;
    #ifdef DEBUG_MODE
        label->PrintData = Label_PrintData;
    #endif

    LabelData* data = Memory_Allocate(NULL, sizeof(LabelData), ZEROVAL_ALL);

    data->txt = String_Copy(NULL, txt);
    data->color = color;
    data->alignment = alignment;

    label->data = data;

    return label;
}


// **************************************************************************** Label_MakeMagic

// Make a magic label, which has the current color of Glo_MCol. It needs to be 
// resized befor its first use
Gadget* Label_MakeMagic(E_GadgetID id, const char* txt, E_RectPointType alignment){
    Gadget* label = Label_Make(id, txt, COL_NULL, alignment);
    Label_SetColorAsMagic(label);
    return label;
}


// **************************************************************************** Label_SetColorAsMagic

// Make the label take automatically the current color of Glo_MCol
void Label_SetColorAsMagic(Gadget* label){
    LDATA->color = COL_NULL;
    label->Draw = Label_DrawMagic;
}


// **************************************************************************** Label_SetColorAsConstant

// Set the label color to the given color
void Label_SetColor(Gadget* label, Color color){
    LDATA->color = color;
    label->Draw = Label_Draw;
}


// **************************************************************************** Label_SetText

// Change the text of the label
void Label_SetText(Gadget* label, const char* txt){
    LDATA->txt = String_Copy(LDATA->txt, txt);
    Gadget_Resize(label);
}


// **************************************************************************** Label_SetFontSize

// Change the font size of the label
void Label_SetFontSize(Gadget* label, float fontSize){
    fontSize = MAX(fontSize, MIN_FONT_SIZE);
    LDATA->fontSize = fontSize;

    Point p = Geo_RectPoint(label->cRect, LDATA->alignment);
    LDATA->pos = Font_CalcTextPos(LDATA->txt, LDATA->fontSize, p, LDATA->alignment);
}


// **************************************************************************** Label_SetAlignment

// Change the alignment of the label, inside its containing size
void Label_SetAlignment(Gadget* label, E_RectPointType alignment){
    LDATA->alignment = alignment;
    Gadget_Resize(label);
}


// **************************************************************************** Label_GetText

// Return a constant pointer to the label text
const char* Label_GetText(const Gadget* label){
    return LDATA->txt;
}


// **************************************************************************** Label_GetColor

// Return the label's color
Color Label_GetColor(const Gadget* label){
    return Label_IsMagic(label) ? MCol(Glo_MCol) : LDATA->color;
}


// **************************************************************************** Label_IsMagic

// Return true if the label takes the color of the magic color at Glo_MCol
bool Label_IsMagic(const Gadget* label){
    return (label->Draw == Label_DrawMagic);
}


// **************************************************************************** Label_GetAlignment

// Return the alignment of the label inside its containing rectangle
int Label_GetAlignment(const Gadget* label){
    return LDATA->alignment;
}


// **************************************************************************** Label_GetFontSize

// Return the font size of the label
int Label_GetFontSize(const Gadget* label){
    return LDATA->fontSize;
}


// **************************************************************************** Label_GetTextPos

// Return the position of the label
Point Label_GetTextPos(const Gadget* label){
    return LDATA->pos;
}






// ============================================================================ PRIVATE FUNC DEF

// **************************************************************************** Label_PrepareToFree

// Free the text in the data object
static void Label_PrepareToFree(Gadget* label){
    LDATA->txt = Memory_Free(LDATA->txt);
}


// **************************************************************************** Label_Resize

// Determine the font size and position of the label, inside its containing 
// rectangle
static void Label_Resize(Gadget* label){
    LDATA->fontSize = Font_FitTextInSize(LDATA->txt, RSIZE(label->cRect));
    Point p = Geo_RectPoint(label->cRect, LDATA->alignment);
    LDATA->pos = Font_CalcTextPos(LDATA->txt, LDATA->fontSize, p, LDATA->alignment);
}


// **************************************************************************** Label_Draw

// Draw the label text
static void Label_Draw(const Gadget* label, Vector2 shift){
    Font_DrawText(LDATA->txt, LDATA->fontSize, Geo_TranslatePoint(LDATA->pos, shift), 
                  LDATA->color);
}


// **************************************************************************** Label_DrawMagic

// Draw the label text with the current color of the default magic color at 
// Glo_MCol
static void Label_DrawMagic(const Gadget* label, Vector2 shift){
    Font_DrawText(LDATA->txt, LDATA->fontSize, Geo_TranslatePoint(LDATA->pos, shift), 
                  MCol(Glo_MCol));
}


#ifdef DEBUG_MODE
// **************************************************************************** Label_PrintData

    // Multiline print of the parameter's of the label's data object
    static void Label_PrintData(const Gadget* label){
        CHECK_NULL(label, WITH_NEW_LINE)
        CHECK_NULL(label->data, WITH_NEW_LINE)

        printf("Text:      \"%s\"\n", LDATA->txt);
        printf("Font Size: %.3f\n", LDATA->fontSize);
        printf("Position:  "); Geo_PrintPoint(LDATA->pos, WITH_NEW_LINE);
        printf("Color:     "); Color_Print(LDATA->color, WITH_NEW_LINE);
        printf("Alignment: %s (%d)\n", RectPointType_ToString(LDATA->alignment), LDATA->alignment);
        printf("Is Magic:  %s\n", Bool_ToString(Label_IsMagic(label), LONG_FORM));
    }
#endif




// ============================================================================
// RODS
// Icon Label
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions as well as structure and constants definitions for the icon label 
    gadget.

    The icon label draws an image icon in the window.
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

// **************************************************************************** ILDATA

// Pointer to the icon label's data object
#define ILDATA \
    ((IconLabelData*) iconLabel->data)


// ============================================================================ PRIVATE STRUCTURES

// **************************************************************************** IconLabelData

// The data object of the icon label
typedef struct IconLabelData{
    E_IconID iconID;
    E_RectPointType alignment;
    Color color;
    float scaleF;
    Point pos;
}IconLabelData;


// ============================================================================ PRIVATE FUNC DECL

static void     IconLabel_Resize(Gadget* iconLabel);
static void     IconLabel_Draw(const Gadget* iconLabel, Vector2 shift);
#ifdef DEBUG_MODE
    static void IconLabel_PrintData(const Gadget* iconLabel);
#endif






// ============================================================================ FUNC DEF

// **************************************************************************** IconLabel_Make

// Make an icon label. The gadget needs to be resized before used for the first 
// time
Gadget* IconLabel_Make(E_GadgetID id, E_IconID icon, Color color, E_RectPointType alignment){
    Gadget* iconLabel = Gadget_Make(id, GT_ICON, IS_NOT_SELECTABLE, 0);

    iconLabel->Resize        = IconLabel_Resize;
    iconLabel->Draw          = IconLabel_Draw;
    #ifdef DEBUG_MODE
        iconLabel->PrintData = IconLabel_PrintData;
    #endif

    IconLabelData* data = Memory_Allocate(NULL, sizeof(IconLabelData), ZEROVAL_ALL);

    data->iconID = icon;
    data->color = color;
    data->alignment = alignment;

    iconLabel->data = data;

    return iconLabel;
}


// **************************************************************************** IconLabel_SetIcon

// Set the icon of the icon label
void IconLabel_SetIcon(Gadget* iconLabel, E_IconID icon){
    ILDATA->iconID = icon;
}


// **************************************************************************** IconLabel_SetColor

// Set the color of the icon label
void IconLabel_SetColor(Gadget* iconLabel, Color color){
    ILDATA->color = color;
}


// **************************************************************************** IconLabel_SetSize

// Set the size of the icon label
void IconLabel_SetSize(Gadget* iconLabel, float size){
    size = MAX(size, 0);
    ILDATA->pos = RORIGIN(Geo_AlignRect(RECT(0, 0, size, size), iconLabel->cRect, ILDATA->alignment));
    ILDATA->scaleF = size / ICON_DEF_TEXTURE_SIZE;
}


// **************************************************************************** IconLabel_SetAlignment

// Set the alignment of the icon label inside its containing rectangle
void IconLabel_SetAlignment(Gadget* iconLabel, E_RectPointType alignment){
    ILDATA->alignment = alignment;
    Gadget_Resize(iconLabel);
}


// **************************************************************************** IconLabel_GetIconID

// Return the id of the icon of the icon label
int IconLabel_GetIconID(const Gadget* iconLabel){
    return ILDATA->iconID;
}


// **************************************************************************** IconLabel_GetColor

// Return the color of the icon label
Color IconLabel_GetColor(const Gadget* iconLabel){
    return ILDATA->color;
}


// **************************************************************************** IconLabel_GetScaleF

// Return the scale factor of the icon label to the default icon texture size
float IconLabel_GetScaleF(const Gadget* iconLabel){
    return ILDATA->scaleF;
}


// **************************************************************************** IconLabel_GetSize

// Return the size of the icon of the icon label
float IconLabel_GetSize(const Gadget* iconLabel){
    return ILDATA->scaleF * ICON_DEF_TEXTURE_SIZE;
}


// **************************************************************************** IconLabel_GetAlignment

// Return the alignment of the icon inside the containing rectangle
int IconLabel_GetAlignment(const Gadget* iconLabel){
    return ILDATA->alignment;
}






// ============================================================================ PRIVATE FUNC DEF

// **************************************************************************** IconLabel_Resize

// Resize the icon label inside its containing rectangle
static void IconLabel_Resize(Gadget* iconLabel){
    float size = MIN_DIM(iconLabel->cRect);
    ILDATA->pos = RORIGIN(Geo_AlignRect(TO_RECT(SIZE_SQR(size)), iconLabel->cRect, ILDATA->alignment));
    ILDATA->scaleF = size / ICON_DEF_TEXTURE_SIZE;
}


// **************************************************************************** IconLabel_Draw

// Draw the icon
static void IconLabel_Draw(const Gadget* iconLabel, Vector2 shift){
    DrawTextureEx(Glo_Textures.icons[ILDATA->iconID], 
                  Geo_TranslatePoint(ILDATA->pos, shift), 
                  0.0f, 
                  ILDATA->scaleF, 
                  ILDATA->color);
}


#ifdef DEBUG_MODE
// **************************************************************************** IconLabel_PrintData

    // Multiline print of the parameters of the icon label's data object
    static void IconLabel_PrintData(const Gadget* iconLabel){
        CHECK_NULL(iconLabel, WITH_NEW_LINE)
        CHECK_NULL(iconLabel->data, WITH_NEW_LINE)

        printf("Icon ID:      %s (%d)\n", IconID_ToString(ILDATA->iconID), ILDATA->iconID);
        printf("Alignment:    %s (%d)\n", RectPointType_ToString(ILDATA->alignment), ILDATA->alignment);
        printf("Color:        "); Color_Print(ILDATA->color, WITH_NEW_LINE);
        printf("Scale Factor: %.3f\n", ILDATA->scaleF);
        printf("Position:     "); Geo_PrintPoint(ILDATA->pos, WITH_NEW_LINE);
    }
#endif





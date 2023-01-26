// ============================================================================
// RODS
// Number Box
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions and definitions of structures and constants for the number box 
    gadget.

    The number box (NumBox) is a gadget that shows a number value, which can be 
    increased or decreased with its two buttons. When its value changes it 
    emits the EVENT_NUMBOX_CHANGED.
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

// **************************************************************************** NBDATA

// Pointer to the data object of the number box
#define NBDATA \
    ((NumBoxData*) numbox->data)


// **************************************************************************** NB_BTN_UP

// Pointer to the up button
#define NB_BTN_UP \
    numbox->subGadgets[0]


// **************************************************************************** NB_BTN_DOWN

// Pointer to the down button
#define NB_BTN_DOWN \
    numbox->subGadgets[1]


// ============================================================================ PRIVATE CONSTANTS

#define NB_BTN_ROUNDNESS                    0.1f
#define NB_BOX_ROUNDNESS                    0.1f
#define NB_DEF_MIN_VALUE                    RGRID_MIN_SIZE
#define NB_DEF_MAX_VALUE                    RGRID_MAX_SIZE

#define NB_ASPECT_RATIO                     1.5f
#define NB_DEF_THICKNESS                    3.0f

#define COL_NB_BOX_BG                       COL_UI_BG_QUATENARY
#define COL_NB_TXT                          COL_UI_FG_SECONDARY

#define NB_WIDEST_CHAR                      '9'
#define NB_WIDEST_CHAR_STR                  ((char[]) {NB_WIDEST_CHAR, '\0'})
#define NB_DIGITS_MAX_N                     4 

#define NB_RECALC_FONT_SIZE                 true
#define NB_RECALC_NOT_FONT_SIZE             false


// ============================================================================ PRIVATE STRUCTURES

// **************************************************************************** NumBoxData

// The data object of the number box
typedef struct NumBoxData{
    int value;
    int minValue;
    int maxValue;
    char* txt;
    Rect boxRect;
    float thickness;
    float txtFontSize;
    Point txtPos;
    E_RectPointType alignment;
}NumBoxData;


// ============================================================================ PRIVATE FUNC DECL

static void     NumBox_PrepareToFree(Gadget* numbox);
static void     NumBox_Resize(Gadget* numbox);
static void     NumBox_ReactToEvent(Gadget* numbox, Event event, EventQueue* queue);
static void     NumBox_Draw(const Gadget* numbox, Vector2 shift);
static void     NumBox_RecalcTextForValue(Gadget* numbox, bool recalcFontSize);
#ifdef DEBUG_MODE
    static void NumBox_PrintData(const Gadget* numbox);
#endif






// ============================================================================ FUNC DEF

// **************************************************************************** NumBox_Make

// Make a number box gadget. It has to be resized before its first use
Gadget* NumBox_Make(E_GadgetID id, E_GadgetID upButtonID, E_GadgetID downButtonID){
    Gadget* numbox = Gadget_Make(id, GT_NUMBOX, IS_SELECTABLE, 2);

    Gadget* btnUp   = Button_MakeAsIcon(upButtonID,   ICON_ARROW_UP);
    Button_SetRoundness(btnUp,   NB_BTN_ROUNDNESS);
    numbox->subGadgets[0] = btnUp;

    Gadget* btnDown = Button_MakeAsIcon(downButtonID, ICON_ARROW_DOWN);
    Button_SetRoundness(btnDown, NB_BTN_ROUNDNESS);
    numbox->subGadgets[1] = btnDown;

    numbox->PrepareToFree = NumBox_PrepareToFree;
    numbox->Resize        = NumBox_Resize;
    numbox->ReactToEvent  = NumBox_ReactToEvent;
    numbox->Draw          = NumBox_Draw;
    #ifdef DEBUG_MODE
        numbox->PrintData = NumBox_PrintData;
    #endif

    NumBoxData* data = Memory_Allocate(NULL, sizeof(NumBoxData), ZEROVAL_ALL);

    data->minValue = NB_DEF_MIN_VALUE;
    data->maxValue = NB_DEF_MAX_VALUE;
    data->value = data->minValue;

    data->alignment = RP_CENTER;
    data->thickness = NB_DEF_THICKNESS;
    data->txtFontSize = 0.0f;

    numbox->data = data;

    return numbox;
}


// **************************************************************************** NumBox_SetValue

// Change the value displayed in the number box
void NumBox_SetValue(Gadget* numbox, int value){
    value = PUT_IN_RANGE(value, NBDATA->minValue, NBDATA->maxValue);
    NBDATA->value = value;

    NumBox_RecalcTextForValue(numbox, NB_RECALC_NOT_FONT_SIZE);
}


// **************************************************************************** NumBox_SetLimits

// Set the minimum and maximum allowed value
void NumBox_SetLimits(Gadget* numbox, int minValue, int maxValue){
    maxValue = MAX(maxValue, minValue);
    NBDATA->minValue = minValue;
    NBDATA->maxValue = maxValue;

    NBDATA->value = PUT_IN_RANGE(NBDATA->value, minValue, maxValue);
    NumBox_RecalcTextForValue(numbox, NB_RECALC_NOT_FONT_SIZE);
}


// **************************************************************************** NumBox_SetAlignment

// Change the alignment of the number box within its containing box
void NumBox_SetAlignment(Gadget* numbox, E_RectPointType alignment){
    NBDATA->alignment = alignment;
    Gadget_Resize(numbox);
}


// **************************************************************************** NumBox_SetThickness

// Set the outline thickness of the box and buttons
void NumBox_SetOutlineThickness(Gadget* numbox, float thickness){
    NBDATA->thickness = thickness;
    Button_SetOutlineThickness(NB_BTN_UP,   thickness);
    Button_SetOutlineThickness(NB_BTN_DOWN, thickness);
}
// **************************************************************************** NumBox_GetValue

// The current value of the number box
int NumBox_GetValue(const Gadget* numbox){
    return NBDATA->value;
}


// **************************************************************************** NumBox_GetMinValue

// The minimum allowed value in the number box
int NumBox_GetMinValue(const Gadget* numbox){
    return NBDATA->minValue;
}


// **************************************************************************** NumBox_GetMaxValue

// The maximum allowed value in the number box
int NumBox_GetMaxValue(const Gadget* numbox){
    return NBDATA->maxValue;
}


// **************************************************************************** NumBox_GetAlignment

// The alignment of the number box within its containing rectangle
int NumBox_GetAlignment(const Gadget* numbox){
    return NBDATA->alignment;
}


// **************************************************************************** NumBox_GetUpButton

// Return a pointer to the up button of the number box
Gadget* NumBox_GetUpButton(const Gadget* numbox){
    return NB_BTN_UP;
}


// **************************************************************************** NumBox_GetDownButton

// Return a pointer to the down button of the number box
Gadget* NumBox_GetDownButton(const Gadget* numbox){
    return NB_BTN_DOWN;
}






// ============================================================================ PRIVATE FUNC DEF

// **************************************************************************** NumBox_PrepareToFree

// Free the text in the data object
static void NumBox_PrepareToFree(Gadget* numbox){
    NBDATA->txt = Memory_Free(NBDATA->txt);
}


// **************************************************************************** NumBox_Resize

// Resize the number box, within its containing rectangle
static void NumBox_Resize(Gadget* numbox){
    Size totalSize = SIZE(NB_ASPECT_RATIO, 1.0f);
    totalSize = Geo_FitSizeInSize(totalSize, RSIZE(numbox->cRect), 0.0f);
    Rect totalRect = Geo_AlignRect(TO_RECT(totalSize), numbox->cRect, NBDATA->alignment);

    NBDATA->boxRect = RECT(totalRect.x, totalRect.y, totalRect.height, totalRect.height);

    Size btnSize = SIZE_SQR(totalRect.height * 0.5f);
    NB_BTN_UP->cRect   = Geo_SetRectPS(Geo_RectPoint(NBDATA->boxRect, RP_TOP_RIGHT),    btnSize, RP_TOP_LEFT);
    NB_BTN_DOWN->cRect = Geo_SetRectPS(Geo_RectPoint(NBDATA->boxRect, RP_BOTTOM_RIGHT), btnSize, RP_BOTTOM_LEFT);

    NumBox_RecalcTextForValue(numbox, NB_RECALC_FONT_SIZE);
}


// **************************************************************************** NumBox_ReactToEvent

// The number box reacts to its buttons being pressed and emits 
// EVENT_NUMBOX_CHANGED
static void NumBox_ReactToEvent(Gadget* numbox, Event event, EventQueue* queue){
    if (event.id == EVENT_BUTTON_PRESSED && (event.source == (int) NB_BTN_UP->id || 
                                             event.source == (int) NB_BTN_DOWN->id)){
        int previous = NBDATA->value;
        NBDATA->value += ((event.source == (int) NB_BTN_UP->id) ? 1 : (-1));
        NBDATA->value = PUT_IN_RANGE(NBDATA->value, NBDATA->minValue, NBDATA->maxValue);
        if (NBDATA->value != previous){
            NumBox_RecalcTextForValue(numbox, NB_RECALC_NOT_FONT_SIZE);
            Queue_AddEvent(queue, Event_SetAsNumBoxChanged(numbox->id, NBDATA->value));
        }
    }
}

// **************************************************************************** NumBox_Draw

// Draw the box, with the text, of the number box
static void NumBox_Draw(const Gadget* numbox, Vector2 shift){
    Shape_DrawOutlinedRoundedRect(Geo_TranslateRect(NBDATA->boxRect, shift), 
                                  NB_BOX_ROUNDNESS, 
                                  NBDATA->thickness, 
                                  COL_NB_BOX_BG, 
                                  COL_NB_TXT);

    Font_DrawText(NBDATA->txt, 
                  NBDATA->txtFontSize, 
                  Geo_TranslatePoint(NBDATA->txtPos, shift), 
                  COL_NB_TXT);
}


// **************************************************************************** NumBox_RecalcTextForValue

// Calculate the position of the numeric text in the box
static void NumBox_RecalcTextForValue(Gadget* numbox, bool recalcFontSize){
    if (NBDATA->txtFontSize == 0.0f || recalcFontSize){
        char* widestString = Memory_Allocate(NULL, NB_DIGITS_MAX_N + 1, ZEROVAL_LAST);
        Memory_Set(widestString, NB_DIGITS_MAX_N, NB_WIDEST_CHAR);
        NBDATA->txtFontSize = Font_FitTextInSize(widestString, RSIZE(NBDATA->boxRect));
        widestString = Memory_Free(widestString);
    }

    NBDATA->txt = String_FromInt(NBDATA->txt, NBDATA->value);
    Point center = Geo_RectPoint(NBDATA->boxRect, RP_CENTER);
    NBDATA->txtPos = Font_CalcTextPos(NBDATA->txt, NBDATA->txtFontSize, center, RP_CENTER);
}


#ifdef DEBUG_MODE
// **************************************************************************** NumBox_Print

    // Multiline print of the parameters of the data object of the number box
    static void NumBox_PrintData(const Gadget* numbox){
        CHECK_NULL(numbox, WITH_NEW_LINE)
        CHECK_NULL(numbox->data, WITH_NEW_LINE)

        printf("Value:     %d\n", NBDATA->value);
        printf("Range:     %d - %d\n", NBDATA->minValue, NBDATA->maxValue);
        printf("Text:      \"%s\"\n", NBDATA->txt);
        printf("Box Rect:  "); Geo_PrintRect(NBDATA->boxRect, WITH_NEW_LINE);
        printf("Thickness: %.3f\n", NBDATA->thickness);
        printf("Font Size: %.3f\n", NBDATA->txtFontSize);
        printf("Pos:       "); Geo_PrintPoint(NBDATA->txtPos, WITH_NEW_LINE);
        printf("Alignment: %s\n", RectPointType_ToString(NBDATA->alignment));
    }
#endif




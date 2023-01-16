// ============================================================================
// RODS
// Timer Display
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions for calculating and drawing the Timer Display.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"
#include "../Fund/Fund.h"
#include "Graph.h"


// ============================================================================ PRIVATE MACROS

// **************************************************************************** TD

// The form (short or long), of the timer display
#define TD(gForm) \
    (td->forms[gForm])


// ============================================================================ PRIVATE CONSTANTS

#define TD_WIDEST_CHAR                      '9'
#define TD_WIDEST_CHAR_STR                  ((char[]) {TD_WIDEST_CHAR, '\0'})

#define TD_WIDEST_STR_SHORT                 ((char[]) {TD_WIDEST_CHAR, TD_WIDEST_CHAR, TIME_SEP_CHAR, \
                                                       TD_WIDEST_CHAR, TD_WIDEST_CHAR, '\0'})

#define TD_WIDEST_STR_LONG                  ((char[]) {TD_WIDEST_CHAR, TD_WIDEST_CHAR, TIME_SEP_CHAR, \
                                                       TD_WIDEST_CHAR, TD_WIDEST_CHAR, TIME_SEP_CHAR, \
                                                       TD_WIDEST_CHAR, TD_WIDEST_CHAR, '\0'})


// ============================================================================ OPAQUE STRUCTURES

// **************************************************************************** TimDisp

// Precalculated values for drawing the time in the form [HH:]MM:SS
struct TimDisp{
    Size cSize;
    E_RectPointType alignment;

    struct{
        float fontSize;
        Point origin;
        float charW;
        float sepW;
        float charShift[10];
        float invalidShift;
    }forms[2];
};






// ============================================================================ FUNC DEF

// **************************************************************************** TimDisp_Make

// Make a timer display and initialize it for the given containing size
TimDisp* TimDisp_Make(Size cSize, E_RectPointType alignment){
    TimDisp* td = Memory_Allocate(NULL, sizeof(TimDisp), ZEROVAL_ALL);

    alignment = PUT_IN_RANGE(alignment, 0, RECT_POINT_TYPES_N - 1);
    td->alignment = alignment;

    TimDisp_Resize(td, cSize);

    return td;
}


// **************************************************************************** TimDisp_Free

// Free the memory of the timer display
TimDisp* TimDisp_Free(TimDisp* td){
    return Memory_Free(td);
}


// **************************************************************************** TimDisp_Resize

// Resize the timer display, so that it fits inside the containing size
void TimDisp_Resize(TimDisp* td, Size cSize){
    td->cSize = cSize;

    for (int form = 0; form < 2; form++){
        const char* widestStr = (form == SHORT_FORM) ? TD_WIDEST_STR_SHORT : 
                                                       TD_WIDEST_STR_LONG;
        TD(form).fontSize = Font_FitTextInSize(widestStr, cSize);

        TD(form).origin = Font_CalcTextPos(widestStr, TD(form).fontSize, 
                                           Geo_RectPoint(TO_RECT(cSize), td->alignment), 
                                           td->alignment);
        
        TD(form).charW = Font_CalcTextSize(TD_WIDEST_CHAR_STR, TD(form).fontSize).width;
        TD(form).charW += FONT_SPACING;

        TD(form).sepW = Font_CalcTextSize(TIME_SEP_CHAR_STR, TD(form).fontSize).width;
        TD(form).sepW += FONT_SPACING;

        char tempStr[2] = {0};
        for (int i = 0; i < 10; i++){
            tempStr[0] = i + '0';
            float w = Font_CalcTextSize(tempStr, TD(form).fontSize).width;
            TD(form).charShift[i] = (TD(form).charW - w) * 0.5f;
        }

        float w = Font_CalcTextSize(TIME_INVALID_CHAR_STR, TD(form).fontSize).width;
        TD(form).invalidShift = (TD(form).charW - w) * 0.5f;
    }
}


// **************************************************************************** TimDisp_SetAlignment

// Change the alignment of the timer display inside its containing size
void TimDisp_SetAlignment(TimDisp* td, E_RectPointType alignment){
    alignment = PUT_IN_RANGE(alignment, 0, RECT_POINT_TYPES_N - 1);
    td->alignment = alignment;
    TimDisp_Resize(td, td->cSize);
}


// **************************************************************************** TimDisp_DrawTime

// Drawv the time in the form [HH:]MM:SS
void TimDisp_DrawTime(Time t, Point pos, TimDisp* td, Color color){
    bool isValid = Time_IsValid(t);

    bool form = (isValid && t.hours > 0) ? LONG_FORM : SHORT_FORM;

    char tempStr[2] = {0};
    Point cursor = Geo_TranslatePoint(pos, TD(form).origin);
    for (int unit = HOURS; unit <= SECONDS; unit++){
        if (unit == HOURS && form == SHORT_FORM){
            continue;
        }

        int digit = t.values[unit] / 10;
        tempStr[0] = isValid ? digit + '0' : TIME_INVALID_CHAR;
        float shift = isValid ? TD(form).charShift[digit] : TD(form).invalidShift;
        Font_DrawText(tempStr, TD(form).fontSize, Geo_MovePoint(cursor, shift, 0.0f), color);
        cursor.x += TD(form).charW;

        digit = t.values[unit] % 10;
        tempStr[0] = isValid ? digit + '0' : TIME_INVALID_CHAR;
        shift = isValid ? TD(form).charShift[digit] : TD(form).invalidShift;
        Font_DrawText(tempStr, TD(form).fontSize, Geo_MovePoint(cursor, shift, 0.0f), color);
        cursor.x += TD(form).charW;

        if (unit != SECONDS){
            Font_DrawText(TIME_SEP_CHAR_STR, TD(form).fontSize, cursor, color);
            cursor.x += TD(form).sepW;
        }
    }
}


#ifdef DEBUG_MODE
// **************************************************************************** TimDisp_Print

    // Multiline print of the parameters of the timer display
    void TimDisp_Print(TimDisp* td){
        CHECK_NULL(td, WITH_NEW_LINE)

        printf("CSize: "); Geo_PrintSize(td->cSize, WITH_NEW_LINE);
        printf("Alignment: %s\n", RectPointType_ToString(td->alignment));

        for (int form = 0; form < 2; form++){
            printf("%s FORM:\n", (form == SHORT_FORM) ? "SHORT" : "LONG");
            printf("   Origin:        "); Geo_PrintPoint(TD(form).origin, WITH_NEW_LINE);
            printf("   Font Size:     %.3f\n", TD(form).fontSize);
            printf("   Char Width:    %.3f\n", TD(form).charW);
            printf("   Sep Width:     %.3f\n", TD(form).sepW);
            printf("   Digit Shift:   ");
            for (int i = 0; i < 10; i++){
                printf("%d: %.3f", i, TD(form).charShift[i]);
                if (i < 9){
                    printf(",   ");
                }
            }
            printf("\n");
            printf("   Invalid Shift: %.3f\n", TD(form).invalidShift);
        }
    }
#endif





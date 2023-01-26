// ============================================================================
// RODS
// Timer
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions and definitions of constants and structures for the Timer 
    gadget.

    The timer has an expanded form, where it presents the current and a record 
    time, and a collapsed, single line form, showing the current time.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "time.h"

#include "../Public/Public.h"
#include "../Fund/Fund.h"
#include "../Graph/Graph.h"
#include "../GUI/GUI.h"
#include "Gadgets.h"


// ============================================================================ PRIVATE MACROS

// **************************************************************************** TIMDATA

// Pointer to the timer's data object
#define TIMDATA \
    ((TimerData*) timer->data)


// ============================================================================ PRIVATE CONSTANTS

#define TIM_CURRENT_DISPLAY_RATIO           MATH_PHI_INVERSE
#define TIM_INTERNAL_MARGIN                 0.1f

#define TIM_ICON_ID                         ICON_MEDAL

#define COL_TIM_DEF_CURRENT                 COL_UI_FG_SECONDARY
#define COL_TIM_DEF_RECORD                  COL_UI_FG_SECONDARY
#define COL_TIM_DEF_ICON                    COL_UI_FG_SECONDARY


// ============================================================================ PRIVATE STRUCTURES

// **************************************************************************** TimerData

// The data object of the Timer gadget
typedef struct TimerData{
    Time current;
    Time record;

    double t;
    double dt;

    TimDisp* currentTimDisp[2]; // Long or short form
    TimDisp* recordTimDisp;
    Point currentOrigin[2]; // Long or short form
    Point recordOrigin;

    Point iconOrigin;
    float iconScaleF;

    Color colorCurrent;
    Color colorRecord;
    Color colorIcon;
}TimerData;


// ============================================================================ PRIVATE FUNC DECL

static void     Timer_PrepareToFree(Gadget* timer);
static void     Timer_Resize(Gadget* timer);
static void     Timer_Update(Gadget* timer, EventQueue* queue);
static void     Timer_Draw(const Gadget* timer, Vector2 shift);
#ifdef DEBUG_MODE
    static void Timer_PrintData(const Gadget* timer);
#endif






// ============================================================================ FUNC DEF

// **************************************************************************** Timer_Make

// Make a timer. The gadget needs to be resized before its first use
Gadget* Timer_Make(E_GadgetID id, Time currentTime, Time recordTime){
    currentTime = Time_IsValid(currentTime) ? currentTime : TIME_INVALID;
    recordTime = Time_IsValid(recordTime) ? recordTime : TIME_INVALID;

    Gadget* timer = Gadget_Make(id, GT_TIMER, IS_NOT_SELECTABLE, 0);

    timer->PrepareToFree = Timer_PrepareToFree;
    timer->Resize        = Timer_Resize;
    timer->Update        = Timer_Update;
    timer->Draw          = Timer_Draw;
    #ifdef DEBUG_MODE
        timer->PrintData = Timer_PrintData;
    #endif

    TimerData* data = Memory_Allocate(NULL, sizeof(TimerData), ZEROVAL_ALL);

    data->current = currentTime;
    data->record = recordTime;
    data->t = GetTime();
    data->dt = 0.0;

    data->currentTimDisp[LONG_FORM]  = TimDisp_Make(RSIZE(GDG_DEF_RECT), RP_CENTER);
    data->currentTimDisp[SHORT_FORM] = TimDisp_Make(RSIZE(GDG_DEF_RECT), RP_CENTER);
    data->recordTimDisp              = TimDisp_Make(RSIZE(GDG_DEF_RECT), RP_CENTER);

    timer->data = data;

    Timer_SetColors(timer, COL_TIM_DEF_CURRENT, COL_TIM_DEF_RECORD, COL_TIM_DEF_ICON);

    return timer;
}


// **************************************************************************** Timer_Pause

// Pause the timer
void Timer_Pause(Gadget* timer){
    timer->Update = NULL;
}


// **************************************************************************** Timer_Resume

// Resume the timer
void Timer_Resume(Gadget* timer){
    TIMDATA->t = GetTime();
    timer->Update = Timer_Update;
}


// **************************************************************************** Timer_Toggle

// If the timer is paused resume, otherwise pause
bool Timer_Toggle(Gadget* timer){
    if (Timer_IsPaused(timer)){
        Timer_Resume(timer);
    }else{
        Timer_Pause(timer);
    }

    return !Timer_IsPaused(timer);
}


// **************************************************************************** Timer_SetTime

// Set the current time of the timer
void Timer_SetTime(Gadget* timer, Time time){
    TIMDATA->current = Time_IsValid(time) ? time : TIME_INVALID;
    TIMDATA->dt = 0.0;
    TIMDATA->t = GetTime();
}


// **************************************************************************** Timer_SetRecordTime

// Set the record time of the timer
void Timer_SetRecordTime(Gadget* timer, Time recordTime){
    TIMDATA->record = Time_IsValid(recordTime) ? recordTime : TIME_INVALID;
}


// **************************************************************************** Timer_Expand

// Set the timer in its expanded form
void Timer_Expand(Gadget* timer){
    timer->isExpanded = true;
}


// **************************************************************************** Timer_Collapse

// Set the timer in its collapsed form
void Timer_Collapse(Gadget* timer){
    timer->isExpanded = false;
}


// **************************************************************************** Timer_SetColors

// Set the color scheme of the timer
void Timer_SetColors(Gadget* timer, Color currentColor, Color recordColor, Color iconColor){
    TIMDATA->colorCurrent = currentColor;
    TIMDATA->colorRecord  = recordColor;
    TIMDATA->colorIcon    = iconColor;
}


// **************************************************************************** Timer_IsPaused

// Return true if the timer is paused
bool Timer_IsPaused(const Gadget* timer){
    return timer->Update == NULL;
}


// **************************************************************************** Timer_GetTime

// Get the current time of the timer
Time Timer_GetTime(const Gadget* timer){
    return TIMDATA->current;
}


// **************************************************************************** Timer_GetRecordTime

// Get the record time of the timer
Time Timer_GetRecordTime(const Gadget* timer){
    return TIMDATA->record;
}






// ============================================================================ PRIVATE FUNC DEF

// **************************************************************************** Timer_PrepareToFree

// Free the memory of the timer displays
static void Timer_PrepareToFree(Gadget* timer){
    TIMDATA->currentTimDisp[LONG_FORM]  = TimDisp_Free(TIMDATA->currentTimDisp[LONG_FORM]);
    TIMDATA->currentTimDisp[SHORT_FORM] = TimDisp_Free(TIMDATA->currentTimDisp[SHORT_FORM]);
    TIMDATA->recordTimDisp              = TimDisp_Free(TIMDATA->recordTimDisp);
}


// **************************************************************************** Timer_Resize

// Resize the timer and precalculate the values both for the expanded and 
// collapsed forms
static void Timer_Resize(Gadget* timer){
    // Current Time - Collapsed form
    Rect collapsedRect_Outer = timer->cRect;
    
    float collpasedMargin = MIN_DIM(collapsedRect_Outer) * TIM_INTERNAL_MARGIN;
    Rect collapsedRect_Inner = Geo_ApplyRectMargins(collapsedRect_Outer, collpasedMargin);
    Size collapsedSize_Inner = RSIZE(collapsedRect_Inner);

    TimDisp_Resize(TIMDATA->currentTimDisp[SHORT_FORM], collapsedSize_Inner);
    TIMDATA->currentOrigin[SHORT_FORM] = RORIGIN(collapsedRect_Inner);

    Point topLeft     = RORIGIN(timer->cRect);
    Point bottomLeft  = Geo_RectPoint(timer->cRect, RP_BOTTOM_LEFT);
    Point bottomRight = Geo_RectPoint(timer->cRect, RP_BOTTOM_RIGHT);

    // Current Time - Expanded form
    Size currentSize_Outer = SIZE(timer->cRect.width, timer->cRect.height * TIM_CURRENT_DISPLAY_RATIO);
    Rect currentRect_Outer = Geo_SetRectPS(topLeft, currentSize_Outer, RP_TOP_LEFT);
    
    float currentMargin = MIN_DIM(currentRect_Outer) * TIM_INTERNAL_MARGIN;
    Rect currentRect_Inner = Geo_ApplyRectMargins(currentRect_Outer, currentMargin);
    Size currentSize_Inner = RSIZE(currentRect_Inner);

    TimDisp_Resize(TIMDATA->currentTimDisp[LONG_FORM], currentSize_Inner);
    TIMDATA->currentOrigin[LONG_FORM] = RORIGIN(currentRect_Inner);

    // Icon
    Size iconSize_Outer = SIZE_SQR(timer->cRect.height - currentSize_Outer.height);
    Rect iconRect_Outer = Geo_SetRectPS(bottomLeft, iconSize_Outer, RP_BOTTOM_LEFT);

    float iconMargin = MIN_DIM(iconRect_Outer) * TIM_INTERNAL_MARGIN;
    Rect iconRect_Inner = Geo_ApplyRectMargins(iconRect_Outer, iconMargin);
    Size iconSize_Inner = RSIZE(iconRect_Inner);

    TIMDATA->iconOrigin = RORIGIN(iconRect_Inner);
    TIMDATA->iconScaleF = iconSize_Inner.width / ICON_DEF_TEXTURE_SIZE;

    // Record Time
    Size recordSize_Outer = SIZE(timer->cRect.width - iconRect_Outer.width, iconRect_Outer.height);
    Rect recordRect_Outer = Geo_SetRectPS(bottomRight, recordSize_Outer, RP_BOTTOM_RIGHT);

    float recordMargin = MIN_DIM(recordRect_Outer) * TIM_INTERNAL_MARGIN;
    Rect recordRect_Inner = Geo_ApplyRectMargins(recordRect_Outer, recordMargin);
    Size recordSize_Inner = RSIZE(recordRect_Inner);

    TimDisp_Resize(TIMDATA->recordTimDisp, recordSize_Inner);
    TIMDATA->recordOrigin = RORIGIN(recordRect_Inner);
}


// **************************************************************************** Timer_Update

// Increment the current time once per second
static void Timer_Update(Gadget* timer, UNUSED EventQueue* queue){
    double t = GetTime();
    TIMDATA->dt = t - TIMDATA->t;

    if (TIMDATA->dt >= 1.0){
        TIMDATA->current = Time_Increment(TIMDATA->current);
        TIMDATA->dt -= (double) ((int) TIMDATA->dt);
        TIMDATA->t = t;
    }
}


// **************************************************************************** Timer_Draw

// Draw the timer in collpased or expanded form
static void Timer_Draw(const Gadget* timer, Vector2 shift){
    if (timer->isExpanded){
        DrawTextureEx(Glo_Textures.icons[TIM_ICON_ID], Geo_TranslatePoint(TIMDATA->iconOrigin, shift), 
                      0.0f, TIMDATA->iconScaleF, TIMDATA->colorIcon);
        TimDisp_DrawTime(TIMDATA->current, Geo_TranslatePoint(TIMDATA->currentOrigin[LONG_FORM], shift), 
                         TIMDATA->currentTimDisp[LONG_FORM], TIMDATA->colorCurrent);
        TimDisp_DrawTime(TIMDATA->record, Geo_TranslatePoint(TIMDATA->recordOrigin, shift), 
                         TIMDATA->recordTimDisp, TIMDATA->colorRecord);
    }else{
        TimDisp_DrawTime(TIMDATA->current, Geo_TranslatePoint(TIMDATA->currentOrigin[SHORT_FORM], shift), 
                         TIMDATA->currentTimDisp[SHORT_FORM], TIMDATA->colorCurrent);
    }
}


#ifdef DEBUG_MODE
// **************************************************************************** Timer_PrintData

    // Multiline print of the parameters of the timer's data object
    static void Timer_PrintData(const Gadget* timer){
        CHECK_NULL(timer, WITH_NEW_LINE)
        CHECK_NULL(timer->data, WITH_NEW_LINE)

        printf("Current Time: "); Time_Print(TIMDATA->current, WITH_NEW_LINE);
        printf("Record Time:  "); Time_Print(TIMDATA->record, WITH_NEW_LINE);
        printf("t:            %.5f\n", TIMDATA->t);
        printf("dt:           %.5f\n", TIMDATA->dt);
        printf("\n");

        printf("CURRENT TIME - COLLAPSED FORM:\n");
        TimDisp_Print(TIMDATA->currentTimDisp[SHORT_FORM]);
        printf("\n");

        printf("CURRENT TIME - EXPANDED FORM:\n");
        TimDisp_Print(TIMDATA->currentTimDisp[LONG_FORM]);
        printf("\n");

        printf("RECORD TIME:\n");
        TimDisp_Print(TIMDATA->recordTimDisp);
        printf("\n");

        printf("Current Time Origin (Collapsed): "); Geo_PrintPoint(TIMDATA->currentOrigin[SHORT_FORM], WITH_NEW_LINE);
        printf("Current Time Origin (Expanded):  "); Geo_PrintPoint(TIMDATA->currentOrigin[LONG_FORM], WITH_NEW_LINE);
        printf("Record Time Origin:              "); Geo_PrintPoint(TIMDATA->recordOrigin, WITH_NEW_LINE);
        printf("\n");

        printf("Icon Origin:       "); Geo_PrintPoint(TIMDATA->iconOrigin, WITH_NEW_LINE);
        printf("Icon Scale Factor: %.3f\n", TIMDATA->iconScaleF);
        printf("\n");

        printf("Current Color: "); Color_Print(TIMDATA->colorCurrent, WITH_NEW_LINE);
        printf("Record Color:  "); Color_Print(TIMDATA->colorRecord, WITH_NEW_LINE);
        printf("Icon Color:    "); Color_Print(TIMDATA->colorIcon, WITH_NEW_LINE);
    }
#endif




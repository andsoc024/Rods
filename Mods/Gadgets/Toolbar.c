// ============================================================================
// RODS
// Toolbar
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions and definitions of constants and structures for the Toolbar 
    gadget.

    The toolbar contains the timer, a label which shows how many unelectrified 
    rods are left, buttons and the sound switch. It sits always at the top, 
    collapsed and is expanded only when the mouse cursor hovers over it.
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

// **************************************************************************** TBDATA

// Pointer to the data object of the toolbar
#define TBDATA \
    ((ToolbarData*) toolbar->data)


// **************************************************************************** TBGDG

// Pointer to the toolbar gadget
#define TBGDG(gIndex) \
    (toolbar->subGadgets[gIndex])


// ============================================================================ PRIVATE CONSTANTS

#define TB_SUBGADGETS_N                     10
#define TB_WIN_MARGIN                       10.0f

#define TB_MAX_BTN_SIZE                     45.0f

#define TB_COLLAPSED_HEIGHT                 25.0f

#define TB_TIMER_RATIO                      2.5f
#define TB_TIMER_HEIGHT_RATIO               2.0f
#define TB_RODS_LEFT_RATIO                  2.0f
#define TB_ICON_RATIO                       0.75f
#define TB_SWITCH_RATIO                     1.5f
#define TB_SMALL_SEP_RATIO                  0.25f
#define TB_BIG_SEP_RATIO                    1.0f
#define TB_HOR_MARGIN_RATIO                 0.5f
#define TB_VER_MARGIN_RATIO                 0.5f
#define TB_VER_MARGIN_COLLAPSED             4.0f

#define TB_BUTTONS_N                        6
#define TB_SMALL_SEPS_N                     5
#define TB_BIG_SEPS_N                       4

#define TB_TOTAL_WIDTH_RATIO                (2.0f                    * TB_HOR_MARGIN_RATIO + \
                                             1.0f                    * TB_TIMER_RATIO      + \
                                             (float) TB_SMALL_SEPS_N * TB_SMALL_SEP_RATIO  + \
                                             1.0f                    * TB_RODS_LEFT_RATIO  + \
                                             (float) TB_BIG_SEPS_N   * TB_BIG_SEP_RATIO    + \
                                             6.0f                    * 1.0f                + \
                                             1.0f                    * TB_ICON_RATIO       + \
                                             1.0f                    * TB_SWITCH_RATIO)

#define TB_TOTAL_HEIGHT_RATIO               (TB_TIMER_HEIGHT_RATIO + 2.0f * TB_VER_MARGIN_RATIO)

#define TB_ANIM_FRAMES_N                    ((FPS * 1) / 3)
#define TB_ROUNDNESS                        0.1f

#define COL_TB_BG                           COL_UI_BG_TERTIARY


// ============================================================================ PRIVATE STRUCTURES

// **************************************************************************** ToolbarData

// The data object of the toolbar gadget
typedef struct ToolbarData{
    Rect activeRect;
    Rect timerRects[2]; // Short or long form (expanded/collapsed)
    Rect rodsLeftRect[2];
    float shiftY;
    float shiftIncr;
}ToolbarData;


// ============================================================================ PRIVATE FUNC DECL

static void     Toolbar_PrepareToFree(Gadget* toolbar);
static void     Toolbar_Resize(Gadget* toolbar);
static void     Toolbar_ResizeAfterSubgadgets(Gadget* toolbar);
static void     Toolbar_ReactToEvent(Gadget* toolbar, Event event, EventQueue* queue);
static void     Toolbar_Update(Gadget* toolbar, EventQueue* queue);
static Vector2  Toolbar_ChangeShift(const Gadget* toolbar, Vector2 shift);
static void     Toolbar_Draw(const Gadget* toolbar, Vector2 shift);
#ifdef DEBUG_MODE
    static void Toolbar_PrintData(const Gadget* toolbar);
#endif






// ============================================================================ FUNC DEF

// **************************************************************************** Toolbar_Make

// Make the toolbar with all its subgadgets. It is already resized. It starts 
// as collapsed
Gadget* Toolbar_Make(void){
    Gadget* toolbar = Gadget_Make(GDG_TOOLBAR, GT_TOOLBAR, IS_SELECTABLE, 10);

    toolbar->subGadgets[0] = Timer_Make(GDG_TIMER, TIME_NULL, TIME_INVALID);
    toolbar->subGadgets[1] = Label_Make(GDG_LBL_RODS_LEFT, "0", COL_UI_FG_PRIMARY, RP_CENTER);
    
    toolbar->subGadgets[2] = Button_MakeAsIcon(GDG_BTN_BACK, ICON_BACK);
    toolbar->subGadgets[3] = Button_MakeAsIcon(GDG_BTN_RESTART, ICON_RESTART);

    toolbar->subGadgets[4] = Button_MakeAsIcon(GDG_BTN_ZOOM_IN, ICON_ZOOM_IN);
    toolbar->subGadgets[5] = Button_MakeAsIcon(GDG_BTN_ZOOM_OUT, ICON_ZOOM_OUT);

    toolbar->subGadgets[6] = Button_MakeAsIcon(GDG_BTN_HELP, ICON_HELP);
    toolbar->subGadgets[7] = Button_MakeAsIcon(GDG_BTN_INFO, ICON_INFO);

    toolbar->subGadgets[8] = IconLabel_Make(GDG_ICON_MUSIC, ICON_MUSIC, COL_UI_FG_SECONDARY, RP_CENTER);
    toolbar->subGadgets[9] = Switch_Make(GDG_SWITCH_SOUND, true);


    toolbar->PrepareToFree         = Toolbar_PrepareToFree;
    toolbar->Resize                = Toolbar_Resize;
    toolbar->ResizeAfterSubgadgets = Toolbar_ResizeAfterSubgadgets;
    toolbar->ReactToEvent          = Toolbar_ReactToEvent;
    toolbar->Update                = Toolbar_Update;
    toolbar->ChangeShift           = Toolbar_ChangeShift;
    toolbar->Draw                  = Toolbar_Draw;
    #ifdef DEBUG_MODE
        toolbar->PrintData         = Toolbar_PrintData;
    #endif

    ToolbarData* data = Memory_Allocate(NULL, sizeof(ToolbarData), ZEROVAL_ALL);
    toolbar->data = data;

    Gadget_Resize(toolbar);

    return toolbar;
}


// **************************************************************************** Toolbar_Expand

// Expand the toolbar, optionally with animation
void Toolbar_Expand(Gadget* toolbar, bool withAnim){
    toolbar->isExpanded = true;
    toolbar->nSubGadgets = TB_SUBGADGETS_N;

    TBGDG(TB_TIMER)->cRect = TBDATA->timerRects[LONG_FORM];
    Gadget_Resize(TBGDG(TB_TIMER));
    Timer_Expand(TBGDG(TB_TIMER));

    TBGDG(TB_LBL_RODS_LEFT)->cRect = TBDATA->rodsLeftRect[LONG_FORM];
    Gadget_Resize(TBGDG(TB_LBL_RODS_LEFT));

    if (withAnim){
        TBDATA->shiftIncr = (toolbar->cRect.height - TB_COLLAPSED_HEIGHT) / (float) TB_ANIM_FRAMES_N;
    }else{
        TBDATA->shiftIncr = 0.0f;
        TBDATA->shiftY = 0.0f;
    }
}


// **************************************************************************** Toolbar_Collapse

// Collapse the toolbar, optionally with animation
void Toolbar_Collapse(Gadget* toolbar, bool withAnim){
    if (withAnim){
        TBDATA->shiftIncr = - (toolbar->cRect.height - TB_COLLAPSED_HEIGHT) / (float) TB_ANIM_FRAMES_N;
        return;
    }

    toolbar->isExpanded = false;
    toolbar->nSubGadgets = 2;

    TBGDG(TB_TIMER)->cRect = TBDATA->timerRects[SHORT_FORM];
    Gadget_Resize(TBGDG(TB_TIMER));
    Timer_Collapse(TBGDG(TB_TIMER));

    TBGDG(TB_LBL_RODS_LEFT)->cRect = TBDATA->rodsLeftRect[SHORT_FORM];
    Gadget_Resize(TBGDG(TB_LBL_RODS_LEFT));

    TBDATA->shiftIncr = 0.0f;
    TBDATA->shiftY = TB_COLLAPSED_HEIGHT - toolbar->cRect.height;
}


// **************************************************************************** Toolbar_FinishAnim

// Complete eventual expand or collapse animation
void Toolbar_FinishAnim(Gadget* toolbar){
    if (TBDATA->shiftIncr < 0.0f){
        Toolbar_Collapse(toolbar, WITHOUT_ANIM);
    }else if (TBDATA->shiftIncr > 0.0f){
        Toolbar_Expand(toolbar, WITHOUT_ANIM);
    }
}


// **************************************************************************** Toolbar_SetTime

// Set the current time of the timer
void Toolbar_SetTime(Gadget* toolbar, Time time){
    Timer_SetTime(TBGDG(TB_TIMER), time);
}


// **************************************************************************** Toolbar_SetRecordTime

// Set the record time of the toolbar timer
void Toolbar_SetRecordTime(Gadget* toolbar, Time time){
    Timer_SetRecordTime(TBGDG(TB_TIMER), time);
}


// **************************************************************************** Toolbar_SetRodsLeft

// Set the number shown by the rods left label
void Toolbar_SetRodsLeft(Gadget* toolbar, int rodsLeft){
    char* tempStr = String_FromInt(NULL, rodsLeft);
    Label_SetText(TBGDG(TB_LBL_RODS_LEFT), tempStr);
    tempStr = Memory_Free(tempStr);
}


// **************************************************************************** Toolbar_SetSoundSwitch

// Set the state of the sound switch, without animation
void Toolbar_SetSoundSwitch(Gadget* toolbar, bool switchState){
    Switch_SetValue(TBGDG(TB_SWITCH_SOUND), switchState, WITHOUT_ANIM);
}


// **************************************************************************** Toolbar_IsAnimating

// Return true if the toolbar is during expand or collapse animation
bool Toolbar_IsAnimating(const Gadget* toolbar){
    return TBDATA->shiftIncr != 0.0f;
}


// **************************************************************************** Toolbar_GetTime

// Return the current time of the toolbar timer
Time Toolbar_GetTime(const Gadget* toolbar){
    return Timer_GetTime(TBGDG(TB_TIMER));
}


// **************************************************************************** Toolbar_GetRecordTime

// Return the record time of the toolbar timer
Time Toolbar_GetRecordTime(const Gadget* toolbar){
    return Timer_GetRecordTime(TBGDG(TB_TIMER));
}


// **************************************************************************** Toolbar_GetSoundSwitchState

// Return the boolean state of the sound switch
bool Toolbar_GetSoundSwitchState(const Gadget* toolbar){
    return Switch_GetValue(TBGDG(TB_SWITCH_SOUND));
}


// **************************************************************************** Toolbar_GetGadget

// Return a pointer to the requested toolbar gadget
Gadget* Toolbar_GetGadget(const Gadget* toolbar, int index){
    if (!IS_IN_RANGE(index, 0, TB_SUBGADGETS_N - 1)){
        return NULL;
    }

    return toolbar->subGadgets[index];
}






// ============================================================================ PRIVATE FUNC DEF

// **************************************************************************** Toolbar_PrepareToFree

// Set the number of gadgets to max, so all are freed
static void Toolbar_PrepareToFree(Gadget* toolbar){
    toolbar->nSubGadgets = TB_SUBGADGETS_N;
}


// **************************************************************************** Toolbar_Resize

// Resize the toolbar
static void Toolbar_Resize(Gadget* toolbar){
    Toolbar_Expand(toolbar, WITHOUT_ANIM);

    float totalWidth = Glo_WinSize.width - 2.0f * TB_WIN_MARGIN;
    float btnSize = totalWidth / TB_TOTAL_WIDTH_RATIO;
    btnSize = MIN(btnSize, TB_MAX_BTN_SIZE);
    totalWidth = btnSize * TB_TOTAL_WIDTH_RATIO;

    float totalHeight = btnSize * TB_TOTAL_HEIGHT_RATIO;
    totalHeight = MAX(totalHeight, TB_COLLAPSED_HEIGHT);
    float middle = totalHeight * 0.5f;

    Size totalSize = SIZE(totalWidth, totalHeight);
    toolbar->cRect = Geo_AlignRect(TO_RECT(totalSize), TO_RECT(Glo_WinSize), RP_TOP_CENTER);

    Size collapsedSize = SIZE(totalWidth, TB_COLLAPSED_HEIGHT);
    TBDATA->activeRect = Geo_AlignRect(TO_RECT(collapsedSize), TO_RECT(Glo_WinSize), RP_TOP_CENTER);

    float collapsedGadgetHeight = TB_COLLAPSED_HEIGHT - 2.0f * TB_VER_MARGIN_COLLAPSED;

    float horMargin     = btnSize * TB_HOR_MARGIN_RATIO;
    float smallSep      = btnSize * TB_SMALL_SEP_RATIO;
    float bigSep        = btnSize * TB_BIG_SEP_RATIO;
    
    float timerWidth    = btnSize * TB_TIMER_RATIO;
    float timerHeight   = btnSize * TB_TIMER_HEIGHT_RATIO;
    float rodsLeftWidth = btnSize * TB_RODS_LEFT_RATIO;
    float iconSize      = btnSize * TB_ICON_RATIO;
    float switchWidth   = btnSize * TB_SWITCH_RATIO;
    float switchHeight  = totalHeight; 

    Point cursor = POINT(toolbar->cRect.x + horMargin, middle);

    TBDATA->timerRects[LONG_FORM]  = Geo_SetRectPS(cursor, SIZE(timerWidth, timerHeight),            RP_MIDDLE_LEFT);
    Point cCursor = POINT(cursor.x, toolbar->cRect.height - TB_COLLAPSED_HEIGHT * 0.5f);
    TBDATA->timerRects[SHORT_FORM] = Geo_SetRectPS(cCursor, SIZE(timerWidth, collapsedGadgetHeight), RP_MIDDLE_LEFT);
    TBGDG(TB_TIMER)->cRect = TBDATA->timerRects[LONG_FORM];
    cursor.x += (timerWidth + smallSep);

    TBDATA->rodsLeftRect[LONG_FORM]  = Geo_SetRectPS(cursor, SIZE(rodsLeftWidth, btnSize),               RP_MIDDLE_LEFT);
    cCursor = POINT(cursor.x, toolbar->cRect.height - TB_COLLAPSED_HEIGHT * 0.5f);
    TBDATA->rodsLeftRect[SHORT_FORM] = Geo_SetRectPS(cCursor, SIZE(rodsLeftWidth, collapsedGadgetHeight), RP_MIDDLE_LEFT);
    TBGDG(TB_LBL_RODS_LEFT)->cRect = TBDATA->rodsLeftRect[LONG_FORM];
    cursor.x += (rodsLeftWidth + bigSep);

    TBGDG(TB_BTN_BACK)->cRect    = Geo_SetRectPS(cursor, SIZE_SQR(btnSize), RP_MIDDLE_LEFT);
    cursor.x += (btnSize + smallSep);

    TBGDG(TB_BTN_RESTART)->cRect = Geo_SetRectPS(cursor, SIZE_SQR(btnSize), RP_MIDDLE_LEFT);
    cursor.x += (btnSize + bigSep);

    TBGDG(TB_BTN_ZOOM_IN)->cRect = Geo_SetRectPS(cursor, SIZE_SQR(btnSize), RP_MIDDLE_LEFT);
    cursor.x += (btnSize + smallSep);

    TBGDG(TB_BTN_ZOOM_OUT)->cRect = Geo_SetRectPS(cursor, SIZE_SQR(btnSize), RP_MIDDLE_LEFT);
    cursor.x += (btnSize + bigSep);

    TBGDG(TB_BTN_HELP)->cRect = Geo_SetRectPS(cursor, SIZE_SQR(btnSize), RP_MIDDLE_LEFT);
    cursor.x += (btnSize + smallSep);

    TBGDG(TB_BTN_INFO)->cRect = Geo_SetRectPS(cursor, SIZE_SQR(btnSize), RP_MIDDLE_LEFT);
    cursor.x += (btnSize + bigSep);

    TBGDG(TB_ICON_SOUND)->cRect = Geo_SetRectPS(cursor, SIZE_SQR(iconSize), RP_MIDDLE_LEFT);
    cursor.x += (iconSize + smallSep);

    TBGDG(TB_SWITCH_SOUND)->cRect = Geo_SetRectPS(cursor, SIZE(switchWidth, switchHeight), RP_MIDDLE_LEFT);

    TBDATA->shiftY    = 0.0f;
    TBDATA->shiftIncr = 0.0f;
}


// **************************************************************************** Toolbar_ResizeAfterSubgadgets

// Set the tollbar in collapsed state, after resizing
static void Toolbar_ResizeAfterSubgadgets(Gadget* toolbar){
    Toolbar_Collapse(toolbar, WITHOUT_ANIM);
}


// **************************************************************************** Toolbar_ReactToEvent

// Collapse or expand the toolbar, depending on the mouse movement
static void Toolbar_ReactToEvent(Gadget* toolbar, Event event, EventQueue* queue){
    switch (event.id){
        case EVENT_MOUSE_MOVE:{
            if (!(toolbar->isExpanded)){
                if (Geo_PointIsInRect(event.data.mouse.pos, TBDATA->activeRect)){
                    Toolbar_Expand(toolbar, WITH_ANIM);
                    Queue_AddEvent(queue, Event_SetAsGadgetExpanded(toolbar->id));
                }

            }else if (TBDATA->shiftIncr == 0.0f){
                if (!Geo_PointIsInRect(event.data.mouse.pos, toolbar->cRect)){
                    Toolbar_Collapse(toolbar, WITH_ANIM);
                    Queue_AddEvent(queue, Event_SetAsGadgetCollapsed(toolbar->id));
                }
            }

            break;
        }

        default: {break;}
    }
}


// **************************************************************************** Toolbar_Update

// Update eventual collapse or expand animation of the toolbar
static void Toolbar_Update(Gadget* toolbar, UNUSED EventQueue* queue){
    if (TBDATA->shiftIncr == 0.0f) {return;}

    TBDATA->shiftY += TBDATA->shiftIncr;

    if (TBDATA->shiftIncr > 0.0f && TBDATA->shiftY >= 0.0f){
        TBDATA->shiftY = 0.0f;
        TBDATA->shiftIncr = 0.0f;
    }

    if (TBDATA->shiftIncr < 0.0f && TBDATA->shiftY <= TB_COLLAPSED_HEIGHT - toolbar->cRect.height){
        Toolbar_Collapse(toolbar, WITHOUT_ANIM);
    }
}


// **************************************************************************** Toolbar_ChangeShift

// Add the shift to the Y axis, when the toolbar is collapsed
static Vector2 Toolbar_ChangeShift(const Gadget* toolbar, Vector2 shift){
    return Geo_MovePoint(shift, 0.0f, TBDATA->shiftY);
}


// **************************************************************************** Toolbar_Draw

// Draw the toolbar background as a rounded rectangle
static void Toolbar_Draw(const Gadget* toolbar, Vector2 shift){
    DrawRectangleRounded(Geo_TranslateRect(toolbar->cRect, shift), TB_ROUNDNESS, 10.0f, COL_TB_BG);
}


#ifdef DEBUG_MODE
// **************************************************************************** Toolbar_PrintData

    // Multiline print of the parameters of the data object of the toolbar
    static void Toolbar_PrintData(const Gadget* toolbar){
        CHECK_NULL(toolbar, WITH_NEW_LINE)
        CHECK_NULL(toolbar->data, WITH_NEW_LINE)

        printf("Active Rect:                "); Geo_PrintRect(TBDATA->activeRect, WITH_NEW_LINE);
        printf("Timer Rect (Expanded):      "); Geo_PrintRect(TBDATA->timerRects[LONG_FORM], WITH_NEW_LINE);
        printf("Timer Rect (Collapsed):     "); Geo_PrintRect(TBDATA->timerRects[SHORT_FORM], WITH_NEW_LINE);
        printf("Rods Left Rect (Expanded):  "); Geo_PrintRect(TBDATA->rodsLeftRect[LONG_FORM], WITH_NEW_LINE);
        printf("Rods Left Rect (Collapsed): "); Geo_PrintRect(TBDATA->rodsLeftRect[SHORT_FORM], WITH_NEW_LINE);
        printf("Y Shift:                    %.3f\n", TBDATA->shiftY);
        printf("Shift Increment:            %.3f\n", TBDATA->shiftIncr);
    }
#endif


// ============================================================================
// RODS
// Setup Page
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions and definitions of constants and structures for the Setup Page.

    The setup page lets setup a new rod grid of optional size. When it appears 
    after victory it displays the current and record time and informs if a new 
    record time is achieved.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"
#include "../Fund/Fund.h"
#include "../Logic/Logic.h"
#include "../Graph/Graph.h"
#include "../GUI/GUI.h"
#include "../Gadgets/Gadgets.h"
#include "Pages.h"


// ============================================================================ PRIVATE MACROS

// **************************************************************************** SPDATA

// Pointer to the data object of the setup page
#define SPDATA \
    ((SetupPageData*) page->data)


// ============================================================================ PRIVATE CONSTANTS

#define SETUP_WIN_MARGIN_RATIO              ((1 - MATH_PHI_INVERSE) * 0.5f)

#define SETUP_NEW_RECORD                    0
#define SETUP_VICTORY                       1
#define SETUP_NORMAL                        2

#define BG_WIDTH                            650.0f

#define NUMBOX_SIZE                         SIZE(210, 140)
#define NUMBOX_HOR_MARGIN                   75.0f
#define VER_MARGIN                          100.0f
#define NEW_RECORD_SIZE                     SIZE(500, 150)
#define TIME_TABLE_SIZE                     SIZE(500, 250)
#define OK_BTN_SIZE                         SIZE(300, 120)

#define NEW_RECORD_LABEL_TEXT               "New Record Time!!!"
#define OK_BUTTON_TEXT                      "OK"

#define TIME_TABLE_STRINGS                  ((char*[]) {"Time:", " ", "Record: ", " "})

const int SETUP_GADGETS_N[] =               {5, 4, 3};

// Gadget indices
#define SU_NUMBOX_COLS                      0
#define SU_NUMBOX_ROWS                      1
#define SU_BTN_OK                           2
#define SU_TABLE_TIME                       3
#define SU_NEW_RECORD                       4


// ============================================================================ PRIVATE STRUCTURES

// **************************************************************************** SetupPageData

// The data object of the setup page
typedef struct SetupPageData{
    int type;
    Rect bgRect;
    Color bgColor;
}SetupPageData;


// ============================================================================ PRIVATE FUNC DECL

void            SetupPage_PrepareToFree(Page* page);
void            SetupPage_Resize(Page* page);
void            SetupPage_ReactToEvent(Page* page, Event event, EventQueue* queue);
void            SetupPage_Draw(const Page* page);
#ifdef DEBUG_MODE
    void        SetupPage_PrintData(const Page* page);
#endif






// ============================================================================ FUNC DEF

// **************************************************************************** SetupPage_Make

// Make the setup page
Page* SetupPage_Make(void){
    Page* page = Page_Make(PAGE_SETUP);
    
    Gadget* numboxCols = NumBox_Make(GDG_NUMBOX_COLS, GDG_NUMBOX_COLS_BTN_UP, GDG_NUMBOX_COLS_BTN_DOWN);
    Page_AddGadget(page, numboxCols);

    Gadget* numboxRows = NumBox_Make(GDG_NUMBOX_ROWS, GDG_NUMBOX_ROWS_BTN_UP, GDG_NUMBOX_ROWS_BTN_DOWN);
    Page_AddGadget(page, numboxRows);

    Gadget* btnOK = Button_MakeAsText(GDG_SETUP_BTN_OK, OK_BUTTON_TEXT);
    Page_AddGadget(page, btnOK);

    Gadget* timeTable = Table_Make(GDG_SETUP_TABLE_TIME, 2, 2, (const char**) TIME_TABLE_STRINGS);
    Table_SetAlignment(timeTable, RP_MIDDLE_RIGHT, TABLE_COLUMN, 0);
    Table_SetAlignment(timeTable, RP_MIDDLE_LEFT,  TABLE_COLUMN, 1);
    Page_AddGadget(page, timeTable);

    Gadget* newRecordLabel = Label_MakeMagic(GDG_SETUP_LBL_NEW_RECORD, NEW_RECORD_LABEL_TEXT, RP_CENTER);
    Page_AddGadget(page, newRecordLabel);

    page->PrepareToFree = SetupPage_PrepareToFree;
    page->Resize        = SetupPage_Resize;
    page->ReactToEvent  = SetupPage_ReactToEvent;
    page->Draw          = SetupPage_Draw;
    #ifdef DEBUG_MODE
        page->PrintData = SetupPage_PrintData;
    #endif

    SetupPageData* data = Memory_Allocate(NULL, sizeof(SetupPageData), ZEROVAL_ALL);
    page->data = data;

    data->type = SETUP_NEW_RECORD;
    data->bgColor = COL_UI_BG_TERTIARY;
    
    Page_Resize(page);

    return page;
}


// **************************************************************************** SetupPage_PrepareToShow

// Prepare the setup page to show. If a new record time is achieved, store it 
// in the records
void SetupPage_PrepareToShow(Page* page, Event event){
    if (event.id != EVENT_SHOW_PAGE) {return;}

    int nCols        = event.data.page.data.nCols;
    int nRows        = event.data.page.data.nRows;
    Time currentTime = event.data.page.data.time;
    bool victory     = event.data.page.data.victory;

    NumBox_SetValue(page->gadgets[SU_NUMBOX_COLS], event.data.page.data.nCols);
    NumBox_SetValue(page->gadgets[SU_NUMBOX_ROWS], event.data.page.data.nRows);

    SPDATA->type = victory ? SETUP_VICTORY : SETUP_NORMAL;

    Time recordTime = Records_Get(Glo_Records, nCols, nRows);
    if (SPDATA->type == SETUP_VICTORY && Time_IsSmaller(currentTime, recordTime)){
        Records_Set(Glo_Records, currentTime, nCols, nRows);
        SPDATA->type = SETUP_NEW_RECORD;
    }

    if (SPDATA->type != SETUP_NORMAL){
        SetupPage_SetCurrentTime(page, currentTime);
        SetupPage_SetRecordTime(page, recordTime);
    }

    page->nGadgets = SETUP_GADGETS_N[SPDATA->type];

    Page_Resize(page);
}


// **************************************************************************** SetupPage_SetCurrentTime

// Set the current time in the time table
void SetupPage_SetCurrentTime(Page* page, Time current){
    char* str = Time_ToString(NULL, current);
    Table_SetText(page->gadgets[SU_TABLE_TIME], 1, 0, str);
    str = Memory_Free(str);
}


// **************************************************************************** SetupPage_SetRecordTime

// Set the record time in the time table
void SetupPage_SetRecordTime(Page* page, Time record){
    char* str = Time_ToString(NULL, record);
    Table_SetText(page->gadgets[SU_TABLE_TIME], 1, 1, str);
    str = Memory_Free(str);
}






// ============================================================================ PRIVATE FUNC DEF

// **************************************************************************** SetupPage_PrepareToFree

// Set the number of gadgets to the max, so that they are all freed
void SetupPage_PrepareToFree(Page* page){
    page->nGadgets = SETUP_GADGETS_N[SETUP_NEW_RECORD];
}


// **************************************************************************** SetupPage_Resize

// Resize the setup page, according to its type
void SetupPage_Resize(Page* page){
    float bgHeight = VER_MARGIN * MATH_PHI;

    Rect numboxColsRect = Geo_SetRectPS(POINT(BG_WIDTH * 0.5f, VER_MARGIN), NUMBOX_SIZE, RP_TOP_RIGHT);
    numboxColsRect = Geo_MoveRect(numboxColsRect, -NUMBOX_HOR_MARGIN * 0.5f, 0.0f);

    Rect numboxRowsRect = Geo_SetRectPS(POINT(BG_WIDTH * 0.5f, VER_MARGIN), NUMBOX_SIZE, RP_TOP_LEFT);
    numboxRowsRect = Geo_MoveRect(numboxRowsRect, NUMBOX_HOR_MARGIN * 0.5f, 0.0f);

    bgHeight += (NUMBOX_SIZE.height + VER_MARGIN);

    Rect newRecordRect = RECT_NULL;
    if (SPDATA->type == SETUP_NEW_RECORD){
        newRecordRect = Geo_SetRectPS(POINT(BG_WIDTH * 0.5f, bgHeight), NEW_RECORD_SIZE, RP_TOP_CENTER);

        bgHeight += (NEW_RECORD_SIZE.height + VER_MARGIN);
    }

    Rect timeTableRect = RECT_NULL;
    if (SPDATA->type == SETUP_VICTORY || SPDATA->type == SETUP_NEW_RECORD){
        timeTableRect = Geo_SetRectPS(POINT(BG_WIDTH * 0.5f, bgHeight), TIME_TABLE_SIZE, RP_TOP_CENTER);

        bgHeight += (TIME_TABLE_SIZE.height + VER_MARGIN);
    }

    Rect okBtnRect = Geo_SetRectPS(POINT(BG_WIDTH * 0.5f, bgHeight), OK_BTN_SIZE, RP_TOP_CENTER);

    bgHeight += (OK_BTN_SIZE.height + VER_MARGIN * MATH_PHI);

    Size bgSize = SIZE(BG_WIDTH, bgHeight);


    float winMargin = SETUP_WIN_MARGIN_RATIO * MIN_DIM(Glo_WinSize);
    VGraph* vg = VGraph_Make(bgSize, TO_RECT(Glo_WinSize), winMargin);

    page->gadgets[SU_NUMBOX_COLS]->cRect = VGraph_ProjectRect(numboxColsRect, vg);
    page->gadgets[SU_NUMBOX_ROWS]->cRect = VGraph_ProjectRect(numboxRowsRect, vg);
    page->gadgets[SU_NEW_RECORD]->cRect  = VGraph_ProjectRect(newRecordRect,  vg);
    page->gadgets[SU_TABLE_TIME]->cRect  = VGraph_ProjectRect(timeTableRect,  vg);
    page->gadgets[SU_BTN_OK]->cRect      = VGraph_ProjectRect(okBtnRect,      vg);

    SPDATA->bgRect = VGraph_GetRScreen(vg);

    vg = VGraph_Free(vg);
}


// **************************************************************************** SetupPage_ReactToEvent

// When the OK button is pressed, emit EVENT_NEW_GRID. When the mouse clicks 
// outside the background rectangle, hide the setup page
void SetupPage_ReactToEvent(Page* page, Event event, EventQueue* queue){
    switch (event.id){
        case EVENT_MOUSE_RELEASED:{
            if (!Geo_PointIsInRect(event.data.mouse.pos, SPDATA->bgRect)){
                Queue_AddEvent(queue, Event_SetAsHidePage(page->id, page->id, WITHOUT_ANIM));
            }
            break;
        }

        case EVENT_BUTTON_PRESSED:{
            if (event.source == GDG_SETUP_BTN_OK){
                int nCols = NumBox_GetValue(page->gadgets[SU_NUMBOX_COLS]);
                int nRows = NumBox_GetValue(page->gadgets[SU_NUMBOX_ROWS]);
                Queue_AddEvent(queue, Event_SetAsMakeNewGrid(page->id, GDG_BOARD, nCols, nRows));
                Queue_AddEvent(queue, Event_SetAsHidePage(page->id, page->id, WITH_ANIM));
            }
            break;
        }

        default: {break;}
    }
}


// **************************************************************************** SetupPage_Draw

// Draw the background of the setup page
void SetupPage_Draw(const Page* page){
    DrawRectangleRounded(Geo_TranslateRect(SPDATA->bgRect, page->shift), RRECT_DEF_ROUNDNESS, 10, SPDATA->bgColor);
}


#ifdef DEBUG_MODE
// **************************************************************************** SetupPage_PrintData

    // Multiline print of the parameters of the data object of the setup page
    void SetupPage_PrintData(const Page* page){
        CHECK_NULL(page, WITH_NEW_LINE)
        CHECK_NULL(page->data, WITH_NEW_LINE)

        const char* names[] = {"New Record", "Victory", "Normal"};

        printf("Type:     %s (%d)\n", names[SPDATA->type], SPDATA->type);
        printf("BG Rect:  "); Geo_PrintRect(SPDATA->bgRect, WITH_NEW_LINE);
        printf("BG Color: "); Color_Print(SPDATA->bgColor, WITH_NEW_LINE);
    }
#endif



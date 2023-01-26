// ============================================================================
// RODS
// Info Page
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions and definitions of constants and structures for the info page.

    The info page presents information about the version and the author of the 
    program.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"
#include "../Fund/Fund.h"
#include "../Graph/Graph.h"
#include "../GUI/GUI.h"
#include "../Gadgets/Gadgets.h"
#include "Pages.h"


// ============================================================================ PRIVATE MACROS

// **************************************************************************** IPDATA

// Pointer to the data object of the info page
#define IPDATA \
    ((InfoPageData*) page->data)


// ============================================================================ PRIVATE CONSTANTS

#define MIN_SIZE                            SIZE(200, 75)
#define MAX_SIZE                            SIZE(400, 150)
#define MIN_MARGIN                          50.0f

#define MARGIN_TO_EDGE                      20.0f
#define MARGIN_BETWEEN_LABELS               10.0f

#define ROW1_TEXT                           APP_TITLE " v" APP_VERSION
#define ROW2_TEXT                           "by " APP_AUTHOR

#define COL_TEXT                            COL_UI_FG_PRIMARY
#define COL_BG_RECT                         COL_UI_BG_TERTIARY


// ============================================================================ PRIVATE STRUCTURES

// **************************************************************************** InfoPageData

// The data object of the info page
typedef struct InfoPageData{
    Rect bgRect;
}InfoPageData;


// ============================================================================ PRIVATE FUNC DECL

static void     InfoPage_Resize(Page* page);
static void     InfoPage_ResizeAfterGadgets(Page* page);
static void     InfoPage_ReactToEvent(Page* page, Event event, EventQueue* queue);
static void     InfoPage_Draw(const Page* page);
#ifdef DEBUG_MODE
    static void InfoPage_PrintData(const Page* page);
#endif






// ============================================================================ FUNC DEF

// **************************************************************************** InfoPage_Make

// Make the Info page
Page* InfoPage_Make(void){
    Page* page = Page_Make(PAGE_INFO);

    Gadget* row1 = Label_Make(GDG_INFO_LBL_TITLE, ROW1_TEXT, COL_TEXT, RP_CENTER);
    Page_AddGadget(page, row1);

    Gadget* row2 = Label_Make(GDG_INFO_LBL_AUTHOR, ROW2_TEXT, COL_TEXT, RP_CENTER);
    Page_AddGadget(page, row2);

    page->Resize             = InfoPage_Resize;
    page->ResizeAfterGadgets = InfoPage_ResizeAfterGadgets;
    page->ReactToEvent       = InfoPage_ReactToEvent;
    page->Draw               = InfoPage_Draw;
    #ifdef DEBUG_MODE
        page->PrintData      = InfoPage_PrintData;
    #endif

    InfoPageData* data = Memory_Allocate(NULL, sizeof(InfoPageData), ZEROVAL_ALL);
    page->data = data;

    Page_Resize(page);

    return page;
}


// ============================================================================ PRIVATE FUNC DEF

// **************************************************************************** InfoPage_Resize

// Resize the info page
static void InfoPage_Resize(Page* page){
    Size cSize = Geo_FitSizeInSize(MIN_SIZE, Glo_WinSize, MIN_MARGIN);
    cSize = Geo_PutSizeInRange(cSize, &MIN_SIZE, &MAX_SIZE);

    Rect cRect = Geo_SetRectPS(Geo_RectPoint(TO_RECT(Glo_WinSize), RP_CENTER), cSize, RP_CENTER);
    IPDATA->bgRect = cRect;

    Size rowSize;
    rowSize.width = cRect.width - 2.0f * MARGIN_TO_EDGE;
    rowSize.height = (cRect.height - 2.0f * MARGIN_TO_EDGE - MARGIN_BETWEEN_LABELS) * 0.5f;

    Rect innerRect = Geo_ApplyRectMargins(cRect, MARGIN_TO_EDGE);

    Point topLeft = RORIGIN(innerRect);
    Point bottomLeft = Geo_RectPoint(innerRect, RP_BOTTOM_LEFT);

    page->gadgets[0]->cRect = Geo_SetRectPS(topLeft,    rowSize, RP_TOP_LEFT);
    page->gadgets[1]->cRect = Geo_SetRectPS(bottomLeft, rowSize, RP_BOTTOM_LEFT);
}


// **************************************************************************** InfoPage_ResizeAfterGadgets

// Equalize the font size of the labels
static void InfoPage_ResizeAfterGadgets(Page* page){
    float fontSize1 = Label_GetFontSize(page->gadgets[0]);
    float fontSize2 = Label_GetFontSize(page->gadgets[1]);

    float fontSize = MIN(fontSize1, fontSize2);

    Label_SetFontSize(page->gadgets[0], fontSize);
    Label_SetFontSize(page->gadgets[1], fontSize);
}


// **************************************************************************** InfoPage_ReactToEvent

// Hide the page when the mouse clicks ouside the background rectangle
static void InfoPage_ReactToEvent(Page* page, Event event, EventQueue* queue){
    switch (event.id){
        case EVENT_MOUSE_RELEASED:{
            if (!Geo_PointIsInRect(event.data.mouse.pos, IPDATA->bgRect)){
                Queue_AddEvent(queue, Event_SetAsHidePage(page->id, page->id, WITH_ANIM));
            }
            break;
        }

        default: {break;}
    }
}


// **************************************************************************** InfoPage_Draw

// Draw the background rectangle
static void InfoPage_Draw(const Page* page){
    DrawRectangleRounded(Geo_TranslateRect(IPDATA->bgRect, page->shift), RRECT_DEF_ROUNDNESS, 10, COL_BG_RECT);
}


#ifdef DEBUG_MODE
// **************************************************************************** InfoPage_PrintData

    // Multiline print of the parameters of the data object of the Info page
    static void InfoPage_PrintData(const Page* page){
        CHECK_NULL(page, WITH_NEW_LINE)
        CHECK_NULL(page->data, WITH_NEW_LINE)

        printf("Bg Rect: "); Geo_PrintRect(IPDATA->bgRect, WITH_NEW_LINE);
    }
#endif




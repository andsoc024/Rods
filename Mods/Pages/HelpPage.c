// ============================================================================
// RODS
// Help Page
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions as well as definitions of structures and constants for the Help 
    page. 

    The help page lists the keyboard controls of the program.
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

// **************************************************************************** HPDATA

// Pointer to the data object of the help page
#define HPDATA \
    ((HelpPageData*) page->data)


// ============================================================================ PRIVATE CONSTANTS

#define MIN_SIZE                            SIZE(350, 400)
#define MAX_SIZE                            SIZE(700, 800)

#define MIN_MARGIN                          50.0f

#define INNER_MARGIN                        20.0f

#define HELP_TEXT                           ((const char*[]) {                       \
                                             "Arrow Keys", "Pan the rod grid",       \
                                             "W, A, S, D", "Move the selection box", \
                                             "Space",       "Rotate",                \
                                             "Tab",         "Select next",           \
                                             "Enter",       "Press",                 \
                                             "+, -, Z, X,", "Zoom in/out",           \
                                             "T",           "Toggle the toolbar",    \
                                             "M",           "Sound on/off",          \
                                             "R",           "Default view"           \
                                            })

#define TABLE_COLS_N                        2
#define TABLE_ROWS_N                        9

// Gadget Indices

#define HP_TABLE                            0

#define COL_HELP_BG                         COL_UI_BG_TERTIARY
#define COL_KEYS                            COL_UI_FG_PRIMARY
#define COL_DESCRIPTION                     COL_UI_FG_SECONDARY


// ============================================================================ PRIVATE STRUCTURES

// **************************************************************************** HelpPageData

// The data object of the Help Page
typedef struct HelpPageData{
    Rect bgRect;
}HelpPageData;


// ============================================================================ PRIVATE FUNC DECL

static void     HelpPage_Resize(Page* page);
static void     HelpPage_ReactToEvent(Page* page, Event event, EventQueue* queue);
static void     HelpPage_Draw(const Page* page);
#ifdef DEBUG_MODE
    static void HelpPage_PrintData(const Page* page);
#endif


// ============================================================================ FUNC DEF

// **************************************************************************** HelpPage_Make

// Make the Help Page
Page* HelpPage_Make(void){
    Page* page = Page_Make(PAGE_HELP);

    const char** helpText = HELP_TEXT;
    Gadget* table = Table_Make(GDG_HELP_TABLE, TABLE_COLS_N, TABLE_ROWS_N, helpText);
    Page_AddGadget(page, table);

    Table_SetAlignment(table, RP_MIDDLE_RIGHT, TABLE_COLUMN, 0);
    Table_SetAlignment(table, RP_MIDDLE_LEFT, TABLE_COLUMN, 1);
    Table_SetColor(table, COL_KEYS, TABLE_COLUMN, 0);
    Table_SetColor(table, COL_DESCRIPTION, TABLE_COLUMN, 1);

    page->Resize        = HelpPage_Resize;
    page->ReactToEvent  = HelpPage_ReactToEvent;
    page->Draw          = HelpPage_Draw;
    #ifdef DEBUG_MODE
        page->PrintData = HelpPage_PrintData;
    #endif

    HelpPageData* data = Memory_Allocate(NULL, sizeof(HelpPageData), ZEROVAL_ALL);
    page->data = data;

    Page_Resize(page);

    return page;
}


// ============================================================================ PRIVATE FUNC DEF

// **************************************************************************** HelpPage_Resize

// Resize the help page table and background rectangle
static void HelpPage_Resize(Page* page){
    Size cSize = Geo_FitSizeInSize(MIN_SIZE, Glo_WinSize, MIN_MARGIN);
    cSize = Geo_PutSizeInRange(cSize, &MIN_SIZE, &MAX_SIZE);
    cSize = Geo_FitSizeInSize(MIN_SIZE, cSize, 0.0f);

    Rect cRect = Geo_AlignRect(TO_RECT(cSize), TO_RECT(Glo_WinSize), RP_CENTER);
    HPDATA->bgRect = cRect;

    page->gadgets[HP_TABLE]->cRect = Geo_ApplyRectMargins(cRect, INNER_MARGIN);
}


// **************************************************************************** HelpPage_ReactToEvent

// Hide the page when the mouse clicks outside the background rectangle
static void HelpPage_ReactToEvent(Page* page, Event event, EventQueue* queue){
    switch (event.id){
        case EVENT_MOUSE_RELEASED:{
            if (!Geo_PointIsInRect(event.data.mouse.pos, HPDATA->bgRect)){
                Queue_AddEvent(queue, Event_SetAsHidePage(page->id, page->id, WITH_ANIM));
            }
            break;
        }

        default: {break;}
    }
}


// **************************************************************************** HelpPage_Draw

// Draw the background rectangle of the Help Page
static void HelpPage_Draw(const Page* page){
    DrawRectangleRounded(Geo_TranslateRect(HPDATA->bgRect, page->shift), 
                         RRECT_DEF_ROUNDNESS, 
                         10, 
                         COL_HELP_BG);
}


#ifdef DEBUG_MODE
// **************************************************************************** HelpPage_PrintData

    // Multiline print of the parameters of the data object of the Help Page
    static void HelpPage_PrintData(const Page* page){
        CHECK_NULL(page, WITH_NEW_LINE)
        CHECK_NULL(page->data, WITH_NEW_LINE)

        printf("Bg Rect: "); Geo_PrintRect(HPDATA->bgRect, WITH_NEW_LINE);
    }
#endif




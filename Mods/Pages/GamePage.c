// ============================================================================
// RODS
// Game Page
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions as well as definitions of constants and structures for the Game 
    page.
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
#include "../Sound/Sound.h"
#include "Pages.h"


// ============================================================================ PRIVATE MACROS

// **************************************************************************** GPDATA

// Pointer to the data object of the game page
#define GPDATA \
    ((GamePageData*) page->data)


// ============================================================================ PRIVATE CONSTANTS

#define SBAR_HOR_RATIO                      0.75f
#define SBAR_VER_RATIO                      0.75f
#define SBAR_WIDTH                          30.0f

// Gadget Indices
#define GP_BOARD                            0
#define GP_SBAR_HOR                         1
#define GP_SBAR_VER                         2
#define GP_TOOLBAR                          3


// ============================================================================ PRIVATE STRUCTURES

// **************************************************************************** GamePageData

// The data object of the game page
typedef struct GamePageData{
    int nRodsLeft;
}GamePageData;


// ============================================================================ PRIVATE FUNC DECL

static void     GamePage_PrepareToShow(Page* page, Event event);
static void     GamePage_Resize(Page* page);
static void     GamePage_ReactToEvent(Page* page, Event event, EventQueue* queue);
static void     GamePage_Update(Page* page, EventQueue* queue);
#ifdef DEBUG_MODE
    static void GamePage_PrintData(const Page* page);
#endif






// ============================================================================ FUNC DEF

// **************************************************************************** GamePage_Make

// Make the game page. Load the grid from persistent data, if any, otherwise 
// make a new rod grid with the default grid size
Page* GamePage_Make(PData* pData){
    Page* page = Page_Make(PAGE_GAME);
    
    Gadget* board = Board_Make(GDG_BOARD, pData);
    Page_AddGadget(page, board);

    Gadget* sbarHor = Scrollbar_Make(GDG_SCROLLBAR_HOR, OR_HORISONTAL);
    Page_AddGadget(page, sbarHor);

    Gadget* sbarVer = Scrollbar_Make(GDG_SCROLLBAR_VER, OR_VERTICAL);
    Page_AddGadget(page, sbarVer);

    Gadget* toolbar = Toolbar_Make();
    Page_AddGadget(page, toolbar);

    page->PrepareToShow = GamePage_PrepareToShow;
    page->Resize        = GamePage_Resize;
    page->ReactToEvent  = GamePage_ReactToEvent;
    page->Update        = GamePage_Update;
    #ifdef DEBUG_MODE
        page->PrintData = GamePage_PrintData;
    #endif

    GamePageData* data = Memory_Allocate(NULL, sizeof(GamePageData), ZEROVAL_ALL);
    page->data = data;
    
    Page_Resize(page);
    
    data->nRodsLeft = Board_GetNumUnelectrifiedRodsLeft(board);
    Toolbar_SetRodsLeft(toolbar, data->nRodsLeft);

    Grid gridSize = Board_GetGridSize(board);
    Time recordTime = Records_Get(Glo_Records, gridSize.nCols, gridSize.nRows);
    Toolbar_SetRecordTime(toolbar, recordTime);

    return page;
}


// **************************************************************************** GamePage_GetRGrid

// Return a pointer to the rod grid in the board gadget
RGrid* GamePage_GetRGrid(const Page* page){
    return Board_GetRGrid(page->gadgets[GP_BOARD]);
}


// **************************************************************************** GamePage_GetSGraph

// Return a pointer to the scroll graphics object in the board gadget
SGraph* GamePage_GetSGraph(const Page* page){
    return Board_GetSGraph(page->gadgets[GP_BOARD]);
}


// **************************************************************************** GamePage_GetCurrentTime

// Return the current time of the timer in the toolbar
Time GamePage_GetCurrentTime(const Page* page){
    return Toolbar_GetTime(page->gadgets[GP_TOOLBAR]);
}


// **************************************************************************** GamePage_GetGridSize

// return the size of the rod grid
Grid GamePage_GetGridSize(const Page* page){
    return Board_GetGridSize(page->gadgets[GP_BOARD]);
}






// ============================================================================ PRIVATE FUNC DEF

// **************************************************************************** GamePage_PrepareToShow

// Prepare the game page to show
static void GamePage_PrepareToShow(Page* page, UNUSED Event event){
    Board_SetAsReactive(page->gadgets[GP_BOARD]);
}


// **************************************************************************** GamePage_Resize

// Resize the game page to the current window size
static void GamePage_Resize(Page* page){
    
    Rect winRect = TO_RECT(Glo_WinSize);

    page->gadgets[GP_BOARD]->cRect = winRect;
    
    float sbarHorWidth = winRect.width * SBAR_HOR_RATIO;
    float sbarVerHeight = winRect.height * SBAR_VER_RATIO;
    Size sbarHorSize = SIZE(sbarHorWidth, SBAR_WIDTH);
    Size sbarVerSize = SIZE(SBAR_WIDTH, sbarVerHeight);

    page->gadgets[GP_SBAR_HOR]->cRect = Geo_AlignRect(TO_RECT(sbarHorSize), winRect, RP_BOTTOM_CENTER);
    page->gadgets[GP_SBAR_VER]->cRect = Geo_AlignRect(TO_RECT(sbarVerSize), winRect, RP_MIDDLE_RIGHT);

    // The toolbar resizes automatically
}


// **************************************************************************** GamePage_ReactToEvent

// React to GUI events
static void GamePage_ReactToEvent(Page* page, Event event, EventQueue* queue){
    switch (event.id){
        case EVENT_GADGET_EXPANDED: case EVENT_GADGET_COLLAPSED:{
            switch (event.source){
                case GDG_TOOLBAR: case GDG_SCROLLBAR_HOR: case GDG_SCROLLBAR_VER:{
                    if (event.id == EVENT_GADGET_EXPANDED){
                        Board_SetAsNotReactive(page->gadgets[GP_BOARD]);
                    }else{
                        Board_SetAsReactive(page->gadgets[GP_BOARD]);
                    }
                    break;
                }

                default: {break;}
            }
            break;
        }

        case EVENT_BUTTON_RELEASED:{
            switch (event.source){
                case GDG_BTN_BACK:{
                    Queue_AddEvent(queue, Event_SetAsShowPage(PAGE_GAME, PAGE_MAIN, WITH_ANIM));
                    Queue_AddEvent(queue, Event_SetAsHidePage(PAGE_GAME, PAGE_GAME, WITH_ANIM));
                    Toolbar_Collapse(page->gadgets[GP_TOOLBAR], WITHOUT_ANIM);
                    Board_SetAsReactive(page->gadgets[GP_BOARD]);

                    if (Board_GetNumUnelectrifiedRodsLeft(page->gadgets[GP_BOARD]) == 0){
                        Grid gridSize = Board_GetGridSize(page->gadgets[GP_BOARD]);
                        Queue_AddEvent(queue, Event_SetAsMakeNewGrid(PAGE_GAME, GDG_BOARD, gridSize.nCols, 
                                                                                           gridSize.nRows));
                    }
                    break;
                }

                case GDG_BTN_RESTART:{
                    Event tempEvent = Event_SetAsShowPage(page->id, PAGE_SETUP, WITH_ANIM);
                    Grid tempGridSize = GamePage_GetGridSize(page);
                    Event_AddPageData(&tempEvent, false, TIME_NULL, tempGridSize.nCols, tempGridSize.nRows);
                    Queue_AddEvent(queue, tempEvent);
                    Toolbar_Collapse(page->gadgets[GP_TOOLBAR], WITH_ANIM);
                    Board_SetAsReactive(page->gadgets[GP_BOARD]);
                    break;
                }

                case GDG_BTN_INFO:{
                    Queue_AddEvent(queue, Event_SetAsShowPage(page->id, PAGE_INFO, WITH_ANIM));
                    Toolbar_Collapse(page->gadgets[GP_TOOLBAR], WITH_ANIM);
                    Board_SetAsReactive(page->gadgets[GP_BOARD]);
                    break;
                }

                case GDG_BTN_HELP:{
                    Queue_AddEvent(queue, Event_SetAsShowPage(page->id, PAGE_HELP, WITH_ANIM));
                    Toolbar_Collapse(page->gadgets[GP_TOOLBAR], WITH_ANIM);
                    Board_SetAsReactive(page->gadgets[GP_BOARD]);
                    break;
                }

                default: {break;}
            }
            break;
        }

        case EVENT_BUTTON_PRESSED:{
            switch (event.source){
                case GDG_BTN_ZOOM_IN: case GDG_BTN_ZOOM_OUT:{
                    KeyboardKey key = (event.source == GDG_BTN_ZOOM_IN) ? WKEY_PLUS : WKEY_MINUS;
                    Queue_AddEvent(queue, Event_SetAsKeyPressed(key));
                    break;
                }

                default: {break;}
            }
            break;
        }

        case EVENT_SCROLLBAR_CHANGED:{
            switch (event.source){
                case GDG_SCROLLBAR_HOR: case GDG_SCROLLBAR_VER:{
                    E_Orientation orientation = (event.source == GDG_SCROLLBAR_HOR) ? OR_HORISONTAL : OR_VERTICAL;
                    SGraph_SetViewportPosFromRatio(Board_GetSGraph(page->gadgets[GP_BOARD]), 
                                                   event.data.scrollbar.posRatio, orientation);
                    break;
                }

                default: {break;}
            }

            break;
        }

        case EVENT_KEY_RELEASED:{
            if (event.data.key == WKEY_T){
                if (Toolbar_IsAnimating(page->gadgets[GP_TOOLBAR])) {break;}
                if (page->gadgets[GP_TOOLBAR]->isExpanded){
                    Toolbar_Collapse(page->gadgets[GP_TOOLBAR], WITH_ANIM);
                    Queue_AddEvent(queue, Event_SetAsGadgetCollapsed(GDG_TOOLBAR));
                }else{
                    Toolbar_Expand(page->gadgets[GP_TOOLBAR], WITH_ANIM);
                    Queue_AddEvent(queue, Event_SetAsGadgetExpanded(GDG_TOOLBAR));
                }
            }
            break;
        }

        case EVENT_MAKE_NEW_GRID:{
            Grid tempGridSize = GamePage_GetGridSize(page);
            Toolbar_SetRecordTime(page->gadgets[GP_TOOLBAR], Records_Get(Glo_Records, tempGridSize.nCols, tempGridSize.nRows));
            Toolbar_SetTime(page->gadgets[GP_TOOLBAR], TIME_NULL);
            Timer_Resume(page->gadgets[GP_TOOLBAR]->subGadgets[TB_TIMER]);
            Music_FadeOut();
            break;
        }

        case EVENT_VICTORY:{
            Timer_Pause(page->gadgets[GP_TOOLBAR]->subGadgets[TB_TIMER]);
            Time tempTime = Toolbar_GetTime(page->gadgets[GP_TOOLBAR]);
            Grid tempGridSize = GamePage_GetGridSize(page);
            Event tempEvent = Event_SetAsShowPage(page->id, PAGE_SETUP, WITH_ANIM);
            Event_AddPageData(&tempEvent, true, tempTime, tempGridSize.nCols, tempGridSize.nRows);
            Queue_AddEvent(queue, tempEvent);
            Sound_PlaySoundFX(SFX_VICTORY);
            Music_Start();
            break;
        }

        default: {break;}
    }
}


// **************************************************************************** GamePage_Update

// Update the rods left count
static void GamePage_Update(Page* page, UNUSED EventQueue* queue){
    int nRodsLeft = Board_GetNumUnelectrifiedRodsLeft(page->gadgets[GP_BOARD]);
    if (GPDATA->nRodsLeft != nRodsLeft){
        if (GPDATA->nRodsLeft > 0){
            Sound_PlaySoundFX(SFX_ELECTRIC);
        }
        GPDATA->nRodsLeft = nRodsLeft;
        Toolbar_SetRodsLeft(page->gadgets[GP_TOOLBAR], nRodsLeft);
    }
}


#ifdef DEBUG_MODE
// **************************************************************************** GamePage_PrintData

    // Multiline print of the parameters of the data object of the Game page
    static void GamePage_PrintData(const Page* page){
        CHECK_NULL(page, WITH_NEW_LINE)
        CHECK_NULL(page->data, WITH_NEW_LINE)

        printf("Rods Left: %d\n", GPDATA->nRodsLeft);
    }
#endif


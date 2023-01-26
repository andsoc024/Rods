// ============================================================================
// RODS
// Main Page
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions as well as constants for the Main Page.

    The Main Page is the first page shown when the program starts. It contains 
    the title and the play button.
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


// ============================================================================ PRIVATE CONSTANTS

#define VSCREEN                             SIZE(500, 1000)
#define TITLE_RECT                          RECT(150, 200, 200, 150)
#define PLAY_BTN_RECT                       RECT(100, 450, 300, 200)

#define TITLE_TEXT                          "RODS"
#define PLAY_BTN_TEXT                       "Play"

// Gadget Indices
#define MP_TITLE                            0
#define MP_PLAY_BTN                         1


// ============================================================================ PRIVATE FUNC DECL

static void     MainPage_Resize(Page* page);
static void     MainPage_ReactToEvent(Page* page, Event event, EventQueue* queue);






// ============================================================================ FUNC DEF

// **************************************************************************** MainPage_Make

// Make the main page
Page* MainPage_Make(void){
    Page* page = Page_Make(PAGE_MAIN);

    Gadget* title = Label_MakeMagic(GDG_TITLE, TITLE_TEXT, RP_CENTER);
    Page_AddGadget(page, title);

    Gadget* playBtn = Button_MakeAsText(GDG_BTN_PLAY, PLAY_BTN_TEXT);
    Button_SetColors(playBtn, COL_PLAY_BTN, COL_UI_FG_PRIMARY, COL_UI_FG_EMPH);
    Page_AddGadget(page, playBtn);

    page->Resize       = MainPage_Resize;
    page->ReactToEvent = MainPage_ReactToEvent;

    Page_Resize(page);

    return page;
}






// ============================================================================ PRIVATE FUNC DEF

// **************************************************************************** MainPage_Resize

// Resize the main page
static void MainPage_Resize(Page* page){
    VGraph* vg = VGraph_Make(VSCREEN, TO_RECT(Glo_WinSize), 0.0f);

    page->gadgets[MP_TITLE]->cRect = VGraph_ProjectRect(TITLE_RECT, vg);
    page->gadgets[MP_PLAY_BTN]->cRect = VGraph_ProjectRect(PLAY_BTN_RECT, vg);

    vg = VGraph_Free(vg);
}


// **************************************************************************** MainPage_Resize

// If the play button is pressed, show the game page
static void MainPage_ReactToEvent(UNUSED Page* page, Event event, EventQueue* queue){
    switch (event.id){
        case EVENT_BUTTON_RELEASED:{
            if (event.source == GDG_BTN_PLAY){
                Queue_AddEvent(queue, Event_SetAsShowPage(PAGE_MAIN, PAGE_GAME, WITH_ANIM));
                Queue_AddEvent(queue, Event_SetAsHidePage(PAGE_MAIN, PAGE_MAIN, WITH_ANIM));
            }
            break;
        }

        default: {break;}
    }
}


// ============================================================================
// RODS
// Router
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions for managing the router.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"
#include "../Fund/Fund.h"
#include "../Logic/Logic.h"
#include "../Graph/Graph.h"
#include "../Store/Store.h"
#include "../Sound/Sound.h"
#include "GUI.h"


// ============================================================================ PRIVATE FUNC DECL

static void     Router_Resize(const Router* router);
static void     Router_ReactToEvents(const Router* router);
static void     Router_Update(const Router* router);
static void     Router_Draw(const Router* router);






// ============================================================================ FUNC DEF

// **************************************************************************** Router_Make

// Make a router, empty of pages
Router* Router_Make(void){
    Router* router = Memory_Allocate(NULL, sizeof(Router), ZEROVAL_ALL);

    router->queue = Queue_Make();

    MCol_MakeDefault();

    router->fRects = FRects_Make();

    return router;
}


// **************************************************************************** Router_Free

// Free the router and all its contained structures. Return NULL
Router* Router_Free(Router* router){
    if (router == NULL) {return NULL;}

    for (int i = 0; i < PAGES_N; i++){
        router->pages[i] = Page_Free(router->pages[i]);
    }

    router->queue = Queue_Free(router->queue);

    router->fRects = FRects_Free(router->fRects);

    MCol_FreeDefault();

    return Memory_Free(router);
}


// **************************************************************************** Router_AddPage

// Add the page to the router
void Router_AddPage(Router* router, Page* page){
    if (router == NULL || page == NULL) {return;}

    if (!PageID_IsValid(page->id)){
        return;
    }

    if (router->pages[page->id] != NULL){
        router->pages[page->id] = Page_Free(router->pages[page->id]);
    }

    router->pages[page->id] = page;
}


// **************************************************************************** Router_GetPage

// Return a pointer to the page with the given page id
Page* Router_GetPage(const Router* router, E_PageID pageID){
    return router->pages[pageID];
}


// **************************************************************************** Router_ShowPage

// Show the page with the given id, optionally with animation
void Router_ShowPage(const Router* router, Event event, E_PageID pageID, bool withAnim){
    Page_Show(router->pages[pageID], event, withAnim);
}


// **************************************************************************** Router_HidePage

// Hide the page with the given id, optionally with animation
void Router_HidePage(const Router* router, E_PageID pageID, bool withAnim){
    Page_Hide(router->pages[pageID], withAnim);
}


// **************************************************************************** Router_Loop

// The main loop of the program
void Router_Loop(const Router* router){
    Music_Start();
    
    while (!WindowShouldClose()){
        if (IsWindowResized()){
            Window_UpdateWinSize();
            Router_Resize(router);
        }

        MCol_Update(Glo_MCol);
        FRects_Update(router->fRects);

        Music_Update();

        Router_ReactToEvents(router);

        Router_Update(router);

        BeginDrawing();
        ClearBackground(COL_BG);
        FRects_Draw(router->fRects);
        Router_Draw(router);
        EndDrawing();
    }
}


#ifdef DEBUG_MODE
// **************************************************************************** Router_Print

    // Multiline print of the parameters of the router
    void Router_Print(const Router* router){
        CHECK_NULL(router, WITH_NEW_LINE);

        printf("Event Queue: %p\n", (void*) router->queue);

        printf("PAGES:\n");
        for (int i = 0; i < PAGES_N; i++){
            printf("   %2d) ", i);
            if (router->pages[i] == NULL){
                printf("%s\n", STR_NULL);
            }else{
                printf("%s ", PageID_ToString(router->pages[i]->id));
                if (i != (int) router->pages[i]->id){
                    printf("MISMATCH");
                }
                printf("\n");
            }
        }

        printf("Flying Rects: %p\n", (void*) router->fRects);
    }
#endif






// ============================================================================ PRIVATE FUNC DEF

// **************************************************************************** Router_Resize

// Resize all the pages and the flying rectangles in the router to the current 
// window size
static void Router_Resize(const Router* router){
    FRects_Resize(router->fRects);

    for (int i = 0; i < PAGES_N; i++){
        Page_Resize(router->pages[i]);
    }
}


// **************************************************************************** Router_ReactToEvents

// Get mouse and keyboard events and let the last shown page to react to the 
// events in the event queue
static void Router_ReactToEvents(const Router* router){
    Queue_SetUserInput(router->queue, Event_GetUserInput());

    for (int i = PAGES_N - 1; i >= 0; i--){
        if (router->pages[i] != NULL && router->pages[i]->isShown){
            Event event;
            while ((event = Queue_GetNext(router->queue)).id != EVENT_NONE){
                if (event.id == EVENT_SHOW_PAGE){
                    Page_Show(router->pages[event.data.page.id], event, event.data.page.withAnim);
                    if (event.data.page.id == PAGE_GAME){
                        Music_FadeOut();
                    }else if (event.data.page.id == PAGE_MAIN){
                        Music_Start();
                    }
                }
                if (event.id == EVENT_HIDE_PAGE){
                    Page_Hide(router->pages[event.data.page.id], event.data.page.withAnim);
                }

                if (event.id == EVENT_KEY_PRESSED && event.data.key == WKEY_M){
                    Sound_Toggle();
                    Queue_AddEvent(router->queue, Event_SetAsUpdateSwitch(PAGE_GAME, GDG_SWITCH_SOUND, 
                                                                          Glo_SoundData->soundOn));
                }
                if (event.id == EVENT_SWITCH_CHANGED && event.source == GDG_SWITCH_SOUND){
                    if (event.data.switchValue == true){
                        Sound_TurnOn();
                    }else{
                        Sound_TurnOff();
                    }
                }

                Page_ReactToEvent(router->pages[i], event, router->queue);
            }
            break;
        }
    }
}


// **************************************************************************** Router_Update

// Update all the shown pages in the router
static void Router_Update(const Router* router){
    for (int i = 0; i < PAGES_N; i++){
        Page_Update(router->pages[i], router->queue);
    }
}


// **************************************************************************** Router_Draw

// Draw all the shown pages in the router
static void Router_Draw(const Router* router){
    for (int i = 0; i < PAGES_N; i++){
        Page_Draw(router->pages[i]);
    }
}



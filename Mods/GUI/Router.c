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
#include "GUI.h"


// ============================================================================ PRIVATE FUNC DECL

void            Router_Resize(Router* router);
void            Router_ReactToEvents(Router* router);
void            Router_Update(Router* router);
void            Router_Draw(Router* router);






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
Page* Router_GetPage(Router* router, E_PageID pageID){
    return router->pages[pageID];
}


// **************************************************************************** Router_ShowPage

// Show the page with the given id, optionally with animation
void Router_ShowPage(Router* router, E_PageID pageID, bool withAnim){
    Page_Show(router->pages[pageID], withAnim);
}


// **************************************************************************** Router_HidePage

// Hide the page with the given id, optionally with animation
void Router_HidePage(Router* router, E_PageID pageID, bool withAnim){
    Page_Hide(router->pages[pageID], withAnim);
}


// **************************************************************************** Router_Loop

// The main loop of the program
void Router_Loop(Router* router){
    while (!WindowShouldClose()){
        if (IsWindowResized()){
            Window_UpdateWinSize();
            Router_Resize(router);
        }

        MCol_Update(Glo_MCol);
        FRects_Update(router->fRects);

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
    void Router_Print(Router* router){
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
void Router_Resize(Router* router){
    FRects_Resize(router->fRects);

    for (int i = 0; i < PAGES_N; i++){
        Page_Resize(router->pages[i]);
    }
}


// **************************************************************************** Router_ReactToEvents

// Get mouse and keyboard events and let the last shown page to react to the 
// events in the event queue
void Router_ReactToEvents(Router* router){
    Queue_SetUserInput(router->queue, Event_GetUserInput());

    for (int i = PAGES_N - 1; i >= 0; i--){
        if (router->pages[i] != NULL && router->pages[i]->isShown){
            Event event;
            while ((event = Queue_GetNext(router->queue)).id != EVENT_NONE){
                if (event.id == EVENT_SHOW_PAGE){
                    Page_Show(router->pages[event.data.page.id], event.data.page.withAnim);
                }
                if (event.id == EVENT_HIDE_PAGE){
                    Page_Hide(router->pages[event.data.page.id], event.data.page.withAnim);
                }
                Page_ReactToEvent(router->pages[i], event, router->queue);
            }
            break;
        }
    }
}


// **************************************************************************** Router_Update

// Update all the shown pages in the router
void Router_Update(Router* router){
    for (int i = 0; i < PAGES_N; i++){
        Page_Update(router->pages[i], router->queue);
    }
}


// **************************************************************************** Router_Draw

// Draw all the shown pages in the router
void Router_Draw(Router* router){
    for (int i = 0; i < PAGES_N; i++){
        Page_Draw(router->pages[i]);
    }
}



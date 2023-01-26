// ============================================================================
// RODS
// Page
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================
/*
    Generic functions for managing pages.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"
#include "../Fund/Fund.h"
#include "GUI.h"


// ============================================================================ PRIVATE CONSTANTS

#define TRANS_ANIM_FRAMES_N                 ((FPS * 1) / 4)


// ============================================================================ PRIVATE FUNC DECL

static void     Page_UpdateTransAnim(Page* page);






// ============================================================================ FUNC DEF

// **************************************************************************** Page_Make

// Allocate memory for a page and initialize it with the given page id
Page* Page_Make(E_PageID id){
    Page* page = Memory_Allocate(NULL, sizeof(Page), ZEROVAL_ALL);

    page->id = id;

    return page;
}


// **************************************************************************** Page_Free

// Free the memory of the page
Page* Page_Free(Page* page){
    if (page == NULL) {return NULL;}

    if (page->PrepareToFree != NULL){
        page->PrepareToFree(page);
    }

    for (int i = 0; i < page->nGadgets; i++){
        page->gadgets[i] = Gadget_Free(page->gadgets[i]);
    }

    page->data = Memory_Free(page->data);

    return Memory_Free(page);
}


// **************************************************************************** Page_AddGadget

// Add the gadget to the page. Return true if successful (a page can have up to 
// PAGE_GADGETS_N gadgets)
bool Page_AddGadget(Page* page, Gadget* gadget){
    if (page == NULL || gadget == NULL) {return false;}
    
    if (page->nGadgets >= PAGE_GADGETS_N){
        return false;
    }

    page->gadgets[page->nGadgets] = gadget;
    page->nGadgets++;

    return true;
}


// **************************************************************************** Page_Resize

// Resize the page to the current window size
void Page_Resize(Page* page){
    if (page == NULL) {return;}

    if (page->Resize != NULL){
        page->Resize(page);
    }

    for (int i = 0; i < page->nGadgets; i++){
        Gadget_Resize(page->gadgets[i]);
    }

    if (page->ResizeAfterGadgets != NULL){
        page->ResizeAfterGadgets(page);
    }
}


// **************************************************************************** Page_ReactToEvent

// React to the event, if the page is shown. Optionally add events to the event 
// queue
void Page_ReactToEvent(Page* page, Event event, EventQueue* queue){
    if (event.id == EVENT_KEY_PRESSED && event.data.key == WKEY_TAB){
        Page_SelectNextGadget(page);
    }

    if (page->ReactToEvent != NULL){
        page->ReactToEvent(page, event, queue);
    }

    if (event.target == (int) page->id){
        Queue_MarkLastEventAsProcessed(queue);
    }

    for (int i = 0; i < page->nGadgets; i++){
        Gadget_ReactToEvent(page->gadgets[i], event, queue);
    }
}


// **************************************************************************** Page_Update

// Update the page, if shown. Optionally add events to the event queue
void Page_Update(Page* page, EventQueue* queue){
    if (page == NULL || !page->isShown) {return;}

    if (page->Update != NULL){
        page->Update(page, queue);
    }

    for (int i = 0; i < page->nGadgets; i++){
        Gadget_Update(page->gadgets[i], queue);
    }

    Page_UpdateTransAnim(page);
}


// **************************************************************************** Page_Draw

// Draw the page, if shown
void Page_Draw(const Page* page){
    if (page == NULL || !page->isShown) {return;}

    if (page->Draw != NULL){
        page->Draw(page);
    }

    for (int i = 0; i < page->nGadgets; i++){
        Gadget_Draw(page->gadgets[i], page->shift);
    }
}


// **************************************************************************** Page_SelectNextGadget

// Select the next selectable gadget in the page
void Page_SelectNextGadget(const Page* page){
    int selIndex = 0;
    for (int i = 0; i < page->nGadgets; i++){
        if (page->gadgets[i]->isSelected){
            selIndex = i;
            break;
        }
    }

    for (int i = selIndex; i < page->nGadgets; i++){
        if (Gadget_SelectNext(page->gadgets[i])){
            break;
        }
    }
}


// **************************************************************************** Page_DeselectAllGadgets

// Deselect all gadgets in the page
void Page_DeselectAllGadgets(const Page* page){
    for (int i = 0; i < page->nGadgets; i++){
        Gadget_Deselect(page->gadgets[i]);
    }
}


// **************************************************************************** Page_Show

// Show the page, optionally with animation
void Page_Show(Page* page, Event event, bool withAnim){
    if (page == NULL) {return;}

    Page_DeselectAllGadgets(page);

    page->isShown = true;

    if (withAnim){
        page->shift.x = Glo_WinSize.width;
        page->shiftIncr = -Glo_WinSize.width / (float) TRANS_ANIM_FRAMES_N;
    }else{
        page->shift = VECTOR_NULL;
        page->shiftIncr = 0.0f;
    }

    if (page->PrepareToShow != NULL){
        page->PrepareToShow(page, event);
    }
}


// **************************************************************************** Page_Hide

// Hide the page, optionally with animation
void Page_Hide(Page* page, bool withAnim){
    if (page == NULL) {return;}

    Page_DeselectAllGadgets(page);

    if (withAnim){
        page->shiftIncr = Glo_WinSize.width / (float) TRANS_ANIM_FRAMES_N;
    }else{
        page->shift = VECTOR_NULL;
        page->shiftIncr = 0.0f;
        page->isShown = false;
    }
}


// **************************************************************************** Page_IsAnimating

// Return true if the page is the page is during transition animation
bool Page_IsAnimating(const Page* page){
    return page== NULL || page->shiftIncr != 0.0f;
}


// **************************************************************************** Page_finishTr
void Page_FinishTransitionAnim(Page* page){
    page->shift = VECTOR_NULL;
    if (page->shiftIncr > 0.0f){
        page->isShown = false;
    }
    page->shiftIncr = 0.0f;
}


#ifdef DEBUG_MODE
// **************************************************************************** Page_Print

    // Multiline print of the page parameters
    void Page_Print(const Page* page){
        CHECK_NULL(page, WITH_NEW_LINE)

        printf("ID:         %s\n", PageID_ToString(page->id));
        printf("\n");

        printf("Shift:      "); Geo_PrintVector(page->shift, WITH_NEW_LINE);
        printf("Shift Incr: %.3f\n", page->shiftIncr);
        printf("\n");

        printf("GADGETS (%d):\n", page->nGadgets);
        int n = MIN(page->nGadgets, PAGE_GADGETS_N);
        for (int i = 0; i < n; i++){
            printf("   %2d) ", i);
            printf("%s\n", GadgetID_ToString(page->gadgets[i]->id));
        }
        if (n == 0){
            printf("%s\n", STR_EMPTY);
        }
        printf("\n");

        printf("FUNCTIONS:\n");
        printf("   PrepareToFree:      %p\n", (void*) page->PrepareToFree);
        printf("   PrepareToShow:      %p\n", (void*) page->PrepareToShow);
        printf("   Resize:             %p\n", (void*) page->Resize);
        printf("   ResizeAfterGadgets: %p\n", (void*) page->ResizeAfterGadgets);
        printf("   ReactToEvent:       %p\n", (void*) page->ReactToEvent);
        printf("   Update:             %p\n", (void*) page->Update);
        printf("   Draw:               %p\n", (void*) page->Draw);
        printf("   PrintData:          %p\n", (void*) page->PrintData);
        printf("\n");

        printf("DATA (%p):\n", (void*) page->data);
        if (page->PrintData != NULL){
            page->PrintData(page);
        }else{
            printf("%s\n", STR_NULL);
        }
    }
#endif






// ============================================================================ PRIVATE FUNC DEF

// **************************************************************************** Page_UpdateTransAnim

static void Page_UpdateTransAnim(Page* page){
    if (page->shiftIncr == 0.0f) {return;}

    page->shift.x += page->shiftIncr;

    // Hide animation that moves the page to the right
    if (page->shiftIncr > 0.0f){
        if (page->shift.x> Glo_WinSize.width){
            Page_FinishTransitionAnim(page);
        }
    }

    // Show animation that moves the page to the left
    if (page->shiftIncr < 0.0f){
        if (page->shift.x < 0.0f){
            Page_FinishTransitionAnim(page);
        }
    }
}




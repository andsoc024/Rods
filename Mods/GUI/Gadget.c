// ============================================================================
// RODS
// Gadget
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Generic functions, common to all gadgets.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"
#include "../Fund/Fund.h"
#include "GUI.h"


// ============================================================================ FUNC DEF

// **************************************************************************** Gadget_Make

// Allocate memory for the gadget
Gadget* Gadget_Make(E_GadgetID id, E_GadgetType type, bool isSelectable, int nSubGadgets){
    Gadget* gadget = Memory_Allocate(NULL, sizeof(Gadget), ZEROVAL_ALL);

    gadget->id = id;
    gadget->type = type;

    gadget->isSelectable = isSelectable;

    nSubGadgets = MAX(0, nSubGadgets);
    gadget->nSubGadgets = nSubGadgets;
    if (nSubGadgets > 0){
        gadget->subGadgets = Memory_Allocate(NULL, sizeof(Gadget) * nSubGadgets, ZEROVAL_ALL);
    }

    return gadget;
}


// **************************************************************************** Gadget_Free

// Free the memory of the gadget. Return NULL
Gadget* Gadget_Free(Gadget* gadget){
    if (gadget == NULL) {return NULL;}

    if (gadget->PrepareToFree != NULL){
        gadget->PrepareToFree(gadget);
    }

    for (int i = 0; i < gadget->nSubGadgets; i++){
        Gadget_Free(gadget->subGadgets[i]);
    }
    gadget->subGadgets = Memory_Free(gadget->subGadgets);

    gadget->data = Memory_Free(gadget->data);

    return Memory_Free(gadget);
}


// **************************************************************************** Gadget_Resize

// Resize the gadget to its containing rectangle, which must be previously set
void Gadget_Resize(Gadget* gadget){
    if (gadget->Resize != NULL){
        gadget->Resize(gadget);
    }

    for (int i = 0; i < gadget->nSubGadgets; i++){
        Gadget_Resize(gadget->subGadgets[i]);
    }

    if (gadget->ResizeAfterSubgadgets != NULL){
        gadget->ResizeAfterSubgadgets(gadget);
    }
}


// **************************************************************************** Gadget_ReactToEvent

// React to the given event and optionally add events to the event queue
void Gadget_ReactToEvent(Gadget* gadget, Event event, EventQueue* queue){
    if (gadget->ReactToEvent != NULL){
        gadget->ReactToEvent(gadget, event, queue);
    }

    if (event.target == (int) gadget->id){
        Queue_RemoveForTarget(queue, gadget->id);
    }

    for (int i = 0; i < gadget->nSubGadgets; i++){
        Gadget_ReactToEvent(gadget->subGadgets[i], event, queue);
    }
}


// **************************************************************************** Gadget_Update

// Update the gadget and optionally add events to the queue
void Gadget_Update(Gadget* gadget, EventQueue* queue){
    if (gadget->Update != NULL){
        gadget->Update(gadget, queue);
    }

    for (int i = 0; i < gadget->nSubGadgets; i++){
        Gadget_Update(gadget->subGadgets[i], queue);
    }
}


// **************************************************************************** Gadget_Draw

// Draw the gadget
void Gadget_Draw(const Gadget* gadget, Vector2 shift){
    if (gadget->Draw != NULL){
        gadget->Draw(gadget, shift);
    }

    for (int i = 0; i < gadget->nSubGadgets; i++){
        Gadget_Draw(gadget->subGadgets[i], shift);
    }
}


// **************************************************************************** Gadget_SelectNext

// Toggle the selected status of the gadget. If it has subgadgets, select the 
// next subgadgets and after the last deselect it. Return true if the gadget is 
// selected
bool Gadget_SelectNext(Gadget* gadget){
    if (!gadget->isSelectable){
        return false;
    }

    if (gadget->nSubGadgets <= 0){
        TOGGLE(gadget->isSelected)
        return gadget->isSelected;
    }

    int selectedInd = 0;
    for (int i = 0; i < gadget->nSubGadgets; i++){
        if (gadget->subGadgets[i]->isSelectable && gadget->subGadgets[i]->isSelected){
            selectedInd = i;
            break;
        }
    }

    for (int i = selectedInd; i < gadget->nSubGadgets; i++){
        if (Gadget_SelectNext(gadget->subGadgets[i])){
            gadget->isSelected = true;
            return true;
        }
    }

    gadget->isSelected = false;
    return false;
}


// **************************************************************************** Gadget_Deselect

// Deselect the gadget and all its subgadgets
void Gadget_Deselect(Gadget* gadget){
    for (int i = 0; i < gadget->nSubGadgets; i++){
        Gadget_Deselect(gadget->subGadgets[i]);
    }

    gadget->isSelected = false;
}


// **************************************************************************** Gadget_Print
#ifdef DEBUG_MODE

    // Multiline print of the gadget's parameters
    void Gadget_Print(const Gadget* gadget){
        CHECK_NULL(gadget, WITH_NEW_LINE)

        printf("ID:         %s\n", GadgetID_ToString(gadget->id));
        printf("Type:       %s\n", GadgetType_ToString(gadget->type));
        printf("\n");

        printf("CRect:      "); Geo_PrintRect(gadget->cRect, WITH_NEW_LINE);
        printf("\n");

        printf("Selectable: %s\n", Bool_ToString(gadget->isSelectable, LONG_FORM));
        printf("Selected:   %s\n", Bool_ToString(gadget->isSelected, LONG_FORM));
        printf("Pressed:    %s\n", Bool_ToString(gadget->isPressed, LONG_FORM));
        printf("Expanded:   %s\n", Bool_ToString(gadget->isExpanded, LONG_FORM));
        printf("\n");

        printf("SUBGGADGETS (%d):\n", gadget->nSubGadgets);
        for (int i = 0; i < gadget->nSubGadgets; i++){
            printf("   %2d) ", i);
            if (gadget->subGadgets[i] == NULL){
                printf("%s\n", STR_NULL);
                continue;
            }
            printf("%s\n", GadgetID_ToString(gadget->subGadgets[i]->id));
        }
        if (gadget->nSubGadgets == 0){
            printf("   %s\n", STR_EMPTY);
        }
        printf("\n");

        printf("FUNCTIONS:\n");
        printf("   PrepareToFree:         %p\n", (void*) gadget->PrepareToFree);
        printf("   Resize:                %p\n", (void*) gadget->Resize);
        printf("   ResizeAfterSubGadgets: %p\n", (void*) gadget->ResizeAfterSubgadgets);
        printf("   ReactToEvent:          %p\n", (void*) gadget->ReactToEvent);
        printf("   Update:                %p\n", (void*) gadget->Update);
        printf("   Draw:                  %p\n", (void*) gadget->Draw);
        printf("   PrintData:             %p\n", (void*) gadget->PrintData);
        printf("\n");

        printf("DATA (%p):\n", gadget->data);
        if (gadget->PrintData != NULL){
            gadget->PrintData(gadget);
        }else{
            printf("%s\n", STR_NULL);
        }
    }
#endif



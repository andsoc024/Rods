// ============================================================================
// RODS
// Test
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Function for testing and debugging. Compiled and executed only when in 
    DEBUG_MODE.
*/


// ============================================================================ DEPENDENCIES

// Included in main.c, only in DEBUG_MODE

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "Mods/Assets/Assets.h"

#include "Mods/Public/Public.h"
#include "Mods/Fund/Fund.h"
#include "Mods/Logic/Logic.h"
#include "Mods/Graph/Graph.h"
#include "Mods/Store/Store.h"
#include "Mods/GUI/GUI.h"


// ============================================================================ TEST

// Function for testing and debugging
int Test(UNUSED int argc, UNUSED char** argv){

    Gadget* gadget1  = Gadget_Make(GDG_GENERIC_1,  GT_GENERIC, IS_SELECTABLE,     3);
    Gadget* gadget2  = Gadget_Make(GDG_GENERIC_2,  GT_GENERIC, IS_NOT_SELECTABLE, 0);
    Gadget* gadget3  = Gadget_Make(GDG_GENERIC_3,  GT_GENERIC, IS_SELECTABLE,     0);
    Gadget* gadget4  = Gadget_Make(GDG_GENERIC_4,  GT_GENERIC, IS_SELECTABLE,     2);

    Gadget* gadget5  = Gadget_Make(GDG_GENERIC_5,  GT_GENERIC, IS_SELECTABLE,     0);
    Gadget* gadget6  = Gadget_Make(GDG_GENERIC_6,  GT_GENERIC, IS_NOT_SELECTABLE, 0);
    Gadget* gadget7  = Gadget_Make(GDG_GENERIC_7,  GT_GENERIC, IS_SELECTABLE,     2);
    Gadget* gadget8  = Gadget_Make(GDG_GENERIC_8,  GT_GENERIC, IS_NOT_SELECTABLE, 0);
    Gadget* gadget9  = Gadget_Make(GDG_GENERIC_9,  GT_GENERIC, IS_SELECTABLE,     0);

    Gadget* gadget10 = Gadget_Make(GDG_GENERIC_10, GT_GENERIC, IS_SELECTABLE,     0);
    Gadget* gadget11 = Gadget_Make(GDG_GENERIC_11, GT_GENERIC, IS_SELECTABLE,     1);

    Gadget* gadget12 = Gadget_Make(GDG_GENERIC_12, GT_GENERIC, IS_SELECTABLE,     0);

    gadget1->subGadgets[0]  = gadget5;
    gadget1->subGadgets[1]  = gadget6;
    gadget1->subGadgets[2]  = gadget7;

    gadget4->subGadgets[0]  = gadget8;
    gadget4->subGadgets[1]  = gadget9;

    gadget7->subGadgets[0]  = gadget10;
    gadget7->subGadgets[1]  = gadget11;

    gadget11->subGadgets[0] = gadget12;

    Gadget* mainGadgets[] = {gadget1, gadget2, gadget3, gadget4};
    int nMain = sizeof(mainGadgets) / sizeof(mainGadgets[0]);
    Gadget* allGadgets[] = {gadget1, gadget2, gadget3, gadget4,  gadget5,  gadget6,
                            gadget7, gadget8, gadget9, gadget10, gadget11, gadget12};
    int nAll = sizeof(allGadgets) / sizeof(allGadgets[0]);
    
    for (int i = 0; i < 20; i++){
        int selIndex = 0;
        for (int j = 0; j < nMain; j++){
            if (mainGadgets[j]->isSelected){
                selIndex = j;
                break;
            }
        }

        for (int j = selIndex; j < nMain; j++){
            if (Gadget_SelectNext(mainGadgets[j])){
                break;
            }
        }

        for (int j = 0; j < nAll; j++){
            if (allGadgets[j]->isSelected){
                printf("%s\n", GadgetID_ToString(allGadgets[j]->id));
            }
        }
        PRINT_LINE3
    }

    for (int i = 0; i < nMain; i++){
        Gadget_Deselect(mainGadgets[i]);
    }
    for (int j = 0; j < nAll; j++){
        if (allGadgets[j]->isSelected){
            printf("%s\n", GadgetID_ToString(allGadgets[j]->id));
        }
    }
    PRINT_LINE3

    for (int i = 0; i < nMain; i++){
        Gadget_Free(allGadgets[i]);
    }

    return 0;
}




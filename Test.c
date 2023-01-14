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

#include "Mods/Public/Public.h"
#include "Mods/Fund/Fund.h"
#include "Mods/Logic/Logic.h"


// ============================================================================ TEST

// Function for testing and debugging
int Test(UNUSED int argc, UNUSED char** argv){

    RGrid* rGrid = RGrid_MakeEmpty(10, 10);

    RGrid_CreateRandom(rGrid);
    RGrid_Print(rGrid);

    RGrid_Reelectrify(rGrid);
    RGrid_Print(rGrid);

    RGrid_Shuffle(rGrid);
    RGrid_Print(rGrid);

    for (int i = 0; i < 4; i++){
        RGrid_RotateRod(rGrid, RGrid_GetSource(rGrid));
        RGrid_Print(rGrid);
        while (RGrid_IsAnimating(rGrid)){
            RGrid_Update(rGrid);
        }
        RGrid_Print(rGrid);
    }

    rGrid = RGrid_Free(rGrid);

    return 0;
}



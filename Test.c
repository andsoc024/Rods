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

    Rod rod = ROD_NULL;
    Rod_Print(&rod, WITH_NEW_LINE);

    Rod_Set(&rod, LEGDIR_RIGHT | LEGDIR_UP);
    Rod_Print(&rod, WITH_NEW_LINE);

    Rod_Rotate(&rod, 1, WITH_ANIM);
    Rod_FinishAnim(&rod);
    Rod_Print(&rod, WITH_NEW_LINE);

    while ((Rod_Update(&rod) == NOT_COMPLETED) & Rod_IsAnimating(&rod));
    Rod_Print(&rod, WITH_NEW_LINE);

    Rod rod2;
    Rod_Set(&rod2, LEGDIR_UP);

    PRINT_LINE3

    for (int dir = DIR_RIGHT; dir <= DIR_UP; dir++){
        printf("\t%s: %s\n", Direction_ToString(dir, SHORT_FORM), 
                             Bool_ToString(Rod_IsConnectedToRod(&rod, &rod2, dir), LONG_FORM));
    }

    return 0;
}



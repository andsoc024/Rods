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

    Records_MakeDefault();

    for (int i = 0; i < 100; i++){
        Records_Set(Glo_Records, Time_FromInt(Math_RandomInt(0, TIME_MAX_SECS)), 
                    Math_RandomInt(RGRID_MIN_SIZE, RGRID_MAX_SIZE), 
                    Math_RandomInt(RGRID_MIN_SIZE, RGRID_MAX_SIZE));
    }

    Records_Print(Glo_Records);

    Records_Free(Glo_Records);

    return 0;
}



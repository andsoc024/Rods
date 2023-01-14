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


// ============================================================================ TEST

// Function for testing and debugging
int Test(UNUSED int argc, UNUSED char** argv){

    Time t = TIME_NULL;
    Time_Print(t, WITH_NEW_LINE);

    while (Time_IsSmaller(t, TIME_MAX)){
        t = Time_Shift(t, 500);
        Time_Print(t, WITH_NEW_LINE);
    }

    Time_Print(t, WITH_NEW_LINE);

    printf("%s\n", Bool_ToString(Time_IsEqual(t, TIME_MAX), LONG_FORM));
    printf("%s\n", Bool_ToString(Time_IsEqual(t, TIME_INVALID), LONG_FORM));

    return 0;
}



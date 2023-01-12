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
    
    for (int i = -360; i <= 720; i += 15){
        printf("%4d\t", i);

        float radians = Math_DegToRad(i);
        
        float s = Math_Sin(radians);
        float c = Math_Cos(radians);
        printf("%.3f\t%.3f\t", s, c);

        printf("%.1f\n", Math_Hypotenuse(s, c));
    }

    return 0;
}



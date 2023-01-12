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
    
    for (int i = 0; i < 100; i++){
        int exp = Math_RandomInt(1, 7);
        int limit = 5 * Math_PowI(10, exp);
        float num = Math_RandomFloat(-limit, limit);
        if (i == 0) {num = 0;}
        printf("%.3f\t", num);

        Bytes bytes = Bytes_FromFloat(num);
        Bytes_Print(bytes, WITHOUT_NEW_LINE);
        printf("\t");

        float num2 = Bytes_ToFloat(bytes);
        printf("%.3f\t", num2);

        if (!FEQ(num, num2)){
            printf("ERROR!");
        }

        printf("\n");
    }

    return 0;
}



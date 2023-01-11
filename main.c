// ============================================================================
// RODS
// Main
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    The main function of the program.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "Mods/Public/Public.h"


// ============================================================================ MAIN

// The main function of the programn
int main(__attribute__((unused)) int argc, __attribute__((unused)) char** argv){
    
    printf("Platform: %s\n", APP_PLATFORM);

    PRINT_LINE3

    int a = 5;
    int b = 15;
    printf("a:%2d, b:%2d\n", a, b);

    SWAP(a, b, int)
    printf("a:%2d, b:%2d\n", a, b);

    const int minV = MIN(a, b);
    const int maxV = MAX(a, b);
    printf("min:%d, max:%2d\n", minV, maxV);

    PRINT_LINE3

    for (int i = -3; i <= 20; i++){
        printf("%2d ", i);

        bool inRange = IS_IN_RANGE(i, minV, maxV);
        if (inRange){
            printf("     \t");
        }else{
            printf("-> %2d\t", PUT_IN_RANGE(i, minV, maxV));
        }

        printf("%2d  ", ABS(i));
        printf("%3d  ", SQR(i));
        printf("%.3f\t", AVG(i, 10.0f));

        printf("%s ", Bool_ToString(inRange, SHORT_FORM));
        TOGGLE(inRange)
        printf("%s\n", Bool_ToString(inRange, LONG_FORM));
    }
    
}


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

    char* str1 = String_Copy(NULL, "John");
    char* str2 = String_Copy(NULL, "X");
    char* str3 = String_Copy(NULL, "Smith");

    str1 = String_Concat(str1, 6, str1, " ", NULL, str2, " ", str3);
    printf("%s\n", str1);

    char* str = NULL;
    for (int i = 0; i < 100; i++){
        int num = Math_RandomInt(-5000, 5000);
        num = (i == 0) ? 0 : num;
        printf("%5d\t", num);

        str = String_FromInt(str, num);
        str = String_Pad(str, 5, ' ');
        printf("%s\n", str);
    }

    Memory_FreeAll(4, &str, &str1, &str2, &str3);

    return 0;
}



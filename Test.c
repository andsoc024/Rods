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
    
    char* str = Memory_Copy(NULL, "John", 5);
    printf("%s ", str);

    str = Memory_Copy(str, "X", 2);
    printf("%s ", str);

    str = Memory_Copy(str, "Smith", 6);
    printf("%s\n", str);

    char* str1 = Memory_Allocate(NULL, 4, ZEROVAL_LAST);
    Memory_Set(str1, 3, '@');
    printf("%s\n", str1);

    Memory_FreeAll(2, &str, &str1);

    Err_Assert(1 < 2, "1 < 2");
    Err_Assert(1 > 2, "1 > 2");

    return 0;
}



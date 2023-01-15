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

#include "Mods/Assets/Assets.h"

#include "Mods/Public/Public.h"
#include "Mods/Fund/Fund.h"
#include "Mods/Logic/Logic.h"
#include "Mods/Graph/Graph.h"

#ifdef DEBUG_MODE
    #include "Test.c"
#endif

// ============================================================================ MAIN

// The main function of the programn
int main(UNUSED int argc, UNUSED char** argv){
    
    #ifdef DEBUG_MODE
        return Test(argc, argv);
    #endif

    printf("Main\n");

    return 0;
}


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

#include "Mods/Assets/Assets.h"

#include "Mods/Public/Public.h"
#include "Mods/Fund/Fund.h"
#include "Mods/Logic/Logic.h"
#include "Mods/Graph/Graph.h"
#include "Mods/Store/Store.h"
#include "Mods/Store/File_Internal.h"


// ============================================================================ TEST

// Function for testing and debugging
int Test(UNUSED int argc, UNUSED char** argv){

    Glo_FilePath = Path_GetDataFile();


    FILE* file = fopen(Glo_FilePath, "w");
    File_WriteSep(file);
    fclose(file);

    file = fopen(Glo_FilePath, "r");
    printf("%s\n", Bool_ToString(File_ReadSep(file), LONG_FORM));
    printf("%s\n", Bool_ToString(File_ReadSep(file), LONG_FORM));
    fclose(file);


    Glo_FilePath = Memory_Free(Glo_FilePath);

    return 0;
}



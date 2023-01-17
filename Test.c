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


// ============================================================================ TEST

// Function for testing and debugging
int Test(UNUSED int argc, UNUSED char** argv){

    Records* records = Records_Make();
    for (int i = 0; i < 10; i++){
        Records_Set(records, Time_FromInt(Math_RandomInt(0, TIME_MAX_SECS)), 
                             Math_RandomInt(RGRID_MIN_SIZE, RGRID_MAX_SIZE), 
                             Math_RandomInt(RGRID_MIN_SIZE, RGRID_MAX_SIZE));
    }
    Records_Set(records, TIME(1,20,30), RGRID_MIN_SIZE, RGRID_MIN_SIZE);
    Records_Set(records, TIME(1,20,30), RGRID_MAX_SIZE, RGRID_MAX_SIZE);

    Grid grid = GRID0(10, 10);
    RGrid* rGrid = RGrid_MakeEmpty(grid.nCols, grid.nRows);
    RGrid_CreateRandom(rGrid);

    SGraph* sg = SGraph_MakeFromGrid(grid, 100, RECT(10, 10, 700, 500), SGRAPH_DEF_MARGIN, 
                                     ROD_MIN_TEXTURE_SIZE, ROD_MAX_TEXTURE_SIZE);
    SGraph_Zoom(sg, 2.0f);
    SGraph_MoveViewport(sg, 0, -10000);

    Time t = TIME(1, 20, 30);
    bool sound = true;

    PRINT_LINE3
    Records_Print(records);
    PRINT_LINE
    RGrid_Print(rGrid);
    PRINT_LINE
    SGraph_Print(sg);
    PRINT_LINE
    printf("Time: "); Time_Print(t, WITH_NEW_LINE);
    printf("Sound: %s\n", Bool_ToString(sound, LONG_FORM));
    PRINT_LINE3

    
    FILE* file = fopen(Glo_FilePath, "w");
    PData_WriteToFile(records, rGrid, sg, t, sound);
    fclose(file);
    

    records = Records_Free(records);
    rGrid = RGrid_Free(rGrid);
    sg = SGraph_Free(sg);
    t = TIME_INVALID;
    sound = false;
    

    file = fopen(Glo_FilePath, "r");
    PData* pData = PData_ReadFromFile();
    fclose(file);

    PData_Print(pData);
    
    pData = PData_Free(pData);

    Glo_FilePath = Memory_Free(Glo_FilePath);

    return 0;
}



// ============================================================================
// RODS
// Records
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions for managing the Records structure.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"
#include "../Fund/Fund.h"
#include "Logic.h"


// ============================================================================ PRIVATE CONSTANTS

#define RECORDS_DIM_SIZE                    (RGRID_MAX_SIZE - RGRID_MIN_SIZE + 1)
// ============================================================================ OPAQUE STRUCTURES

// Structure for storing the minimum record time for various grid sizes
struct Records{
    Time values[RECORDS_DIM_SIZE][RECORDS_DIM_SIZE];
};






// ============================================================================ FUNC DEF

// **************************************************************************** Records_Make

// Make an empty Records structure
Records* Records_Make(void){
    Records* records = Memory_Allocate(NULL, sizeof(Records), ZEROVAL_NONE);

    Records_Clear(records);

    return records;
}


// **************************************************************************** Records_Copy

// Copy the Records structure from src to dst
Records* Records_Copy(Records* dst, const Records* src){
    return Memory_Copy(dst, src, sizeof(Records));
}


// **************************************************************************** Records_Free

// Free the memory of the Records structure. Return NULL
Records* Records_Free(Records* records){
    return Memory_Free(records);
}


// **************************************************************************** Records_Clear

// Set all times to TIME_INVALID
void Records_Clear(Records* records){
    for (int y = RGRID_MIN_SIZE; y <= RGRID_MAX_SIZE; y++){
        for (int x = RGRID_MIN_SIZE; x <= RGRID_MAX_SIZE; x++){
            records->values[x - RGRID_MIN_SIZE][y - RGRID_MIN_SIZE] = TIME_INVALID;
        }
    }
}


// **************************************************************************** Records_Get

// Get the minimum record time for the given grid size
Time Records_Get(const Records* records, int nCols, int nRows){
    if (!IS_IN_RANGE(nCols, RGRID_MIN_SIZE, RGRID_MAX_SIZE) || 
        !IS_IN_RANGE(nRows, RGRID_MIN_SIZE, RGRID_MAX_SIZE)){
        return TIME_INVALID;
    }

    return records->values[nCols - RGRID_MIN_SIZE][nRows - RGRID_MIN_SIZE];
}


// **************************************************************************** Records_Set

// Set the minimum record time for the given grid size
void Records_Set(Records* records, Time t, int nCols, int nRows){
    if (!IS_IN_RANGE(nCols, RGRID_MIN_SIZE, RGRID_MAX_SIZE) || 
        !IS_IN_RANGE(nRows, RGRID_MIN_SIZE, RGRID_MAX_SIZE)){
        return;
    }

    if (!Time_IsValid(t)){
        t = TIME_INVALID;
    }

    records->values[nCols - RGRID_MIN_SIZE][nRows - RGRID_MIN_SIZE] = t;
}


// **************************************************************************** Records_N

// The number of valid times in the record
int Records_N(const Records* records){
    int res = 0;
    for (int y = RGRID_MIN_SIZE; y <= RGRID_MAX_SIZE; y++){
        for (int x = RGRID_MIN_SIZE; x <= RGRID_MAX_SIZE; x++){
            Time temp = records->values[x - RGRID_MIN_SIZE][y - RGRID_MIN_SIZE];
            if (Time_IsValid(temp)){
                res++;
            }
        }
    }

    return res;
}


// **************************************************************************** Records_MakeDefault

// Make the Records structure at Glo_Records
void Records_MakeDefault(void){
    Records_FreeDefault();
    Glo_Records = Records_Make();
}


// **************************************************************************** Records_FreeDefault

// Free the memory of the Records structure at Glo_Records
void Records_FreeDefault(void){
    Glo_Records = Records_Free(Glo_Records);
}


#ifdef DEBUG_MODE
// **************************************************************************** Records_Print

    // Multiline print of all the valid times in the Records structure
    void Records_Print(const Records* records){
        CHECK_NULL(records, WITH_NEW_LINE)

        int n = Records_N(records);

        printf("Records (%d):\n", n);

        int count = 0;
        for (int y = RGRID_MIN_SIZE; y <= RGRID_MAX_SIZE; y++){
            for (int x = RGRID_MIN_SIZE; x <= RGRID_MAX_SIZE; x++){
                Time t = Records_Get(records, x, y);
                if (!Time_IsValid(t)) {continue;}
                count++;
                printf(" %5d) %3d x %3d:   ", count, x, y);
                Time_Print(t, WITH_NEW_LINE);
            }
        }
    }
#endif


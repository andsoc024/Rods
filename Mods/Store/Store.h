// ============================================================================
// RODS
// Store Header
// by Andreas Socratous
// Jan 2023
// ============================================================================


#ifndef STORE_GUARD
#define STORE_GUARD


// ============================================================================ INFO
/*
    Functions for file management and storage of persistent data.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"


// ============================================================================ FUNC DECL

// ---------------------------------------------------------------------------- Path Functions

char*           Path_GetDataFile(void);
bool            Path_FileExists(const char* path);
bool            Path_DirExists(const char* path);

// ---------------------------------------------------------------------------- Persistent Data Functions

PData*          PData_MakeEmpty(void);
PData*          PData_Free(PData* pData);
bool            PData_WriteToFile(const Records* records, const RGrid* rGrid, const SGraph* sg, 
                                  Time time, bool sound);
PData*          PData_ReadFromFile(void);
#ifdef DEBUG_MODE
    void        PData_Print(PData* pData);
#endif



#endif // STORE_GUARD



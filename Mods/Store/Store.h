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

// ---------------------------------------------------------------------------- Path_Functions

char*           Path_GetDataFile(void);
bool            Path_FileExists(const char* path);
bool            Path_DirExists(const char* path);





#endif // STORE_GUARD



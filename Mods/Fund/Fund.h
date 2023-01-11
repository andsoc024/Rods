// ============================================================================
// RODS
// Fundamentals Header
// by Andreas Socratous
// Jan 2023
// ============================================================================


#ifndef FUND_GUARD
#define FUND_GUARD


// ============================================================================ INFO
/*
    Functions for error handling and memory management, math functions, 
    functions for working with the Bytes structure and directions, for string 
    manipulation, geometry and working with grid structures, time and for 
    managing the program's window. 
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"


// ============================================================================ FUNC DECL

// ---------------------------------------------------------------------------- Error Functions

void            Err_FatalError(const char* errorMsg);
void            Err_Assert(int condition, const char* errorMsg);

// ---------------------------------------------------------------------------- Memory Functions

void*           Memory_Allocate(void* ptr, int size, E_ZeroValMode zeroValMode);
void*           Memory_Copy(void* dst, const void* src, int size);
void            Memory_Write(void* dst, const void* src, int size);
void            Memory_Set(void* ptr, int size, unsigned char value);
void*           Memory_Free(void* ptr);
void            Memory_FreeAll(int nObjects, ...);


#endif // FUND_GUARD



// ============================================================================
// RODS
// Structures (Fundamental) Header
// by Andreas Socratous
// Jan 2023
// ============================================================================


#ifndef STRUCT_FUND_GUARD
#define STRUCT_FUND_GUARD


// ============================================================================ INFO
/*
    Math, geometry, grid and time structures, as well as logic and graphics 
    structures.
*/


// ============================================================================ DEPENDENCIES

// Included in Public.h


// ============================================================================ STRUCTURES

// ---------------------------------------------------------------------------- Math Structures

// **************************************************************************** Bytes

// 1-4 bytes representing an integer or a float
typedef struct Bytes{
    unsigned char values[4];
    int n;
}Bytes;




#endif // STRUCT_FUND_GUARD


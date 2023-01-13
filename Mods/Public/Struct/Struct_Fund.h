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


// ---------------------------------------------------------------------------- Geometry Structures

// **************************************************************************** Point

// A point in the cartesian plane with float x, y coords. Typedef of Vector2
typedef Vector2 Point;
    // float x;
    // float y;


// **************************************************************************** Size

// Size with float width and height
typedef struct Size{
    float width;
    float height;
}Size;


// **************************************************************************** Rect

// A rectangle with top left corner at (x,y) and float width and height. 
// Typedef of Rectangle
typedef Rectangle Rect;
    // float x;
    // float y;
    // float width;
    // float height;
    


#endif // STRUCT_FUND_GUARD


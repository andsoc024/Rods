// ============================================================================
// RODS
// Macros (Fundamental) Header
// by Andreas Socratous
// Jan 2023
// ============================================================================ 


#ifndef MACRO_FUND_GUARD
#define MACRO_FUND_GUARD


// ============================================================================ INFO
/*
    Fundamental macros about math, geometry, grid, logic and graphics.
*/


// ============================================================================ DEPENDENCIES

// Included in Public.h


// ============================================================================ MACROS

// ---------------------------------------------------------------------------- Misc Macros

// **************************************************************************** ARCH_DEF

// The platform specific variant of the constant
#define ARCH_DEF(gConst) ARCH_DEF1(gConst, ARCH)
#define ARCH_DEF1(gConst, gArch) ARCH_DEF2(gConst, gArch)
#define ARCH_DEF2(gConst, gArch) gConst ## _ ## gArch


// **************************************************************************** MAKE_2D_ARR

// Allocate memory for a 2D array of the given type and size
#define MAKE_2D_ARR(gVar, gX, gY, gType, gZeroValMode) \
    (gVar) = Memory_Allocate(NULL, sizeof(gType*) * (gX), ZEROVAL_NONE); \
    for (int x = 0; x < (gX); x++){ \
        (gVar)[x] = Memory_Allocate(NULL, sizeof(gType) * (gY), (gZeroValMode)); \
    }


// **************************************************************************** FREE_2D_ARR

// Free the memory of the 2D array
#define FREE_2D_ARR(gVar, gX) \
    for (int x = 0; x < (gX); x++){ \
        (gVar)[x] = Memory_Free((gVar)[x]); \
    } \
    (gVar) = Memory_Free(gVar);






// ---------------------------------------------------------------------------- Print Macros

// **************************************************************************** PRINT_LINE

// Print a line in the terminal
#define PRINT_LINE \
    printf("%s\n", STR_LINE);

// Print a line in the terminal with an empty line above and below
#define PRINT_LINE3 \
    printf("\n"); \
    PRINT_LINE \
    printf("\n");


// **************************************************************************** CHECK_INVALID

// For use in ToString functions. If the isValid function returns false, return 
// the invalid string, in long or short form
#define CHECK_INVALID(isValid, gForm) \
    if (!(isValid)){ \
        return ((gForm) == SHORT_FORM) ? STR_INVALID_SHORT : STR_INVALID; \
    }






// ---------------------------------------------------------------------------- Math Macros

// **************************************************************************** MIN

// The minimum of the two numbers
#define MIN(v1, v2) \
    (((v1) < (v2)) ? (v1) : (v2))


// **************************************************************************** MAX

// The maximum of the two numbers
#define MAX(v1, v2) \
    (((v1) > (v2)) ? (v1) : (v2))


// **************************************************************************** IS_IN_RANGE

// Evaluates to true if the number is within and including the limits
#define IS_IN_RANGE(v, minV, maxV) \
    ((v) >= (minV) && (v) <= (maxV))


// **************************************************************************** PUT_IN_RANGE

// Ensure that the number is within and including the limits
#define PUT_IN_RANGE(v, minV, maxV) \
    MIN(maxV, MAX(minV, v))


// **************************************************************************** ABS

// The absolute value
#define ABS(v) \
    (((v) < 0) ? -(v) : (v))


// **************************************************************************** SQR

// The number raised to the power of 2
#define SQR(v) \
    ((v) * (v))


// **************************************************************************** AVG

// the average of the two numbers
#define AVG(v1, v2) \
    (((v1) + (v2)) * 0.5f)


// **************************************************************************** TOGGLE

// Toggle the boolean
#define TOGGLE(v) \
    (v) = !(v);


// **************************************************************************** SWAP

// Swap the values of the two variables of the given type
#define SWAP(v1, v2, gType) \
    { \
        gType swapTemp = (v1); \
        (v1) = (v2); \
        (v2) = swapTemp; \
    }


// **************************************************************************** FEQ

// Evaluates to true if the two floats are equal with the default precision
#define FEQ(v1, v2) \
    Math_FloatsAreEqual(v1, v2, MATH_FLOAT_PRECISION)






// ---------------------------------------------------------------------------- Graphics Macros

// **************************************************************************** COLOR

// Define a color
#define COLOR(gR, gG, gB, gA) \
    ((Color) {(gR), (gG), (gB), (gA)})





#endif // MACRO_FUND_GUARD


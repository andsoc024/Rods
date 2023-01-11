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






// ---------------------------------------------------------------------------- Graphics Macros

// **************************************************************************** COLOR

// Define a color
#define COLOR(gR, gG, gB, gA) \
    ((Color) {(gR), (gG), (gB), (gA)})





#endif // MACRO_FUND_GUARD


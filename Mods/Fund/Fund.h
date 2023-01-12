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

// ---------------------------------------------------------------------------- Math Functions

int             Math_RandomInt(int minV, int maxV);
float           Math_RandomFloat(float minV, float maxV);
bool            Math_FloatsAreEqual(float num1, float num2, float precision);
float           Math_Sqrt(float num);
float           Math_RightTriangleSide(float side2, float hypotenuse);
float           Math_Hypotenuse(float side1, float side2);
int             Math_PowI(int base, int exp);
float           Math_PowF(float base, int exp);
int             Math_Factorial(int num);
int             Math_Round(float num);
float           Math_RadToDeg(float radians);
float           Math_DegToRad(float degrees);
float           Math_PrincipalAngle(float angle);
int             Math_AngleQuarter(float angle);
float           Math_Sin(float angle);
float           Math_Cos(float angle);
float           Math_Tan(float angle);

// ---------------------------------------------------------------------------- Bytes Functions

bool            Bytes_IsValid(Bytes bytes);
Bytes           Bytes_FromInt(int num, int nBytes);
int             Bytes_ToInt(Bytes bytes);
Bytes           Bytes_FromFloat(float num);
float           Bytes_ToFloat(Bytes bytes);
#ifdef DEBUG_MODE
    void        Bytes_Print(Bytes bytes, bool withNewLine);
#endif



#endif // FUND_GUARD



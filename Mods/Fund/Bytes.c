// ============================================================================
// RODS
// Bytes
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions for working with the Bytes structure.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"
#include "Fund.h"


// ============================================================================ PRIVATE CONSTANTS

#define FLOAT_MAX_EXP                       64


// ============================================================================ FUNC DEF

// **************************************************************************** Bytes_IsValid

// Return true if the bytes structure is valid
bool Bytes_IsValid(Bytes bytes){
    return IS_IN_RANGE(bytes.n, 1, 4);
}


// **************************************************************************** Bytes_FromInt

// Turn a signed integer to a collection of 1 to 4 bytes
Bytes Bytes_FromInt(int num, int nBytes){
    // Small endian. The first bit of the last byte represents the sign
    nBytes = PUT_IN_RANGE(nBytes, 1, 4);

    bool isNegative = (num < 0);
    num = ABS(num);

    Bytes bytes = BYTES_NULL;
    bytes.n = nBytes;

    for (int i = 0; i < nBytes; i++){
        int mask = (i == nBytes - 1) ? ((1 << 7) - 1) : 0xFF;
        bytes.values[i] = (num & mask);
        num >>= 8;
    }

    if (isNegative){
        bytes.values[nBytes - 1] |= (1 << 7);
    }

    return bytes;
}


// **************************************************************************** Bytes_ToInt

// Turn the bytes structure to a signed integer. Return INVALID, if invalid
int Bytes_ToInt(Bytes bytes){
    if (!Bytes_IsValid(bytes)) {return INVALID;}

    bool isNegative = (bytes.values[bytes.n - 1] & (1 << 7));
    bytes.values[bytes.n - 1] &= ((1 << 7) - 1);

    int num = 0;
    for (int i = 0; i < bytes.n; i++){
        num += ((int) bytes.values[i] << (i * 8));
    }

    if (isNegative){
        num *= (-1);
    }

    return num;
}


// **************************************************************************** Bytes_FromFloat

// Turn a float to 4 bytes
Bytes Bytes_FromFloat(float num){
    // The first 3 bytes represent a signed integer. The last byte represents 
    // the exponent

    const int DIGITS_N = 6;

    if (FEQ(num, 0.0f)) {return BYTES_ZERO;}

    bool isNegative = (num < 0.0f);
    num = ABS(num);

    int exp = 0;
    while (num < 0.1f) {num *= 10.0f; exp--;}
    while (num >= 1.0f) {num /= 10.0f; exp++;}

    num *= Math_PowI(10, DIGITS_N);
    exp -= DIGITS_N;

    if (!IS_IN_RANGE(exp, -FLOAT_MAX_EXP, FLOAT_MAX_EXP - 1)){
        return BYTES_NULL;
    }

    num *= (isNegative ? (-1.0f) : 1.0f);

    int intPart = Math_Round(num);
    Bytes bytes = Bytes_FromInt(intPart, 3);

    bytes.n = 4;
    bytes.values[3] = exp + FLOAT_MAX_EXP;

    return bytes;
}


// **************************************************************************** Bytes_ToFloat

// Turn the bytes to a float. Return INVALID if invalid
float Bytes_ToFloat(Bytes bytes){
    if (bytes.n != 4) {return INVALID;}

    bytes.n = 3;
    float res = (float) Bytes_ToInt(bytes);

    res *= Math_PowF(10.0f, (int) bytes.values[3] - FLOAT_MAX_EXP);

    return res;
}


#ifdef DEBUG_MODE
// **************************************************************************** Bytes_Print

    // Single line print of the Bytes structure. Optionally with new line
    void Bytes_Print(Bytes bytes, bool withNewLine){
        printf("(%d: [", bytes.n);
        for (int i = 0; i < 4; i++){
            printf("%02X", bytes.values[i]);
            if (i < 3) {printf(", ");}
        }
        printf("])");

        if (withNewLine) {printf("\n");}
    }
#endif



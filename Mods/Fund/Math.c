// ============================================================================
// RODS
// Math
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Mathematical functions.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include <time.h>

#include "../Public/Public.h"
#include "Fund.h"


// ============================================================================ FUNC DEF

// **************************************************************************** Math_RandomInt

// Return a random integer between and including the limits
int Math_RandomInt(int minV, int maxV){
    static bool isSeeded = false;
    if (!isSeeded){
        srand(time(NULL));
        isSeeded = true;
    }

    int range = maxV - minV + 1;
    range = MAX(1, range);

    return rand() % range + minV;
}


// **************************************************************************** Math_RandomFloat

// Return a random float between and including the limits. The limits must be 
// larger than 0.001
float Math_RandomFloat(float minV, float maxV){
    const float MULTIPLIER = 1000;
    int iMinV = (int) (minV * MULTIPLIER);
    int iMaxV = (int) (maxV * MULTIPLIER);

    int iNum = Math_RandomInt(iMinV, iMaxV);

    return (float) iNum / MULTIPLIER;
}


// **************************************************************************** Math_FloatsAreEqual

// Return true if the two floats are equal, with the given precision
bool Math_FloatsAreEqual(float num1, float num2, float precision){
    if (!IS_IN_RANGE(num2, -10.0f, 10.0f)){
        precision = num2 * precision;
    }
    precision = ABS(precision);

    return IS_IN_RANGE(num1, num2 - precision, num2 + precision);
}


// **************************************************************************** Math_Sqrt

// The square root of the number. If the number is negative, return 0
float Math_Sqrt(float num){
    // Fast square root algorithm, with babylonian steps
    // https://www.codeproject.com/Articles/69941/Best-Square-Root-Method-Algorithm-Function-Precisi

    if (num <= 0.0f) {return 0.0f;}

    union{
        int i;
        float x;
    }u;

    u.x = num;
    u.i = (1 << 29) + (u.i >> 1) - (1 << 22);

    // Two babylonian steps, simplified from:
    // u.x = 0.5f * (u.x + num / u.x);
    // u.x = 0.5f * (u.x + num / u.x);
    u.x =         u.x + num / u.x;
    u.x = 0.25f * u.x + num / u.x;

    return u.x;
}


// **************************************************************************** Math_RightTriangleSide

// The vertical side of a right triangle, given the other vertical side and the 
// hypotenuse
float Math_RightTriangleSide(float side2, float hypotenuse){
    float res = Math_Sqrt(SQR(hypotenuse) - SQR(side2));
    return MIN(res, hypotenuse);
}


// **************************************************************************** Math_Hypotenuse

// The hypotenuse of a right triangle, given the two vertical sides
float Math_Hypotenuse(float side1, float side2){
    return Math_Sqrt(SQR(side1) + SQR(side2));
}


// **************************************************************************** Math_PowI

// The base raised to the exponent. Negative exponent returns 1
int Math_PowI(int base, int exp){
    int res = 1;

    for (; exp > 0; exp--){
        res *= base;
    }

    return res;
}


// **************************************************************************** Math_PowF

// The float base raised to the exponent. Negative exponent inverts the base
float Math_PowF(float base, int exp){
    base = (exp < 0) ? 1.0f / base : base;
    exp = ABS(exp);

    float res = 1.0f;

    for (; exp > 0; exp--){
        res *= base;
    }

    return res;
}


// **************************************************************************** Math_Factorial

// The factorial of the number. If the number is negative, return 1
int Math_Factorial(int num){
    int res = 1;

    for (; num > 0; num--){
        res *= num;
    }

    return res;
}


// **************************************************************************** Math_Round

// Round the number to the nearest integer
int Math_Round(float num){
    return (num < 0) ? num - 0.5f : num + 0.5f;
}


// **************************************************************************** Math_RadToDeg

// Turn radians to degrees
float Math_RadToDeg(float radians){
    return (radians / MATH_PI) * 180.0f;
}


// **************************************************************************** Math_DegToRad

// Turn degrees to radians
float Math_DegToRad(float degrees){
    return (degrees / 180.0f) * MATH_PI; 
}


// **************************************************************************** Math_PrincipalAngle

// The equivalent angle in the range [0, 2π). Angle is in radians
float Math_PrincipalAngle(float angle){
    while (angle < 0) {angle += MATH_PI_DOUBLE;}
    while (angle >= MATH_PI_DOUBLE) {angle -= MATH_PI_DOUBLE;}

    return angle;
}


// **************************************************************************** Math_AngleQuarter

// The circle quarter the angle is inside of (1-4). Angle is in radians
int Math_AngleQuarter(float angle){
    angle = Math_PrincipalAngle(angle);

    return (int) (angle / MATH_PI_HALF) + 1;
}


// **************************************************************************** Math_Sin

// The sine of the angle. Angle is in radians
float Math_Sin(float angle){
    // Use the Taylor series for sin(x):
    // sin(x) = x - x^3/3! + x^5/5! - x^7/7! + ...

    // Bring the angle in the range [0, 2π)
    angle = Math_PrincipalAngle(angle);

    // Determine the sign of the angle
    float sign = (angle < MATH_PI) ? 1.0f : -1.0f;

    // Bring the angle in the range [0, π)
    angle = (angle >= MATH_PI) ? angle - MATH_PI : angle;

    // Bring the angle in the range [0, π/2]
    angle = (angle > MATH_PI_HALF) ? MATH_PI - angle : angle;

    // If the angle is larger than the threshold, calculate the sine of half 
    // the angle recursively and use the double angle sine formula: 
    // sin(2x) = 2 * sin(x) * cos(x)
    // (The Taylor series is accurate near zero)
    const float THRESHOLD = 0.8f;
    if (angle > THRESHOLD){
        float s = Math_Sin(angle * 0.5f);
        float c = Math_RightTriangleSide(s, 1.0f);
        return sign * 2.0f * s * c;
    }

    // The number of terms in the Taylor series
    const int TERMS_N = 4;

    // Apply the Taylor series
    float res = angle;
    float termSign = -1.0f;
    for (int i = 1; i < TERMS_N; i++){
        int exp = (2 * i) + 1;
        float term = Math_PowF(angle, exp) / (float) Math_Factorial(exp);
        term *= termSign;
        res += term;
        termSign *= (-1.0f);
    }

    // Apply the sign
    res *= sign;

    // Return the result
    return res;
}


// **************************************************************************** Math_Cos

// The cosine of the angle. Angle is in radians
float Math_Cos(float angle){
    float s = Math_Sin(angle);
    float res = Math_RightTriangleSide(s, 1.0f);
    int quarter = Math_AngleQuarter(angle);
    float sign = ((quarter == 1) || (quarter == 4)) ? 1.0f : -1.0f;
    return sign * res;
}


// **************************************************************************** Math_Tan

// The tangent of the angle. Angle is in radians
float Math_Tan(float angle){
    float s = Math_Sin(angle);
    s = ABS(s);
    float c = Math_RightTriangleSide(s, 1.0f);
    int quarter = Math_AngleQuarter(angle);
    float sign = ((quarter == 1) || (quarter == 3)) ? 1.0f : -1.0f;
    return sign * (s / c);
}




// ============================================================================
// RODS
// Color
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions for manipulating color and managing the Magic Color.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"
#include "../Fund/Fund.h"
#include "Graph.h"


// ============================================================================ PRIVATE CONSTANTS



// ============================================================================ PRIVATE STRUCTURES

// **************************************************************************** ColVal

// A color represented both as Color and as an array of 4 bytes for the RGBA 
// values
typedef union ColVal{
    Color color;

    unsigned char values[4];
}ColVal;


// ============================================================================ OPAQUE STRUCTURES






// ============================================================================ FUNC DEF

// ---------------------------------------------------------------------------- Color Functions

// **************************************************************************** Color_Random

// A random color with RGB values within and including the limits. Alpha is set 
// at 0xFF
Color Color_Random(unsigned char minV, unsigned char maxV){
    ColVal res;
    for (int i = 0; i < 3; i++){
        res.values[i] = Math_RandomInt(minV, maxV);
    }
    res.color.a = 0xFF;

    return res.color;
}


// **************************************************************************** Color_Blend

// Blend the two colors, f1 and f2 being the relative weights
Color Color_Blend(Color color1, int f1, Color color2, int f2){
    f1 = ABS(f1);
    f2 = ABS(f2);

    int sum = f1 + f2;
    if (sum == 0){
        return COL_NULL;
    }

    ColVal res, c2;
    res.color = color1;
    c2.color = color2;

    for (int i = 0; i < 4; i++){
        int temp = (int) res.values[i] * f1 + (int) c2.values[i] * f2;
        res.values[i] = (unsigned char) (temp / sum);
    }

    return res.color;
}


// **************************************************************************** Color_ChangeBrightness

// Change the brightness of the color. Positive brightness makes the color 
// brighter, negative makes it darker
Color Color_ChangeBrightness(Color color, int brightness){
    // The bigger the inertia, the more the color remains unchanged
    const int INERTIA = 7;

    Color blendColor = (brightness >= 0) ? WHITE : BLACK;

    return Color_Blend(color, INERTIA, blendColor, brightness);
}


// **************************************************************************** Color_SetAlpha

// Change the alpha value of the color
Color Color_SetAlpha(Color color, unsigned char alpha){
    color.a = alpha;
    return color;
}


#ifdef DEBUG_MODE
// **************************************************************************** Color_Print

    // Print the color in the form (R,G,B,A). Optionally with new line
    void Color_Print(Color color, bool withNewLine){
        printf("(%02X, %02X, %02X, %02X)", color.r, color.g, color.b, color.a);

        if (withNewLine) {printf("\n");}
    }
#endif



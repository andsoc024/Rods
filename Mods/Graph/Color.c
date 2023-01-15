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

#define MCOL_SPECTRUM_N                     ((FPS * 4) / 5)
#define MCOL_PAUSE_N                        ((FPS * 7) / 5)
#define COL_MCOL_DEF_1                      COL_ELECTRIC_1
#define COL_MCOL_DEF_2                      COL_ELECTRIC_2


// ============================================================================ PRIVATE STRUCTURES

// **************************************************************************** ColVal

// A color represented both as Color and as an array of 4 bytes for the RGBA 
// values
typedef union ColVal{
    Color color;

    unsigned char values[4];
}ColVal;


// ============================================================================ OPAQUE STRUCTURES

// **************************************************************************** MagicColor

// The magic colors changes gradually from the first to the last color in its 
// spectrum. Then back to the first and then it pauses. It repeats with set 
// period.
struct MagicColor{
    Color spectrum[MCOL_SPECTRUM_N];

    int index;
    int increment;

    int pauseCount;
};






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






// ---------------------------------------------------------------------------- Magic Color Functions

// **************************************************************************** MCol_Make

// Make a magic color with color1 and color2 as the first and last color in its 
// spectrum
MagicColor* MCol_Make(Color color1, Color color2){
    MagicColor* mcol = Memory_Allocate(NULL, sizeof(MagicColor), ZEROVAL_ALL);

    for (int i = 0; i < MCOL_SPECTRUM_N; i++){
        mcol->spectrum[i] = Color_Blend(color1, MCOL_SPECTRUM_N - 1 - i, color2, i);
    }

    return mcol;
}


// **************************************************************************** MCol_Free

// Free the memory of the magic color. Return NULL
MagicColor* MCol_Free(MagicColor* mcol){
    return Memory_Free(mcol);
}


// **************************************************************************** MCol_Update

// Update the magic color
void MCol_Update(MagicColor* mcol){
    // The magic color can be in one of three states:
    // 1) Paused (increment == 0)
    // 2) Ascending (increment == 1)
    // 3) Descending (increment == -1)

    mcol->index += mcol->increment;

    switch (mcol->increment){
        // Paused -> Ascending
        case 0:{
            mcol->pauseCount++;
            if (mcol->pauseCount >= MCOL_PAUSE_N){
                mcol->pauseCount = 0;
                mcol->increment = 1;
                break;
            }
            break;
        }

        // Ascending -> Descending
        case 1:{
            if (mcol->index >= MCOL_SPECTRUM_N){
                mcol->index = MCOL_SPECTRUM_N - 1;
                mcol->increment = -1;
            }
            break;
        }

        // Descending -> Paused
        case -1:{
            if (mcol->index < 0){
                mcol->index = 0;
                mcol->increment = 0;
                mcol->pauseCount = 0;
            }
            break;
        }

        default: {break;}
    }
}


// **************************************************************************** MCol_Reset

// Reset the magic color by setting it in the paused state, at the first color 
// of the spectrum
void MCol_Reset(MagicColor* mcol){
    mcol->index = 0;
    mcol->increment = 0;
    mcol->pauseCount = 0;
}


// **************************************************************************** MCol

// Return the current color of the spectrum
Color MCol(MagicColor* mcol){
    return mcol->spectrum[mcol->index];
}


// **************************************************************************** MCol_MakeDefault

// Make the default Magic Color at Glo_MCol
void MCol_MakeDefault(void){
    MCol_FreeDefault();
    Glo_MCol = MCol_Make(COL_MCOL_DEF_1, COL_MCOL_DEF_2);
}


// **************************************************************************** MCol_FreeDefault

// Free the memory of the default magic color at Glo_MCol
void MCol_FreeDefault(void){
    Glo_MCol = MCol_Free(Glo_MCol);
}


#ifdef DEBUG_MODE
// **************************************************************************** MCol_Print

    // Multiline print of the parameters of the Magic Color
    void MCol_Print(MagicColor* mcol){
        CHECK_NULL(mcol, WITH_NEW_LINE)
        
        printf("Spectrum: ");
        
        const int COLORS_PER_ROW_N = 3;
        for (int i = 0; i < MCOL_SPECTRUM_N; i++){
            if (i > 0 && i % COLORS_PER_ROW_N == 0){
                printf("\n          ");
            }
            Color_Print(mcol->spectrum[i], WITHOUT_NEW_LINE);
            printf("   ");
        }
        printf("\n");

        printf("Index:       %d\n", mcol->index);
        printf("Increment:   %d\n", mcol->increment);
        printf("Pause Count: %d\n", mcol->pauseCount);
    }
#endif





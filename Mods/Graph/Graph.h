// ============================================================================
// RODS
// Graphics Header
// by Andreas Socratous
// Jan 2023
// ============================================================================


#ifndef GRAPH_GUARD
#define GRAPH_GUARD


// ============================================================================ INFO
/*
    Functions for manipulating color, managing the Magic Color and the Flying 
    Rectangles, calculating and drawing text with the default custom font, 
    drawing shapes on screen and in images, making and drawing the textures as 
    well as calculating and drawing the Timer Display.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"


// ============================================================================ FUNC DECL

// ---------------------------------------------------------------------------- Color Functions

Color           Color_Random(unsigned char minV, unsigned char maxV);
Color           Color_Blend(Color color1, int f1, Color color2, int f2);
Color           Color_ChangeBrightness(Color color, int brightness);
Color           Color_SetAlpha(Color color, unsigned char alpha);
#ifdef DEBUG_MODE
    void        Color_Print(Color color, bool withNewLine);
#endif

// ---------------------------------------------------------------------------- Magic Color Functions

MagicColor*     MCol_Make(Color color1, Color color2);
MagicColor*     MCol_Free(MagicColor* mcol);
void            MCol_Update(MagicColor* mcol);
void            MCol_Reset(MagicColor* mcol);
Color           MCol(MagicColor* mcol);
void            MCol_MakeDefault(void);
void            MCol_FreeDefault(void);
#ifdef DEBUG_MODE
    void        MCol_Print(MagicColor* mcol);
#endif





#endif // GRAPH_GUARD


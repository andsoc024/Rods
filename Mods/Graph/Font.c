// ============================================================================
// RODS
// Font
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions for calculating and drawing text with the default custom font at 
    Glo_Font.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Assets/Assets.h"

#include "../Public/Public.h"
#include "../Fund/Fund.h"
#include "Graph.h"


// ============================================================================ PRIVATE CONSTANTS

#define FONT_FIRST_GLYPH                    32
#define FONT_LAST_GLYPH                     127
#define FONT_GLYPHS_N                       (FONT_LAST_GLYPH - FONT_FIRST_GLYPH + 1)






// ============================================================================ FUNC DEF

// **************************************************************************** Font_LoadDefault

// Load the default custom font at the global Glo_Font
void Font_LoadDefault(void){
    int* glyphs = Memory_Allocate(NULL, sizeof(int) * FONT_GLYPHS_N, ZEROVAL_ALL);

    for (int i = 0; i < FONT_GLYPHS_N; i++){
        glyphs[i] = FONT_FIRST_GLYPH + i;
    }

    Glo_Font = LoadFontFromMemory(".ttf", Asset_Font_HeyComic, Asset_Font_HeyComic_Size, 
                                  FONT_DEF_SIZE, glyphs, FONT_GLYPHS_N);

    glyphs = Memory_Free(glyphs);

    Err_Assert(Glo_Font.baseSize > 0, "Failed to load the font");
}


// **************************************************************************** Font_UnloadDefault

// Unload the default custom font at the flobal Glo_Font
void Font_UnloadDefault(void){
    UnloadFont(Glo_Font);
    Glo_Font = (Font) {0};
}


// **************************************************************************** Font_CalcTextSize

// Calculate the size of the text, rendered with the default custom font and 
// the given font size
Size Font_CalcTextSize(const char* txt, float fontSize){
    Vector2 vectorSize = MeasureTextEx(Glo_Font, txt, fontSize, FONT_SPACING);
    return SIZE(vectorSize.x, vectorSize.y);
}


// **************************************************************************** Font_CalcTextRect

// Calculate the rectangle containing the text, rendered with the default 
// custom font, with the given font size and at the given position
Rect Font_CalcTextRect(const char* txt, float fontSize, Point pos, E_RectPointType pointType){
    Size size = Font_CalcTextSize(txt, fontSize);
    return Geo_SetRectPS(pos, size, pointType);
}


// **************************************************************************** Font_CalcTextPos

// The top left corner of the rectangle containing the text, rendered with the 
// default custom font, with the given font size and at the given position
Point Font_CalcTextPos(const char* txt, float fontSize, Point pos, E_RectPointType pointType){
    Rect rect = Font_CalcTextRect(txt, fontSize, pos, pointType);
    return RORIGIN(rect);
}


// **************************************************************************** Font_FitTextInSize

// The maximum font size, for which the text, rendered with the default custom 
// font, fits inside the containing size
float Font_FitTextInSize(const char* txt, Size cSize){
    float fontSize = cSize.height;

    while (Font_CalcTextSize(txt, fontSize).width > cSize.width){
        fontSize -= 1.0f;
        if (fontSize < 1.0f){
            break;
        }
    }

    return fontSize;
}


// **************************************************************************** Font_DrawText

// Draw the text, rendered with the default custom font
void Font_DrawText(const char* txt, float fontSize, Point pos, Color color){
    DrawTextEx(Glo_Font, txt, pos, fontSize, FONT_SPACING, color);
}




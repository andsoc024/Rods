// ============================================================================
// RODS
// Public
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Definitions of enumeration functions, constant arrays and initialization of 
    globals.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "Public.h"


// ============================================================================ ENUMERATION FUNCTIONS

#include "EnumFunc.c"


// ============================================================================ CONSTANTS

const KeyboardKey WATCHED_KEYS[] = {
    WKEY_RIGHT, WKEY_DOWN,  WKEY_LEFT, WKEY_UP,
    WKEY_D,     WKEY_S,     WKEY_A,    WKEY_W,
    WKEY_SPACE, WKEY_ENTER, WKEY_TAB,  WKEY_M,
    WKEY_T,     WKEY_R,     WKEY_PLUS, WKEY_MINUS,
    WKEY_Z,     WKEY_X
};

const int WATCHED_KEYS_N = sizeof(WATCHED_KEYS) / sizeof(WATCHED_KEYS[0]);


// ============================================================================ GLOBALS

// The window size
Size Glo_WinSize = {0};

// Structure for holdeing the minimum record times
Records* Glo_Records = 0;

// The default magic color
MagicColor* Glo_MCol = 0;

// The dafault custom font
Font Glo_Font = {0};

// Structure for storing the icon, rod and selection box textures
Textures Glo_Textures = {0};

// The data File Path
char* Glo_FilePath = 0;

// Sound and music data
SoundData* Glo_SoundData = 0;

// Counters for memory tracking
#ifdef MEMORY_TRACK
    int Glo_AllocCount = 0;
    int Glo_FreeCount = 0;
#endif

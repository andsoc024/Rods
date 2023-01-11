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






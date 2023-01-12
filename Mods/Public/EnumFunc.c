// ============================================================================ 
// RODS
// Enumeration Functions
// by Andreas Socratous
// Jan 2023
// ============================================================================ 


// ============================================================================ INFO
/*
    Definitions of enumeration functions.

    IsValid functions return true if the value is a valid enumeration value.

    ToString functions return a string describing the enumeration value.
*/


// ============================================================================ DEPENDENCIES

// Included in Public.c

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "Public.h"


// ============================================================================  ENUMERATION FUNCTIONS

// **************************************************************************** bool

#ifdef DEBUG_MODE
    const char* Bool_ToString(bool value, bool form){
        if (form == SHORT_FORM){
            return value ? "T" : "F";
        }

        return value ? "True" : "False";
    }
#endif


// **************************************************************************** KeyboardKey

// Return true if the key is in the WATCHED_KEYS array
bool KeyboardKey_IsValid(KeyboardKey key){
    for (int i = 0; i < WATCHED_KEYS_N; i++){
        if (key == WATCHED_KEYS[i]){
            return true;
        }
    }

    return false;
}

#ifdef DEBUG_MODE
    // Return a string describing the keyboard key
    const char* KeyboardKey_ToString(KeyboardKey key){
        static const char* names[] = {
            "Right", "Down",  "Left", "Up",
            "D",     "S",     "A",    "W",
            "Space", "Enter", "Tab",  "M",
            "T",     "R",     "Plus", "Minus",
            "Z",     "X"
        };

        for (int i = 0; i < WATCHED_KEYS_N; i++){
            if (key == WATCHED_KEYS[i]){
                return names[i];
            }
        }

        return STR_INVALID;
    }

    // Print the code and name of the pressed keys in the terminal
    void KeyboardKey_Test(void){
        InitWindow(100, 100, "Keyboard Test");
        SetTargetFPS(60);

        while (!WindowShouldClose()){
            KeyboardKey key = GetKeyPressed();
            if (key != KEY_NULL){
                printf("%d\t%s\n", key, KeyboardKey_ToString(key));
            }

            BeginDrawing();
            ClearBackground(BLACK);
            EndDrawing();
        }

        CloseWindow();
    }
#endif


// **************************************************************************** E_ZeroValMode

// Return true if the value is a valid zero values mode
bool ZeroValMode_IsValid(int mode){
    return IS_IN_RANGE(mode, 0, ZEROVAL_MODES_N - 1);
}

#ifdef DEBUG_MODE
    // Return a string describing the zero values mode
    const char* ZeroValMode_ToString(int mode){
        static const char* names[] = {"None", "All", "Last"};

        CHECK_INVALID(ZeroValMode_IsValid(mode), LONG_FORM);

        return names[mode];
    }
#endif


// **************************************************************************** E_Direction

// Return true if the value is a valid direction, optionally including DIR_NONE
bool Direction_IsValid(int dir, bool includeNone){
    int startDir = includeNone ? DIR_NONE : DIR_RIGHT;
    return IS_IN_RANGE(dir, startDir, DIR_UP);
}

#ifdef DEBUG_MODE
    // Return a string describing the direction, in short or long form
    const char* Direction_ToString(int dir, bool form){
        const char* names[] = {"None", "Right", "Down", "Left", "Up"};
        const char* shortNames[] = {"N", "R", "D", "L", "U"};

        CHECK_INVALID(Direction_IsValid(dir, INCLUDE_NONE), form)

        return (form == SHORT_FORM) ? shortNames[dir] : names[dir];
    }
#endif


// **************************************************************************** E_Orientation

// Return true if the value is a valid orientation value
bool Orientation_IsValid(int orientation){
    return IS_IN_RANGE(orientation, 0, ORIENTATIONS_N - 1);
}

#ifdef DEBUG_MODE
    // Return a string describing the orientation
    const char* Orientation_ToString(int orientation){
        const char* names[] = {"Horisontal", "Vertical"};

        CHECK_INVALID(Orientation_IsValid(orientation), LONG_FORM)

        return names[orientation];
    }
#endif



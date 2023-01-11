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




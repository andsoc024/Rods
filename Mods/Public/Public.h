// ============================================================================
// RODS
// Public Header
// by Andreas Socratous
// Jan 2023
// ============================================================================


#ifndef PUBLIC_GUARD
#define PUBLIC_GUARD


// ============================================================================ INFO
/*
    Enumerations, constants, macros, structures and globals, used throughout 
    the program.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../../CompSettings.h"


// ============================================================================ MACROS

#include "Macro/Macro_Fund.h"


// ============================================================================ PLATFORM

#ifdef PLATFORM_WINDOWS
    #include "../../Arch/Windows/Windows.h"
#endif
#ifdef PLATFORM_MACOS
    #include "../../Arch/MacOS/MacOS.h"
#endif
#ifdef PLATFORM_LINUX
    #include "../../Arch/Linux/Linux.h"
#endif


// ============================================================================ APP INFO

#define APP_TITLE                           "Rods"
#define APP_VERSION                         RODS_VERSION
#define APP_AUTHOR                          "Andreas Socratous"
#define APP_COMPANY                         "Zenon"
#define APP_YEAR                            "2023"
#define APP_PLATFORM                        ARCH_DEF(APP_PLATFORM)


// ============================================================================ ENUMERATIONS

#include "Enum.h"


// ============================================================================ CONSTANTS

#define INVALID                             (-1)
#define IGNORE                              INVALID

// Strings
#define STR_NULL                            "(Null)"
#define STR_NONE                            "(None)"
#define STR_EMPTY                           "(Empty)"
#define STR_INVALID                         "(Invalid)"
#define STR_INVALID_SHORT                   "-"
#define STR_LINE                            "----------------------------------------" \
                                            "----------------------------------------"


// ============================================================================ COLORS

#include "Colors.h"


// ============================================================================ STRUCTURES



// ============================================================================ FLAGS & INDICES

// Short or Long Form
#define SHORT_FORM                          true
#define LONG_FORM                           false


// ============================================================================ DEF STRUCTURES



// ============================================================================ GLOBALS





#endif // PUBLIC_GUARD


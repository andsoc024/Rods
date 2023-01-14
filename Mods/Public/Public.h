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

#define UNUSED                              ARCH_DEF(UNUSED)


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

// Math Constants
#define MATH_PI                             3.14159f
#define MATH_PI_HALF                        (MATH_PI * 0.5f)
#define MATH_PI_THREE_HALVES                (MATH_PI * 1.5f)
#define MATH_PI_DOUBLE                      (MATH_PI * 2.0f)
#define MATH_PHI                            1.61803
#define MATH_PHI_INVERSE                    (1.0f / MATH_PHI)
#define MATH_FLOAT_PRECISION                0.001f

// Time Constants
#define TIME_MAX_HOURS                      8
#define TIME_MAX_SECS                       (TIME_MAX_HOURS * 60 * 60 + 59 * 60 + 59)
#define TIME_INVALID_CHAR                   '-'
#define TIME_INVALID_CHAR_STR               ((char[]) {TIME_INVALID_CHAR, '\0'})
#define TIME_SEP_CHAR                       ':'
#define TIME_SEP_CHAR_STR                   ((char[]) {TIME_SEP_CHAR, '\0'})
#define TIME_INVALID_STR                    ((char[]) {TIME_INVALID_CHAR, TIME_INVALID_CHAR, TIME_SEP_CHAR, \
                                                       TIME_INVALID_CHAR, TIME_INVALID_CHAR, '\0'})


// ============================================================================ COLORS

#include "Colors.h"


// ============================================================================ STRUCTURES

#include "Struct/Struct_Fund.h"


// ============================================================================ FLAGS & INDICES

// Short or Long Form
#define SHORT_FORM                          true
#define LONG_FORM                           false

// For use in print functions
#define WITH_NEW_LINE                       true
#define WITHOUT_NEW_LINE                    false

// For use in direction functions
#define INCLUDE_NONE                        true
#define INCLUDE_NOT_NONE                    false

// Binary leg directions
#define LEGDIR_NONE                         0
#define LEGDIR_RIGHT                        (1 << 0)
#define LEGDIR_DOWN                         (1 << 1)
#define LEGDIR_LEFT                         (1 << 2)
#define LEGDIR_UP                           (1 << 3)

// Time Units
#define HOURS                               0
#define MINUTES                             1
#define SECONDS                             2


// ============================================================================ DEF STRUCTURES

#define BYTES_NULL                          BYTES(0, 0, 0, 0, 0)
#define BYTES_ZERO                          BYTES(4, 0, 0, 0, 0)

#define POINT_NULL                          POINT(0, 0)
#define VECTOR_NULL                         VECTOR(0, 0)
#define SIZE_NULL                           SIZE(0, 0)
#define RECT_NULL                           RECT(0, 0, 0, 0)

#define GNODE_NULL                          GNODE(0, 0)
#define GNODE_INVALID                       GNODE(INVALID, INVALID)
#define GRID_NULL                           GRID0(0, 0)

#define TIME_NULL                           TIME(0, 0, 0)
#define TIME_INVALID                        TIME(INVALID, INVALID, INVALID)
#define TIME_MAX                            TIME(TIME_MAX_HOURS, 59, 59)


// ============================================================================ GLOBALS





#endif // PUBLIC_GUARD


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

#include "Macro.h"


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
#define STR_DEF_LENGTH                      200

// Math Constants
#define MATH_PI                             3.14159f
#define MATH_PI_HALF                        (MATH_PI * 0.5f)
#define MATH_PI_THREE_HALVES                (MATH_PI * 1.5f)
#define MATH_PI_DOUBLE                      (MATH_PI * 2.0f)
#define MATH_PHI                            1.61803
#define MATH_PHI_INVERSE                    (1.0f / MATH_PHI)
#define MATH_FLOAT_PRECISION                0.001f

// Window Constants
#define FPS                                 60

// Time Constants
#define TIME_MAX_HOURS                      8
#define TIME_MAX_SECS                       (TIME_MAX_HOURS * 60 * 60 + 59 * 60 + 59)
#define TIME_INVALID_CHAR                   '-'
#define TIME_INVALID_CHAR_STR               ((char[]) {TIME_INVALID_CHAR, '\0'})
#define TIME_SEP_CHAR                       ':'
#define TIME_SEP_CHAR_STR                   ((char[]) {TIME_SEP_CHAR, '\0'})
#define TIME_INVALID_STR                    ((char[]) {TIME_INVALID_CHAR, TIME_INVALID_CHAR, TIME_SEP_CHAR, \
                                                       TIME_INVALID_CHAR, TIME_INVALID_CHAR, '\0'})

// Rod Constants
#define ROD_ROT_FRAMES_N                    ((FPS * 1) / 3)

#define RGRID_MIN_SIZE                      3
#define RGRID_MAX_SIZE                      100
#define RGRID_DEF_SIZE                      10

#define ROD_DEF_TEXTURE_SIZE                300.0f
#define ROD_MIN_TEXTURE_SIZE                20.0f
#define ROD_MAX_TEXTURE_SIZE                350.0f

// Font Constants
#define FONT_DEF_SIZE                       300.0f
#define FONT_SPACING                        1.0f

// Shape Constants
#define RRECT_DEF_ROUNDNESS                 0.27f

// Scroll Graphics Constants
#define SGRAPH_DEF_MARGIN                   30.0f

// Icon Constants
#define ICON_DEF_TEXTURE_SIZE               200.0f

// GUI Constants
#define TURBO_COUNT                         ((FPS * 1) / 1)


// ============================================================================ COLORS

#include "Colors.h"


// ============================================================================ STRUCTURES

#include "Struct/Struct_Fund.h"
#include "Struct/Struct_GUI.h"


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

// For use in functions that involve animation
#define WITH_ANIM                           true
#define WITHOUT_ANIM                        false

// Result of rod functions
#define COMPLETED                           true
#define NOT_COMPLETED                       false

// For use with gadget functions
#define IS_SELECTABLE                       true
#define IS_NOT_SELECTABLE                   false


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

#define ROD_NULL                            ((Rod) {.legs = 0, .isElectrified = false, .frame = 0})

#define EVENT_NULL                          ((Event) {.id = EVENT_NONE, .source = GDG_NONE, .target = GDG_NONE, .data = {0}})


// ============================================================================ GLOBALS

// The window size
extern Size Glo_WinSize;

// Structure for holdeing the minimum record times
extern Records* Glo_Records;

// The default Magic Color
extern MagicColor* Glo_MCol;

// The default custom font
extern Font Glo_Font;

// Structure for storing the icon, rod and selection box textures
extern Textures Glo_Textures;

// The data file path
extern char* Glo_FilePath;

// Counters for memory tracking
#ifdef MEMORY_TRACK
    extern int Glo_AllocCount;
    extern int Glo_FreeCount;
#endif



#endif // PUBLIC_GUARD


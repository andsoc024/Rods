// ============================================================================ 
// RODS
// Enumerations Header
// by Andreas Socratous
// Jan 2023
// ============================================================================ 


#ifndef ENUM_GUARD
#define ENUM_GUARD


// ============================================================================ INFO
/*
    Enumerations and declarations of enumeration functions.
*/


// ============================================================================ DEPENDENCIES

// Included in Public.h


// ============================================================================ ENUMERATIONS

// **************************************************************************** bool

#ifdef DEBUG_MODE
    const char* Bool_ToString(bool value, bool form);
#endif


// **************************************************************************** KeyboardKey

// Keyboard Keys (Watched Keys)
#define WKEY_RIGHT                          ARCH_DEF(WKEY_RIGHT)
#define WKEY_DOWN                           ARCH_DEF(WKEY_DOWN)
#define WKEY_LEFT                           ARCH_DEF(WKEY_LEFT)
#define WKEY_UP                             ARCH_DEF(WKEY_UP)
#define WKEY_D                              ARCH_DEF(WKEY_D)
#define WKEY_S                              ARCH_DEF(WKEY_S)
#define WKEY_A                              ARCH_DEF(WKEY_A)
#define WKEY_W                              ARCH_DEF(WKEY_W)
#define WKEY_SPACE                          ARCH_DEF(WKEY_SPACE)
#define WKEY_ENTER                          ARCH_DEF(WKEY_ENTER)
#define WKEY_TAB                            ARCH_DEF(WKEY_TAB)
#define WKEY_M                              ARCH_DEF(WKEY_M)
#define WKEY_T                              ARCH_DEF(WKEY_T)
#define WKEY_R                              ARCH_DEF(WKEY_R)
#define WKEY_PLUS                           ARCH_DEF(WKEY_PLUS)
#define WKEY_MINUS                          ARCH_DEF(WKEY_MINUS)
#define WKEY_Z                              ARCH_DEF(WKEY_Z)
#define WKEY_X                              ARCH_DEF(WKEY_X)

extern const KeyboardKey WATCHED_KEYS[];
extern const int WATCHED_KEYS_N;

bool           KeyboardKey_IsValid(KeyboardKey key);
#ifdef DEBUG_MODE
    const char* KeyboardKey_ToString(KeyboardKey key);
    void        KeyboardKey_Test(void);
#endif


// **************************************************************************** E_ZeroValMode

// Zero Values Mode. Determines which, if any, values of a memory block to be 
// set to 0
typedef enum E_ZeroValMode{
    ZEROVAL_NONE,
    ZEROVAL_ALL,
    ZEROVAL_LAST
}E_ZeroValMode;
#define ZEROVAL_MODES_N                     (ZEROVAL_LAST + 1)

bool            ZeroValMode_IsValid(int mode);
#ifdef DEBUG_MODE
    const char* ZeroValMode_ToString(int mode);
#endif


// **************************************************************************** E_Direction

// Direction enumeration: None, Right, Down, Left, Up
typedef enum E_Direction{
    DIR_NONE,
    DIR_RIGHT,
    DIR_DOWN,
    DIR_LEFT,
    DIR_UP
}E_Direction;
#define DIRECTIONS_N                        (DIR_UP + 1)

bool            Direction_IsValid(int dir, bool includeNone);
#ifdef DEBUG_MODE
    const char* Direction_ToString(int dir, bool form);
#endif


// **************************************************************************** E_Orientation

// Orientation enumeration: Horisontal or Vertical
typedef enum E_Orientation{
    OR_HORISONTAL,
    OR_VERTICAL
}E_Orientation;
#define ORIENTATIONS_N                      (OR_VERTICAL + 1)

bool            Orientation_IsValid(int orientation);
#ifdef DEBUG_MODE
    const char* Orientation_ToString(int orientation);
#endif


// **************************************************************************** E_RectPointType

// Rectangle Point Type
typedef enum E_RectPointType{
    RP_TOP_LEFT,    RP_TOP_CENTER,    RP_TOP_RIGHT,
    RP_MIDDLE_LEFT, RP_CENTER,        RP_MIDDLE_RIGHT,
    RP_BOTTOM_LEFT, RP_BOTTOM_CENTER, RP_BOTTOM_RIGHT
}E_RectPointType;
#define RECT_POINT_TYPES_N                  (RP_BOTTOM_RIGHT + 1)

bool            RectPointType_IsValid(int pointType);
#ifdef DEBUG_MODE
    const char* RectPointType_ToString(int pointType);
#endif


// **************************************************************************** E_IconID

// Unique ID for each icon
typedef enum E_IconID{
    ICON_ARROW_RIGHT, ICON_ARROW_DOWN, ICON_ARROW_LEFT, ICON_ARROW_UP,
    ICON_BACK,        ICON_HELP,       ICON_INFO,       ICON_MEDAL,
    ICON_MUSIC,       ICON_RESTART,    ICON_ZOOM_IN,    ICON_ZOOM_OUT
}E_IconID;
#define ICONS_N                             (ICON_ZOOM_OUT + 1)

bool            IconID_IsValid(int id);
#ifdef DEBUG_MODE
    const char* IconID_ToString(int id);
#endif


#endif // ENUM_GUARD


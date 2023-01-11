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


#endif // ENUM_GUARD


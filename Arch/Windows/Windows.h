// ============================================================================
// RODS
// Windows
// by Andreas Socratous
// Jan 2023
// ============================================================================


#ifndef WINDOWS_GUARD
#define WINDOWS_GUARD


// ============================================================================ INFO
/*
    Constants specific for the Windows platform.
*/


// ============================================================================ DEPENDENCIES

// Included in Public.h when PLATFORM_WINDOWS is defined

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>


// ============================================================================ CONSTANTS

#define ARCH                                WIN
#define APP_PLATFORM_WIN                    "Windows"

#define UNUSED_WIN

// Keyboard Keys (Watched Keys)
#define WKEY_RIGHT_WIN                      KEY_RIGHT
#define WKEY_DOWN_WIN                       KEY_DOWN
#define WKEY_LEFT_WIN                       KEY_LEFT
#define WKEY_UP_WIN                         KEY_UP
#define WKEY_D_WIN                          KEY_D
#define WKEY_S_WIN                          KEY_S
#define WKEY_A_WIN                          KEY_A
#define WKEY_W_WIN                          KEY_W
#define WKEY_SPACE_WIN                      KEY_SPACE
#define WKEY_ENTER_WIN                      KEY_ENTER
#define WKEY_TAB_WIN                        KEY_TAB
#define WKEY_M_WIN                          KEY_M
#define WKEY_T_WIN                          KEY_T
#define WKEY_R_WIN                          KEY_R
#define WKEY_PLUS_WIN                       KEY_KP_ADD
#define WKEY_MINUS_WIN                      KEY_KP_SUBTRACT
#define WKEY_Z_WIN                          KEY_Z
#define WKEY_X_WIN                          KEY_X

#define MOUSE_WHEEL_SENITIVITY_WIN          0.05f

#endif // WINDOWS_GUARD


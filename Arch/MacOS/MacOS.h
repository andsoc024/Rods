// ============================================================================
// RODS
// MacOS
// by Andreas Socratous
// Jan 2023
// ============================================================================


#ifndef MACOS_GUARD
#define MACOS_GUARD


// ============================================================================ INFO
/*
    Constants specific for the MacOS platform.
*/


// ============================================================================ DEPENDENCIES

// Included in Public.h when PLATFORM_MACOS is defined

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>


// ============================================================================ CONSTANTS

#define ARCH                                MAC
#define APP_PLATFORM_MAC                    "MacOS"

#define UNUSED_MAC                          __attribute__((unused))

// Keyboard Keys (Watched Keys)
#define WKEY_RIGHT_MAC                      KEY_RIGHT
#define WKEY_DOWN_MAC                       KEY_DOWN
#define WKEY_LEFT_MAC                       KEY_LEFT
#define WKEY_UP_MAC                         KEY_UP
#define WKEY_D_MAC                          KEY_D
#define WKEY_S_MAC                          KEY_S
#define WKEY_A_MAC                          KEY_A
#define WKEY_W_MAC                          KEY_W
#define WKEY_SPACE_MAC                      KEY_SPACE
#define WKEY_ENTER_MAC                      KEY_ENTER
#define WKEY_TAB_MAC                        KEY_TAB
#define WKEY_M_MAC                          KEY_M
#define WKEY_T_MAC                          KEY_T
#define WKEY_R_MAC                          KEY_R
#define WKEY_PLUS_MAC                       45
#define WKEY_MINUS_MAC                      47
#define WKEY_Z_MAC                          KEY_Z
#define WKEY_X_MAC                          KEY_X

// Paths
#define PATH_SUPPORT_DIR_NAME_MAC           "Library/Application Support"
#define PATH_APP_DIR_NAME_MAC               "ZRods"
#define PATH_SEP_MAC                        "/"


#endif // MACOS_GUARD


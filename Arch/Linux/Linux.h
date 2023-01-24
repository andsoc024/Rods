// ============================================================================
// RODS
// Linux
// by Andreas Socratous
// Jan 2023
// ============================================================================


#ifndef LINUX_GUARD
#define LINUX_GUARD


// ============================================================================ INFO
/*
    Constants specific for the Linux platform.
*/


// ============================================================================ DEPENDENCIES

// Included in Public.h when PLATFORM_LINUX is defined

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>


// ============================================================================ CONSTANTS

#define ARCH                                LIN
#define APP_PLATFORM_LIN                    "Linux"

#define UNUSED_LIN                          __attribute__((unused))

// Keyboard Keys (Watched Keys)
#define WKEY_RIGHT_LIN                      KEY_RIGHT
#define WKEY_DOWN_LIN                       KEY_DOWN
#define WKEY_LEFT_LIN                       KEY_LEFT
#define WKEY_UP_LIN                         KEY_UP
#define WKEY_D_LIN                          KEY_D
#define WKEY_S_LIN                          KEY_S
#define WKEY_A_LIN                          KEY_A
#define WKEY_W_LIN                          KEY_W
#define WKEY_SPACE_LIN                      KEY_SPACE
#define WKEY_ENTER_LIN                      KEY_ENTER
#define WKEY_TAB_LIN                        KEY_TAB
#define WKEY_M_LIN                          KEY_M
#define WKEY_T_LIN                          KEY_T
#define WKEY_R_LIN                          KEY_R
#define WKEY_PLUS_LIN                       KEY_KP_ADD
#define WKEY_MINUS_LIN                      KEY_KP_SUBTRACT
#define WKEY_Z_LIN                          KEY_Z
#define WKEY_X_LIN                          KEY_X

#define MOUSE_WHEEL_SENITIVITY_LIN          0.05f


#endif // LINUX_GUARD


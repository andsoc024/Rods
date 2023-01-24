// ============================================================================
// RODS
// Colors
// by Andreas Socratous
// Jan 2023
// ============================================================================


#ifndef COLORS_GUARD
#define COLORS_GUARD


// ============================================================================ INFO
/*
    Definitions of colors.
*/


// ============================================================================ DEPENDENCIES

// Included in Public.h


// ============================================================================ COLORS

#define COL_NULL                            COLOR(0x00, 0x00, 0x00, 0x00)

// Background Color
#define COL_BG                              COLOR(0x23, 0x23, 0x23, 0xFF)

// Rod Colors
#define COL_ROD                             COLOR(0x2E, 0xA0, 0x5D, 0xFF)
#define COL_ELECTRIC_1                      COLOR(0xFB, 0xA5, 0x4A, 0xFF)
#define COL_ELECTRIC_2                      COLOR(0xFB, 0xE0, 0x7F, 0xFF)

// Selection Box Color
#define COL_SELBOX                          COLOR(0xBB, 0x33, 0x33, 0xFF)

// Flying Rectangles Color
#define COL_FRECT                           COLOR(0x55, 0x55, 0x55, 0xFF)

// GUI Colors
#define COL_UI_FG_PRIMARY                   COLOR(0xB1, 0x77, 0x41, 0xFF) // ex Text in Buttons
#define COL_UI_FG_SECONDARY                 COLOR(0xB6, 0xB6, 0xB6, 0xFF) // ex Text in Textbox, Timer
#define COL_UI_FG_EMPH                      COLOR(0xF4, 0xE1, 0x5A, 0xFF) // ex Text in buttons, when selected

#define COL_UI_BG_PRIMARY                   COLOR(0x3C, 0x3C, 0x3C, 0xFF) // ex Button Background
#define COL_UI_BG_SECONDARY                 COLOR(0x2A, 0x2A, 0x2A, 0xFF) // ex Scrollbar Background
#define COL_UI_BG_TERTIARY                  COLOR(0x58, 0x58, 0x58, 0xDD) // ex Toolbar
#define COL_UI_BG_QUATENARY                 COLOR(0x0B, 0x0B, 0x0B, 0xFF) // ex Textbox Background
#define COL_UI_BG_ACTIVE                    COLOR(0x00, 0xA4, 0x30, 0xFF) // ex Active Background in Switch

#define COL_PLAY_BTN                        COLOR(0x00, 0x59, 0x99, 0xFF) // Play Button Background



#endif // COLORS_GUARD


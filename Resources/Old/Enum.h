// ============================================================================
// RODS
// Enumerations Header
// by Andreas Socratous
// Sep 2022
// ============================================================================


#ifndef ENUM_GUARD
#define ENUM_GUARD


// ============================================================================ INFO
/*
    Definitions of enumerations and declarations of enumeration functions.

    IsValid functions return true when the value is part of the enumeration.

    ToString functions return a string describing the enumeration value.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>


// ============================================================================ ENUMERATIONS

// **************************************************************************** bool

// A string describing the given boolean value, in long (True, False) or short 
// form (T, F)
#ifdef DEBUG_MODE
    const char* Bool_ToString(bool value, bool form);
#endif


// **************************************************************************** KeyboardKey

#ifdef DEBUG_MODE
    const char* KeyboardKey_ToString(KeyboardKey key);
#endif


// **************************************************************************** E_ZeroValMode

// Zero Values Mode. Values to be set to 0 after memory allocation
typedef enum E_ZeroValMode{
    ZEROVAL_NO,
    ZEROVAL_ALL,
    ZEROVAL_LAST,
    ZEROVAL_MODES_N
}E_ZeroValMode;

bool ZeroValMode_IsValid(int zeroValMode);
#ifdef DEBUG_MODE
    const char* ZeroValMode_ToString(int zeroValMode);
#endif


// **************************************************************************** E_Direction

// Direction enumeration
typedef enum E_Direction{
    DIR_NONE,
    DIR_RIGHT,
    DIR_DOWN,
    DIR_LEFT,
    DIR_UP,
    DIRECTIONS_N
}E_Direction;

bool Direction_IsValid(int dir, bool includeNone);
#ifdef DEBUG_MODE
    const char* Direction_ToString(int dir, bool form);
#endif


// **************************************************************************** E_RectPointType

// Rectangle Point Type
typedef enum E_RectPointType{
    RP_TOP_LEFT,    RP_TOP_CENTER,    RP_TOP_RIGHT,
    RP_MIDDLE_LEFT, RP_CENTER,        RP_MIDDLE_RIGHT,
    RP_BOTTOM_LEFT, RP_BOTTOM_CENTER, RP_BOTTOM_RIGHT,
    RECT_POINT_TYPES_N
}E_RectPointType;

bool RectPointType_IsValid(int pointType);
#ifdef DEBUG_MODE
    const char* RectPointType_ToString(int pointType);
#endif


// **************************************************************************** E_IconID

// Icon ID
typedef enum E_IconID{
    ICON_ARROW_RIGHT, ICON_ARROW_DOWN, ICON_ARROW_LEFT, ICON_ARROW_UP,
    ICON_BACK,        ICON_HELP,       ICON_INFO,       ICON_MEDAL,
    ICON_MUSIC,       ICON_RESTART,    ICON_ZOOM_IN,    ICON_ZOOM_OUT,
    ICONS_N
}E_IconID;

bool IconID_IsValid(int id);
#ifdef DEBUG_MODE
    const char* IconID_ToString(int id);
#endif


// **************************************************************************** E_EventID

// Event ID
typedef enum E_EventID{
    EVENT_NONE,             EVENT_MOUSE_MOVE,      EVENT_MOUSE_DRAG,        EVENT_MOUSE_PRESSED, 
    EVENT_MOUSE_RELEASED,   EVENT_MOUSE_WHEEL,     EVENT_KEY_PRESSED,       EVENT_KEY_RELEASED,
    EVENT_BUTTON_PRESSED,   EVENT_BUTTON_RELEASED, EVENT_SCROLLBAR_CHANGED, EVENT_UPDATE_SCROLLBAR,
    EVENT_SWITCH_CHANGED,   EVENT_UPDATE_SWITCH,   EVENT_NUMBOX_CHANGED,    EVENT_GADGET_EXPANDED,
    EVENT_GADGET_COLLAPSED, EVENT_SHOW_PAGE,       EVENT_HIDE_PAGE,         EVENT_VICTORY,
    EVENT_MAKE_NEW_GRID,    
    EVENTS_N
}E_EventID;

bool EventID_IsValid(int id);
#ifdef DEBUG_MODE
    const char* EventID_ToString(int id);
#endif


// **************************************************************************** E_GadgetID

// Gadget ID
typedef enum E_GadgetID{
    GDG_NONE,                   GDG_BOARD,                    GDG_TOOLBAR,                  GDG_SCROLLBAR_HOR,
    GDG_BTN_SB_LEFT,            GDG_BTN_SB_RIGHT,             GDG_SCROLLBAR_VER,            GDG_BTN_SB_UP,
    GDG_BTN_SB_DOWN,            GDG_TIMER,                    GDG_BTN_BACK,                 GDG_BTN_RESTART,
    GDG_BTN_ZOOM_IN,            GDG_BTN_ZOOM_OUT,             GDG_BTN_HELP,                 GDG_BTN_INFO,
    GDG_LBL_SOUND,              GDG_SWITCH_SOUND,             GDG_LBL_TITLE,                GDG_BTN_PLAY,
    GDG_NB_RESTART_COLS,        GDG_BTN_NB_RESTART_COLS_UP,   GDG_BTN_NB_RESTART_COLS_DOWN, GDG_NB_RESTART_ROWS,
    GDG_BTN_NB_RESTART_ROWS_UP, GDG_BTN_NB_RESTART_ROWS_DOWN, GDG_BTN_RESTART_OK,           GDG_NB_COMPL_COLS,
    GDG_BTN_NB_COMPL_COLS_UP,   GDG_BTN_NB_COMPL_COLS_DOWN,   GDG_NB_COMPL_ROWS,            GDG_BTN_NB_COMPL_ROWS_UP,
    GDG_BTN_NB_COMPL_ROWS_DOWN, GDG_LBL_NEW_RECORD,           GDG_TABLE_TIME,               GDG_BTN_COMPL_OK,
    GDG_TABLE_HELP,             GDG_BTN_HELP_OK,              GDG_LBL_INFO_TITLE,           GDG_LBL_INFO_AUTHOR,
    GDG_LBL_RODS_LEFT,          GDG_LBL_RESTART_MEDAL,        GDG_LBL_RESTART_RECORD,       
    #ifdef DEBUG_MODE
    GDG_GENERIC_1,              GDG_GENERIC_2,                GDG_GENERIC_3,                GDG_GENERIC_4,
    GDG_GENERIC_5,              GDG_GENERIC_6,                GDG_GENERIC_7,                GDG_GENERIC_8,
    GDG_GENERIC_9,              GDG_GENERIC_10,               GDG_GENERIC_11,               GDG_GENERIC_12,
    GDG_GENERIC_13,             GDG_GENERIC_14,               GDG_GENERIC_15,               GDG_GENERIC_16,
    #endif
    GADGETS_N
}E_GadgetID;

bool GadgetID_IsValid(int id);
#ifdef DEBUG_MODE
    const char* GadgetID_ToString(int id);
#endif


// **************************************************************************** E_PageID

// Page ID
typedef enum E_PageID{
    PAGE_GAME,      PAGE_MAIN,      PAGE_RESTART,   PAGE_COMPLETED,
    PAGE_HELP,      PAGE_INFO,
    #ifdef DEBUG_MODE
    PAGE_GENERIC_1, PAGE_GENERIC_2, PAGE_GENERIC_3, PAGE_GENERIC_4,
    #endif
    PAGES_N
}E_PageID;

bool PageID_IsValid(int id);
#ifdef DEBUG_MODE
    const char* PageID_ToString(int id);
#endif


// **************************************************************************** E_Orientation

// Orientation: Horisontal or Vertical
typedef enum E_Oriantation{
    OR_HORISONTAL,
    OR_VERTICAL,
    ORIENTATIONS_N
}E_Oriantation;

bool Orientation_IsValid(int orientation);
#ifdef DEBUG_MODE
    const char* Orientation_ToString(int orientation);
#endif




#endif // ENUM_GUARD


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


// **************************************************************************** E_EventID

// Unique ID for each GUI event
typedef enum E_EventID{
    EVENT_NONE,            EVENT_MOUSE_MOVE,        EVENT_MOUSE_DRAG, 
    EVENT_MOUSE_PRESSED,   EVENT_MOUSE_RELEASED,    EVENT_MOUSE_WHEEL,
    EVENT_KEY_PRESSED,     EVENT_KEY_RELEASED,      EVENT_BUTTON_PRESSED,
    EVENT_BUTTON_RELEASED, EVENT_SCROLLBAR_CHANGED, EVENT_UPDATE_SCROLLBAR,
    EVENT_SWITCH_CHANGED,  EVENT_UPDATE_SWITCH,     EVENT_NUMBOX_CHANGED,
    EVENT_GADGET_EXPANDED, EVENT_GADGET_COLLAPSED,  EVENT_SHOW_PAGE,
    EVENT_HIDE_PAGE,       EVENT_VICTORY,           EVENT_MAKE_NEW_GRID
}E_EventID;
#define EVENTS_N                            (EVENT_MAKE_NEW_GRID + 1)

bool            EventID_IsValid(int id);
#ifdef DEBUG_MODE
    const char* EventID_ToString(int id);
#endif


// **************************************************************************** E_PageID

// Unique ID for each Page
typedef enum E_PageID{
    PAGE_MAIN,      PAGE_GAME,      PAGE_SETUP,
    PAGE_HELP,      PAGE_INFO,
    #ifdef DEBUG_MODE
    PAGE_GENERIC_1, PAGE_GENERIC_2, PAGE_GENERIC_3,
    PAGE_GENERIC_4, PAGE_GENERIC_5, PAGE_GENERIC_6
    #endif
}E_PageID;
#ifdef DEBUG_MODE
    #define PAGES_N                         (PAGE_GENERIC_6 + 1)
#else
    #define PAGES_N                         (PAGE_INFO + 1)
#endif

bool            PageID_IsValid(int id);
#ifdef DEBUG_MODE
    const char* PageID_ToString(int id);
#endif


// **************************************************************************** E_GadgetID

// Unique ID for each Gadget
typedef enum E_GadgetID{
    GDG_NONE = PAGES_N,
    GDG_BOARD,              GDG_TOOLBAR,              GDG_SCROLLBAR_HOR,
    GDG_SBAR_LEFT_BTN,      GDG_SBAR_RIGHT_BTN,       GDG_SCROLLBAR_VER,
    GDG_SBAR_UP_BTN,        GDG_SBAR_DOWN_BTN,        GDG_TIMER,
    GDG_LBL_RODS_LEFT,      GDG_BTN_BACK,             GDG_BTN_RESTART,
    GDG_BTN_ZOOM_IN,        GDG_BTN_ZOOM_OUT,         GDG_BTN_HELP,
    GDG_BTN_INFO,           GDG_ICON_MUSIC,           GDG_SWITCH_SOUND,
    GDG_TITLE,              GDG_BTN_PLAY,             GDG_NUMBOX_COLS,
    GDG_NUMBOX_COLS_BTN_UP, GDG_NUMBOX_COLS_BTN_DOWN, GDG_NUMBOX_ROWS,
    GDG_NUMBOX_ROWS_BTN_UP, GDG_NUMBOX_ROWS_BTN_DOWN, GDG_SETUP_LBL_NEW_RECORD,
    GDG_SETUP_LBL_TIME,     GDG_SETUP_TIME_CURRENT,   GDG_SETUP_LBL_RECORD,
    GDG_SETUP_TIME_RECORD,  GDG_HELP_TABLE,           GDG_INFO_LBL_TITLE,
    GDG_INFO_LBL_AUTHOR, 
    #ifdef DEBUG_MODE
    GDG_GENERIC_1,          GDG_GENERIC_2,            GDG_GENERIC_3,
    GDG_GENERIC_4,          GDG_GENERIC_5,            GDG_GENERIC_6,
    GDG_GENERIC_7,          GDG_GENERIC_8,            GDG_GENERIC_9,
    GDG_GENERIC_10,         GDG_GENERIC_11,           GDG_GENERIC_12
    #endif
}E_GadgetID;
#ifdef DEBUG_MODE
    #define GADGETS_N                       (GDG_GENERIC_12 - GDG_NONE + 1)
#else
    #define GADGETS_N                       (GDG_INFO_LBL_AUTHOR - GDG_NONE + 1)
#endif

bool            GadgetID_IsValid(int id);
#ifdef DEBUG_MODE
    const char* GadgetID_ToString(int id);
#endif


// **************************************************************************** E_GuiID

// Unique ID for each Page or Gadget
typedef int E_GuiID;

#define GUI_OBJECTS_N                       (PAGES_N + GADGETS_N)

bool            GuiID_IsValid(int id);
#ifdef DEBUG_MODE
    const char* GuiID_ToString(int id);
#endif


// **************************************************************************** E_GadgetType

// The kinds of gadgets
typedef enum E_GadgetType{
    GT_GENERIC, GT_BOARD,     GT_LABEL,
    GT_BUTTON,  GT_SCROLLBAR, GT_SWITCH,
    GT_NUMBOX,  GT_TIMER,     GT_ICON,
    GT_TABLE
}E_GadgetType;
#define GADGET_TYPES_N                      (GT_TABLE + 1)

bool            GadgetType_IsValid(int type);
#ifdef DEBUG_MODE
    const char* GadgetType_ToString(int type);
#endif


#endif // ENUM_GUARD


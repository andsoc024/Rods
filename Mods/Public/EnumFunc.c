// ============================================================================ 
// RODS
// Enumeration Functions
// by Andreas Socratous
// Jan 2023
// ============================================================================ 


// ============================================================================ INFO
/*
    Definitions of enumeration functions.

    IsValid functions return true if the value is a valid enumeration value.

    ToString functions return a string describing the enumeration value.
*/


// ============================================================================ DEPENDENCIES

// Included in Public.c

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "Public.h"


// ============================================================================  ENUMERATION FUNCTIONS

// **************************************************************************** bool

#ifdef DEBUG_MODE
    const char* Bool_ToString(bool value, bool form){
        if (form == SHORT_FORM){
            return value ? "T" : "F";
        }

        return value ? "True" : "False";
    }
#endif


// **************************************************************************** KeyboardKey

// Return true if the key is in the WATCHED_KEYS array
bool KeyboardKey_IsValid(KeyboardKey key){
    for (int i = 0; i < WATCHED_KEYS_N; i++){
        if (key == WATCHED_KEYS[i]){
            return true;
        }
    }

    return false;
}

#ifdef DEBUG_MODE
    // Return a string describing the keyboard key
    const char* KeyboardKey_ToString(KeyboardKey key){
        static const char* names[] = {
            "Right", "Down",  "Left", "Up",
            "D",     "S",     "A",    "W",
            "Space", "Enter", "Tab",  "M",
            "T",     "R",     "Plus", "Minus",
            "Z",     "X"
        };

        for (int i = 0; i < WATCHED_KEYS_N; i++){
            if (key == WATCHED_KEYS[i]){
                return names[i];
            }
        }

        return STR_INVALID;
    }

    // Print the code and name of the pressed keys in the terminal
    void KeyboardKey_Test(void){
        InitWindow(100, 100, "Keyboard Test");
        SetTargetFPS(60);

        while (!WindowShouldClose()){
            KeyboardKey key = GetKeyPressed();
            if (key != KEY_NULL){
                printf("%d\t%s\n", key, KeyboardKey_ToString(key));
            }

            BeginDrawing();
            ClearBackground(BLACK);
            EndDrawing();
        }

        CloseWindow();
    }
#endif


// **************************************************************************** E_ZeroValMode

// Return true if the value is a valid zero values mode
bool ZeroValMode_IsValid(int mode){
    return IS_IN_RANGE(mode, 0, ZEROVAL_MODES_N - 1);
}

#ifdef DEBUG_MODE
    // Return a string describing the zero values mode
    const char* ZeroValMode_ToString(int mode){
        static const char* names[] = {"None", "All", "Last"};

        CHECK_INVALID(ZeroValMode_IsValid(mode), LONG_FORM);

        return names[mode];
    }
#endif


// **************************************************************************** E_Direction

// Return true if the value is a valid direction, optionally including DIR_NONE
bool Direction_IsValid(int dir, bool includeNone){
    int startDir = includeNone ? DIR_NONE : DIR_RIGHT;
    return IS_IN_RANGE(dir, startDir, DIR_UP);
}

#ifdef DEBUG_MODE
    // Return a string describing the direction, in short or long form
    const char* Direction_ToString(int dir, bool form){
        const char* names[] = {"None", "Right", "Down", "Left", "Up"};
        const char* shortNames[] = {"N", "R", "D", "L", "U"};

        CHECK_INVALID(Direction_IsValid(dir, INCLUDE_NONE), form)

        return (form == SHORT_FORM) ? shortNames[dir] : names[dir];
    }
#endif


// **************************************************************************** E_Orientation

// Return true if the value is a valid orientation value
bool Orientation_IsValid(int orientation){
    return IS_IN_RANGE(orientation, 0, ORIENTATIONS_N - 1);
}

#ifdef DEBUG_MODE
    // Return a string describing the orientation
    const char* Orientation_ToString(int orientation){
        const char* names[] = {"Horisontal", "Vertical"};

        CHECK_INVALID(Orientation_IsValid(orientation), LONG_FORM)

        return names[orientation];
    }
#endif


// **************************************************************************** E_RectPointType

// Return true if the value is a valid rectangle point type
bool RectPointType_IsValid(int pointType){
    return IS_IN_RANGE(pointType, 0, RECT_POINT_TYPES_N - 1);
}

#ifdef DEBUG_MODE
    // Return a string describing the rectangle point type
    const char* RectPointType_ToString(int pointType){
        const char* names[] = {
            "Top Left",    "Top Center",    "Top Right",
            "Middle Left", "Center",        "Middle Right",
            "Bottom Left", "Bottom Center", "Bottom Right"
        };

        CHECK_INVALID(RectPointType_IsValid(pointType), LONG_FORM)

        return names[pointType];
    }
#endif


// **************************************************************************** E_IconID

// Return true if the value is a valid icon id
bool IconID_IsValid(int id){
    return IS_IN_RANGE(id, 0, ICONS_N - 1);
}

#ifdef DEBUG_MODE
    // Return a string describing the icon
    const char* IconID_ToString(int id){
        static const char* names[] = {
            "Right Arrow", "Down Arrow", "Left Arrow", "Up Arrow",
            "Back",        "Help",       "Info",       "Medal",
            "Music",       "Restart",    "Zoom In",    "Zoom Out"
        };

        CHECK_INVALID(IconID_IsValid(id), LONG_FORM);

        return names[id];
    }
#endif


// **************************************************************************** E_EventID

// Return true if the value is a valid event id
bool EventID_IsValid(int id){
    return IS_IN_RANGE(id, 0, EVENTS_N - 1);
}

#ifdef DEBUG_MODE
    // Return a string describing the event id
    const char* EventID_ToString(int id){
        static const char* names[] = {
            "None",                      "Mouse Move",                 "Mouse Drag", 
            "Left Mouse Button Pressed", "Left Mouse Button Released", "Mouse Wheel Moved",
            "Key Pressed",               "Key Released",               "Button Pressed",
            "Button Released",           "Scrollbar Changed",          "Update Scrollbar",
            "Switch Changed",            "Update Switch",              "Numberbox Changed",
            "Gadget Expanded",           "Gadget Collapsed",           "Show Page",
            "Hide Page",                 "Victory",                    "Make New Grid",
            "Generic Event"
        }; 

        CHECK_INVALID(EventID_IsValid(id), LONG_FORM)

        return names[id]; 
    }
#endif


// **************************************************************************** E_PageID

// Return true if the value is a valid page id
bool PageID_IsValid(int id){
    return IS_IN_RANGE(id, 0, PAGES_N - 1);
}

#ifdef DEBUG_MODE
    // Return a string describing the page
    const char* PageID_ToString(int id){
        static const char* names[] = {
            "Main Page",      "Game Page",      "Setup Page",
            "Help Page",      "Info Page",
            "Generic Page 1", "Generic Page 2", "Generic Page 3",
            "Generic Page 4", "Generic Page 5", "Generic Page 6"
        };

        CHECK_INVALID(PageID_IsValid(id), LONG_FORM)

        return names[id];
    }
#endif


// **************************************************************************** E_GadgetID

// Return true if the value is a valid gadget id
bool GadgetID_IsValid(int id){
    return IS_IN_RANGE(id - GDG_NONE, 0, GADGETS_N - 1);
}

#ifdef DEBUG_MODE
    const char* GadgetID_ToString(int id){
        static const char* names[] = {
            "None",
            "Board",                    "Toolbar",                    "Horisontal Scrollbar",
            "Left Scrollbar Button",    "Right Scrollbar Button",     "Vertical Scrollbar",
            "Up Scrollbar Button",      "Down Scrollbar Button",      "Timer",
            "Rods Left Label",          "Back Button",                "Restart Button",
            "Zoom In Button",           "Zoom Out Button",            "Help Button",
            "Info Button",              "Music Icon",                 "Sound Switch",
            "Title",                    "Play Button",                "Columns Number Box",
            "Up Button of Cols NumBox", "Down Button of Cols NumBox", "Rows Number Box",
            "Up Button of Rows NumBox", "Down Button of Rows NumBox", "New Record Label",
            "Time Label in Setup",      "Current Time in Setup",      "Record Label in Setup",
            "Record Time in Setup",     "Help Table",                 "Title Label in Info",
            "Author Label in Info", 
            "Generic Gadget 1",         "Generic Gadget 2",           "Generic Gadget 3",
            "Generic Gadget 4",         "Generic Gadget 5",           "Generic Gadget 6",
            "Generic Gadget 7",         "Generic Gadget 8",           "Generic Gadget 9",
            "Generic Gadget 10",        "Generic Gadget 11",          "Generic Gadget 12"         
        };

        CHECK_INVALID(GadgetID_IsValid(id), LONG_FORM)

        return names[id - GDG_NONE];
    }
#endif


// **************************************************************************** E_GuiID

// Return true if the value is a valid GUI id
bool GuiID_IsValid(int id){
    return IS_IN_RANGE(id, 0, GUI_OBJECTS_N - 1);
}

#ifdef DEBUG_MODE
    // Return a string describing the GUI object (page or gadget)
    const char* GuiID_ToString(int id){
        if (PageID_IsValid(id)){
            return PageID_ToString(id);
        }

        if (GadgetID_IsValid(id)){
            return GadgetID_ToString(id);
        }

        return STR_INVALID;
    }
#endif


// **************************************************************************** E_GadgetType

// Return true if the value is a valid gadget type
bool GadgetType_IsValid(int type){
    return IS_IN_RANGE(type, 0, GADGET_TYPES_N - 1);
}

#ifdef DEBUG_MODE
    // Return a string describing the gadget type
    const char* GadgetType_ToString(int type){
        static const char* names[] = {
            "Generic",    "Board",     "Label",
            "Button",     "Scrollbar", "Switch",
            "Number Box", "Timer",     "Icon",
            "Table"
        };

        CHECK_INVALID(GadgetType_IsValid(type), LONG_FORM)

        return names[type];
    }
#endif




// ============================================================================
// RODS
// Structures (GUI) Header
// by Andreas Socratous
// Jan 2023
// ============================================================================


#ifndef STRUCT_GUI_GUARD
#define STRUCT_GUI_GUARD


// ============================================================================ INFO
/*
    Definitions of GUI structures.

    Gadgets are the fundamental units of the GUI. They are organised in Pages. 
    The Pages are controlled by the Router.
*/


// ============================================================================ DEPENDENCIES

// Included in Public.h


// ============================================================================ STRUCTURES

// ---------------------------------------------------------------------------- Event

// A mouse, keyboard or other GUI event
typedef struct Event{
    E_EventID id;
    E_GuiID source;
    E_GuiID target;
    
    union{
        struct{
            Point pos;
            Vector2 delta;
        }mouse;

        struct{
            float move;
            Point pos;
        }wheel;

        KeyboardKey key;

        struct{
            float posRatio;
            float sizeRatio;
        }scrollbar;

        bool switchValue;

        int numboxValue;

        struct{
            int nCols;
            int nRows;
        }newGrid;

        struct{
            E_PageID id;
            bool withAnim;
            struct{
                bool victory;
                unsigned char nCols;
                unsigned char nRows;
                Time time;
            }data;
        }page;
    }data;
}Event;






#endif // STRUCT_GUI_GUARD


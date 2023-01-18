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

// **************************************************************************** Event

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


// **************************************************************************** EventQueue

// Structure that stores events. Events are stored in two lists: a current and 
// a next list. In each cycle the events in the current list are processed. New 
// events are added in the next list. Events from mouse and keyboard are stored 
// seperately for immediate processing
typedef struct EventQueue EventQueue;


// **************************************************************************** Gadget

// A gadget is the elementary unit of the GUI
typedef struct Gadget{
    E_GadgetID id;
    E_GadgetType type;

    Rect cRect;

    bool isSelected;
    bool isSelectable;
    bool isPressed;
    bool isCollapsed;

    struct Gadget** subGadgets;
    int nSubGadgets;

    void (*PrepareToFree)(struct Gadget* gadget);
    void (*Resize)(struct Gadget* gadget);
    void (*ResizeAfterSubgadgets)(struct Gadget* gadget);
    void (*ReactToEvent)(struct Gadget* gadget, Event event, EventQueue* queue);
    void (*Update)(struct Gadget* gadget, EventQueue* queue);
    void (*Draw)(struct Gadget* gadget, Vector2 shift);
    #ifdef DEBUG_MODE
        void (*PrintData)(struct Gadget* gadget);
    #endif

    void* data;
}Gadget;




#endif // STRUCT_GUI_GUARD


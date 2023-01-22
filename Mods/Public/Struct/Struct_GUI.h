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
    bool isExpanded;

    struct Gadget** subGadgets;
    int nSubGadgets;

    void     (*PrepareToFree)(struct Gadget* gadget);
    void     (*Resize)(struct Gadget* gadget);
    void     (*ResizeAfterSubgadgets)(struct Gadget* gadget);
    void     (*ReactToEvent)(struct Gadget* gadget, Event event, EventQueue* queue);
    void     (*Update)(struct Gadget* gadget, EventQueue* queue);
    Vector2  (*ChangeShift)(const struct Gadget* gadget, Vector2 shift);
    void     (*Draw)(const struct Gadget* gadget, Vector2 shift);
    #ifdef DEBUG_MODE
        void (*PrintData)(const struct Gadget* gadget);
    #endif

    void* data;
}Gadget;


// **************************************************************************** Page

// A page is the second unit of the GUI. It can contain one or more gadgets. 
// Pages are controlled by the router
typedef struct Page{
    E_PageID id;

    bool isShown;

    Vector2 shift;
    float shiftIncr;

    Gadget* gadgets[PAGE_GADGETS_N];
    int nGadgets;

    void (*PrepareToFree)(struct Page* page);
    void (*PrepareToShow)(struct Page* page);
    void (*Resize)(struct Page* page);
    void (*ResizeAfterGadgets)(struct Page* page);
    void (*ReactToEvent)(struct Page* page, Event event, EventQueue* queue);
    void (*Update) (struct Page* page, EventQueue* queue);
    void (*Draw)(const struct Page* page);
    #ifdef DEBUG_MODE
        void (*PrintData)(const struct Page* page);
    #endif

    void* data;
}Page;


// **************************************************************************** Router

// The router is the top unit of the GUI. It controls pages and events and it 
// also manages the magic color and the flying rectangles
typedef struct Router{
    EventQueue* queue;

    Page* pages[PAGES_N];

    FRects* fRects;
}Router;



#endif // STRUCT_GUI_GUARD


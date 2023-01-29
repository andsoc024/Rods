// ============================================================================
// RODS
// Structures (Fundamental) Header
// by Andreas Socratous
// Jan 2023
// ============================================================================


#ifndef STRUCT_FUND_GUARD
#define STRUCT_FUND_GUARD


// ============================================================================ INFO
/*
    Math, geometry, grid and time structures, as well as logic, graphics as 
    well as storage structures.
*/


// ============================================================================ DEPENDENCIES

// Included in Public.h


// ============================================================================ STRUCTURES

// ---------------------------------------------------------------------------- Math Structures

// **************************************************************************** Bytes

// 1-4 bytes representing an integer or a float
typedef struct Bytes{
    unsigned char values[4];
    int n;
}Bytes;


// ---------------------------------------------------------------------------- Geometry Structures

// **************************************************************************** Point

// A point in the cartesian plane with float x, y coords. Typedef of Vector2
typedef Vector2 Point;
    // float x;
    // float y;


// **************************************************************************** Size

// Size with float width and height
typedef struct Size{
    float width;
    float height;
}Size;


// **************************************************************************** Rect

// A rectangle with top left corner at (x,y) and float width and height. 
// Typedef of Rectangle
typedef Rectangle Rect;
    // float x;
    // float y;
    // float width;
    // float height;


// ---------------------------------------------------------------------------- Grid Structures

// **************************************************************************** GNode

// Grid node with integer x and y coords
typedef struct GNode{
    int x;
    int y;
}GNode;


// **************************************************************************** Grid

// A grid with top left corner at origin and size nCols (number of columns) x 
// nRows (number of rows)
typedef struct Grid{
    int nCols;
    int nRows;
    GNode origin;
}Grid;


// ---------------------------------------------------------------------------- Time Structures

// **************************************************************************** Time

// Time in hours, minutes, seconds
typedef union Time{
    struct{
        char hours;
        char minutes;
        char seconds;
    };

    char values[3];
}Time;






// ---------------------------------------------------------------------------- Logic Structures

// **************************************************************************** Rod

// The rod is the elementary unit of the game. It consists of 1-4 legs, with 
// which it can connect to adjacent rods. It can be electrified or not
typedef struct Rod{
    unsigned char legs:4;
    bool isElectrified:1;
    unsigned char frame;
}Rod;


// **************************************************************************** RGrid

// A grid of rods of which one is the source. The rods can be rotated until all 
// are connected to the source
typedef struct RGrid RGrid;


// **************************************************************************** Records

// Structure for storing the minimum record time for various grid sizes
typedef struct Records Records;






// ---------------------------------------------------------------------------- Graphics Structures

// **************************************************************************** MagicColor

// The magic colors changes gradually from the first to the last color in its 
// spectrum. Then back to the first and then it pauses. It repeats with set 
// period.
typedef struct MagicColor MagicColor;


// **************************************************************************** VGraph

// Virtual Graphics transforms points by scaling and translating, so that a 
// virtual screen is projected in a real screen
typedef struct VGraph VGraph;


// **************************************************************************** SGraph

// Scroll Graphics transforms points by translating, so that only the part of 
// a virtual screen that is inside its viewport is visible in the view in the 
// screen. The viewport and view have always the same size
typedef struct SGraph SGraph;


// **************************************************************************** FRects

// Flying rectangles are the semitransparent rectangles that move from left to 
// right, at different speeds, in the background. FRects is a collection of 
// flying rectangles.
typedef struct FRects FRects;


// **************************************************************************** Textures

// Structure for storing the icon, rod and selection box textures
typedef struct Textures{
    Texture2D icons[ICONS_N];
    Texture2D rods[0x10];
    Texture2D selBox;
}Textures;


// **************************************************************************** RodModel

// Structure that contains precalculated values for drawing the rod textures
typedef struct RodModel RodModel;


// **************************************************************************** TimDisp

// Precalculated values for drawing the time in the form [HH:]MM:SS
typedef struct TimDisp TimDisp;






// ---------------------------------------------------------------------------- Store Structures

// **************************************************************************** PData

// Persistent data loaded from the data file
typedef struct PData{
    char* version;
    Records* records;
    RGrid* rGrid;
    SGraph* sg;
    Time time;
    bool sound;
}PData;


// ---------------------------------------------------------------------------- Sound Structures

// **************************************************************************** SoundData

// Structure for storing music and sound data
typedef struct SoundData{
    Music music;
    Sound soundFX[SOUND_FX_N];

    bool soundOn;

    float musicVolume;
    float musicDVol;
}SoundData;



#endif // STRUCT_FUND_GUARD


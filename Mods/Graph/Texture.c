// ============================================================================
// RODS
// Textures
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions for managing the icon, rod and selection box textures as well as 
    drawing the rod textures.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Assets/Assets.h"

#include "../Public/Public.h"
#include "../Fund/Fund.h"
#include "../Logic/Logic.h"
#include "Graph.h"


// ============================================================================ OPAQUE STRUCTURES

// **************************************************************************** RodModel

// Structure that contains precalculated values for drawing the rod textures
struct RodModel{
    float tileSize;
    float scaleF;

    struct{
        float angle;
        Vector2 unitShift;
        Vector2 shift;
    }frames[ROD_ROT_FRAMES_N];
};


// ============================================================================ PRIVATE FUNC DECL

static Texture2D Texture_LoadIcon(E_IconID id);
static Texture2D Texture_LoadRod(int legs);
static Texture2D Texture_LoadSelBox(void);






// ============================================================================ FUNC DEF

// ---------------------------------------------------------------------------- Texture Functions

// **************************************************************************** Texture_LoadAll

// Load all the icon textures and make the rod and selection box textures. 
// Store them in the global Glo_Textures
void Texture_LoadAll(void){
    // Load the icons
    for (int iconID = 0; iconID < ICONS_N; iconID++){
        Glo_Textures.icons[iconID] = Texture_LoadIcon(iconID);
    }

    // Make the rod textures
    for (int legs = 0x0; legs <= 0xF; legs++){
        Glo_Textures.rods[legs] = Texture_LoadRod(legs);
    }

    // Make the selection box texture
    Glo_Textures.selBox = Texture_LoadSelBox();
}


// **************************************************************************** Texture_UnloadAll

// Unload all the textures stored in Glo_Textures
void Texture_UnloadAll(void){
    // Unload the icons
    for (int iconID = 0; iconID < ICONS_N; iconID++){
        UnloadTexture(Glo_Textures.icons[iconID]);
    }

    // Unload the rod textures
    for (int legs = 0x0; legs <= 0xF; legs++){
        UnloadTexture(Glo_Textures.rods[legs]);
    }

    // Unload the selection box texture
    UnloadTexture(Glo_Textures.selBox);

    Memory_Set(&Glo_Textures, sizeof(Textures), 0);
}


#ifdef DEBUG_MODE
// **************************************************************************** Texture_PrintAll

    // Multiline print of the parameters of the Textures structure
    void Texture_PrintAll(void){
        printf("Icon Textures (%d):\n", ICONS_N);
        for (int iconID = 0; iconID < ICONS_N; iconID++){
            printf("   %s: ", IconID_ToString(iconID));
            printf("%d x %d\n", Glo_Textures.icons[iconID].width, Glo_Textures.icons[iconID].height);
        }
        printf("\n");

        printf("Rod Textures:\n");
        for (int legs = 0x0; legs <= 0xF; legs++){
            printf("   %2d) ", legs);
            printf("%d x %d\n", Glo_Textures.rods[legs].width, Glo_Textures.rods[legs].height);
        }
        printf("\n");

        printf("Selection Box Texture: %d x %d\n", Glo_Textures.selBox.width, Glo_Textures.selBox.height);
    }
#endif






// ---------------------------------------------------------------------------- Rod Graphics Functions

// **************************************************************************** RGraph_MakeRodModel

// Make a rod model and initialize it for the given tile size
RodModel* RGraph_MakeRodModel(float tileSize){
    RodModel* rodModel = Memory_Allocate(NULL, sizeof(RodModel), ZEROVAL_ALL);

    const float RAD_INCR = MATH_PI_HALF / (float) (ROD_ROT_FRAMES_N - 1);

    float radians = 0.0f;
    for (int i = 0; i < ROD_ROT_FRAMES_N; i++){
        rodModel->frames[i].angle = Math_RadToDeg(radians);

        float a = Math_Sqrt(2.0f) * Math_Sin(radians * 0.5f);
        float b = (MATH_PI - 2.0f * radians) * 0.25f;

        rodModel->frames[i].unitShift.x =  a * Math_Cos(b);
        rodModel->frames[i].unitShift.y = -a * Math_Sin(b);

        radians -= RAD_INCR;
    }

    RGraph_ResizeRodModel(rodModel, tileSize);

    return rodModel;
}


// **************************************************************************** RGraph_FreeRodModel

// Free the memory of the rod model. Return NULL
RodModel* RGraph_FreeRodModel(RodModel* rodModel){
    return Memory_Free(rodModel);
}


// **************************************************************************** RGraph_ResizeRodModel

// Resize the rod model to the given tile size
void RGraph_ResizeRodModel(RodModel* rodModel, float tileSize){
    rodModel->tileSize = tileSize;
    rodModel->scaleF = tileSize / ROD_DEF_TEXTURE_SIZE;

    for (int i = 0; i < ROD_ROT_FRAMES_N; i++){
        rodModel->frames[i].shift = Geo_ScalePoint(rodModel->frames[i].unitShift, tileSize);
    }
}


// **************************************************************************** RGraph_GetTileSize

// Get the tile size
float RGraph_GetTileSize(const RodModel* rodModel){
    return rodModel->tileSize;
}


// **************************************************************************** RGraph_GetScaleF

// Get the scale factor
float RGraph_GetScaleF(const RodModel* rodModel){
    return rodModel->scaleF;
}


// **************************************************************************** RGraph_GetAngle

// Get the rotation angle of the frame in degrees
float RGraph_GetAngle(const RodModel* rodModel, int frame){
    frame = PUT_IN_RANGE(frame, 0, ROD_ROT_FRAMES_N - 1);
    return rodModel->frames[frame].angle;
}


// **************************************************************************** RGraph_GetShift

// Get the shift vector for the given rotation frame
Vector2 RGraph_GetShift(const RodModel* rodModel, int frame){
    frame = PUT_IN_RANGE(frame, 0, ROD_ROT_FRAMES_N - 1);
    return rodModel->frames[frame].shift;
}


// **************************************************************************** RGraph_ResizeRodModel

// Draw the rod using the appropriate texture in Glo_Textures and the 
// precalculated values in rodModel
void RGraph_DrawRod(const Rod* rod, Point pos, const RodModel* rodModel){
    DrawTextureEx(Glo_Textures.rods[rod->legs],                                 // Texture
                  Geo_TranslatePoint(pos, rodModel->frames[rod->frame].shift),  // Position
                  rodModel->frames[rod->frame].angle,                           // Rotation
                  rodModel->scaleF,                                             // Scale
                  rod->isElectrified ? MCol(Glo_MCol) : COL_ROD);               // Color
}


// **************************************************************************** RGraph_DrawGrid

// Draw the visible part of the rod grid, with top left corner at pos
void RGraph_DrawRGrid(const RGrid* rGrid, Grid visible, Point pos, const RodModel* rodModel){
    Point cursor = pos;
    for (int y = visible.origin.y; y < visible.origin.y + visible.nRows; y++){
        for (int x = visible.origin.x; x < visible.origin.x + visible.nCols; x++){
            Rod* rod = RGrid_GetRod(rGrid, GNODE(x, y));
            RGraph_DrawRod(rod, cursor, rodModel);
            
            cursor.x += rodModel->tileSize;
        }

        cursor.x = pos.x;
        cursor.y += rodModel->tileSize;
    }

    GNode source = RGrid_GetSource(rGrid);
    if (Grid_NodeIsInGrid(source, visible)){
        Point sourcePos = Geo_MovePoint(pos, (float) (source.x - visible.origin.x) * rodModel->tileSize, 
                                             (float) (source.y - visible.origin.y) * rodModel->tileSize);
        Shape_DrawSource(sourcePos, rodModel->tileSize, MCol(Glo_MCol));
    }
}


// **************************************************************************** RGraph_DrawSelBox

// Draw the selection box at the given position
void RGraph_DrawSelBox(Point pos, const RodModel* rodModel){
    DrawTextureEx(Glo_Textures.selBox, pos, 0.0f, rodModel->scaleF, COL_SELBOX);
}


#ifdef DEBUG_MODE
// **************************************************************************** RGraph_PrintRodModel

    // Multiline print of the parameters of the rod model
    void RGraph_PrintRodModel(RodModel* rodModel){
        CHECK_NULL(rodModel, WITH_NEW_LINE)

        printf("Tile Size: %.3f\n", rodModel->tileSize);
        printf("Scale Factor: %.3f\n", rodModel->scaleF);

        printf("Rotation Frames (%d):\n", ROD_ROT_FRAMES_N);
        for (int i = 0; i < ROD_ROT_FRAMES_N; i++){
            printf("   %d) Angle: %.3f | ", i, rodModel->frames[i].angle);
            printf("Unit Shift: "); Geo_PrintVector(rodModel->frames[i].unitShift, WITHOUT_NEW_LINE);
            printf(" | Shift: "); Geo_PrintVector(rodModel->frames[i].shift, WITH_NEW_LINE);
        }
    }
#endif


#ifdef DEBUG_MODE
// **************************************************************************** AppIcon_Create

    // Create the app icon as a png image
    void AppIcon_Create(void){
        const char* outputPath = "AppIcon.png";
        const float iconSize = 500.0f;

        Image im = GenImageColor(iconSize, iconSize, COL_BG);
        Shape_DrawRodInImage(&im, LEGDIR_RIGHT | LEGDIR_UP, COL_ROD);
        
        if (!ExportImage(im, outputPath)){
            Err_FatalError("Failed to export the app icon as image");
        }
        
        UnloadImage(im);
    }
#endif





// ============================================================================ PRIVATE FUNC DEF

// **************************************************************************** Texture_LoadIcon

// Load the icon from memory
static Texture2D Texture_LoadIcon(E_IconID id){
    const unsigned char* ptrs[ICONS_N] = {
        Asset_Icon_ArrowRight, Asset_Icon_ArrowDown, Asset_Icon_ArrowLeft,
        Asset_Icon_ArrowUp,    Asset_Icon_Back,      Asset_Icon_Help,
        Asset_Icon_Info,       Asset_Icon_Medal,     Asset_Icon_Music,
        Asset_Icon_Restart,    Asset_Icon_ZoomIn,    Asset_Icon_ZoomOut
    };

    const int dataSizes[ICONS_N] = {
        Asset_Icon_ArrowRight_Size, Asset_Icon_ArrowDown_Size, Asset_Icon_ArrowLeft_Size,
        Asset_Icon_ArrowUp_Size,    Asset_Icon_Back_Size,      Asset_Icon_Help_Size,
        Asset_Icon_Info_Size,       Asset_Icon_Medal_Size,     Asset_Icon_Music_Size,
        Asset_Icon_Restart_Size,    Asset_Icon_ZoomIn_Size,    Asset_Icon_ZoomOut_Size
    };

    Err_Assert(IconID_IsValid(id), "Invalid icon ID");

    Image im = LoadImageFromMemory(".png", ptrs[id], dataSizes[id]);
    Err_Assert(FEQ(im.width, ICON_DEF_TEXTURE_SIZE) && FEQ(im.height, ICON_DEF_TEXTURE_SIZE), 
               "Failed to load icon from memory");

    Texture2D res = LoadTextureFromImage(im);
    Err_Assert(FEQ(res.width, ICON_DEF_TEXTURE_SIZE) && FEQ(res.height, ICON_DEF_TEXTURE_SIZE), 
               "Failed to load icon as texture");

    UnloadImage(im);

    return res;
}


// **************************************************************************** Texture_LoadRod

// Make a rod texture
static Texture2D Texture_LoadRod(int legs){
    legs = PUT_IN_RANGE(legs, 0x0, 0xF);

    Image im = GenImageColor(ROD_DEF_TEXTURE_SIZE, ROD_DEF_TEXTURE_SIZE, COL_NULL);
    Err_Assert(FEQ(im.width, ROD_DEF_TEXTURE_SIZE) && FEQ(im.height, ROD_DEF_TEXTURE_SIZE), 
               "Failed to create rod image");

    Shape_DrawRodInImage(&im, legs, WHITE);

    Texture2D res = LoadTextureFromImage(im);
    Err_Assert(FEQ(res.width, ROD_DEF_TEXTURE_SIZE) && FEQ(res.height, ROD_DEF_TEXTURE_SIZE), 
               "Failed to load rod texture");

    UnloadImage(im);

    return res;
}


// **************************************************************************** Texture_LoadSelBox

// Make a selection box texture
static Texture2D Texture_LoadSelBox(void){
    Image im = GenImageColor(ROD_DEF_TEXTURE_SIZE, ROD_DEF_TEXTURE_SIZE, COL_NULL);
    Err_Assert(FEQ(im.width, ROD_DEF_TEXTURE_SIZE) && FEQ(im.height, ROD_DEF_TEXTURE_SIZE), 
               "Failed to create selection box image");

    Shape_DrawSelBoxInImage(&im, WHITE);

    Texture2D res = LoadTextureFromImage(im);
    Err_Assert(FEQ(res.width, ROD_DEF_TEXTURE_SIZE) && FEQ(res.height, ROD_DEF_TEXTURE_SIZE), 
               "Failed to load selection box texture");

    UnloadImage(im);

    return res;
}

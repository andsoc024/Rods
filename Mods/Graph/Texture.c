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
#include "Graph.h"


// ============================================================================ PRIVATE FUNC DECL

Texture2D       Texture_LoadIcon(E_IconID id);
Texture2D       Texture_LoadRod(int legs);
Texture2D       Texture_LoadSelBox(void);






// ============================================================================ FUNC DEF

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






// ============================================================================ PRIVATE FUNC DEF

// **************************************************************************** Texture_LoadIcon

// Load the icon from memory
Texture2D Texture_LoadIcon(E_IconID id){
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
Texture2D Texture_LoadRod(int legs){
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
Texture2D Texture_LoadSelBox(void){
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


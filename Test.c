// ============================================================================
// RODS
// Test
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Function for testing and debugging. Compiled and executed only when in 
    DEBUG_MODE.
*/


// ============================================================================ DEPENDENCIES

// Included in main.c, only in DEBUG_MODE

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "Mods/Assets/Assets.h"

#include "Mods/Public/Public.h"
#include "Mods/Fund/Fund.h"
#include "Mods/Logic/Logic.h"
#include "Mods/Graph/Graph.h"
#include "Mods/Store/Store.h"
#include "Mods/GUI/GUI.h"
#include "Mods/Gadgets/Gadgets.h"
#include "Mods/Pages/Pages.h"


// ============================================================================ TEST


// Function for testing and debugging
int Test(UNUSED int argc, UNUSED char** argv){

    Window_Init();

    Font_LoadDefault();
    Texture_LoadAll();

    Router* router = Router_Make();

    Router_AddPage(router, MainPage_Make());

    Router_ShowPage(router, PAGE_MAIN, WITH_ANIM);

    Router_Loop(router);
    
    router = Router_Free(router);
    Font_UnloadDefault();
    Texture_UnloadAll();

    Window_Close();
    
    return 0;
}






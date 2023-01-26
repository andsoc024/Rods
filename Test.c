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

    Records_MakeDefault();
    Records_Set(Glo_Records, TIME(0, 1, 12), 10, 10);

    Router* router = Router_Make();

    Router_AddPage(router, MainPage_Make());
    Router_AddPage(router, GamePage_Make(NULL));
    Router_AddPage(router, SetupPage_Make());
    Router_AddPage(router, HelpPage_Make());
    Router_AddPage(router, InfoPage_Make());

    Router_ShowPage(router, EVENT_NULL, PAGE_MAIN, WITH_ANIM);

    Router_Loop(router);
    
    router = Router_Free(router);
    Font_UnloadDefault();
    Texture_UnloadAll();
    Records_FreeDefault();

    Window_Close();
    
    return 0;
}






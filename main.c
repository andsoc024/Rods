// ============================================================================
// RODS
// Main
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    The main function of the program.
*/


// ============================================================================ DEPENDENCIES

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
#include "Mods/Sound/Sound.h"

#ifdef DEBUG_MODE
    #include "Test.c"
#endif

// ============================================================================ MAIN

// The main function of the programn
int main(UNUSED int argc, UNUSED char** argv){
    
    #ifdef DEBUG_MODE
        int res = Test(argc, argv);
        #ifdef MEMORY_TRACK
            Memory_Print();
        #endif
        return res;
    #endif

    Window_Init();

    Font_LoadDefault();
    Texture_LoadAll();
    Sound_LoadAll();

    Records_MakeDefault();

    Glo_FilePath = Path_GetDataFile();
    PData* pData = PData_ReadFromFile();
    if (pData != NULL){
        if (pData->records != NULL){
            Records_Copy(Glo_Records, pData->records);
        }
        if (pData->sound){
            Sound_TurnOn();
        }else{
            Sound_TurnOn();
        }
    }

    Router* router = Router_Make();

    Router_AddPage(router, MainPage_Make());
    Router_AddPage(router, GamePage_Make(pData));
    Router_AddPage(router, SetupPage_Make());
    Router_AddPage(router, HelpPage_Make());
    Router_AddPage(router, InfoPage_Make());

    pData = PData_Free(pData);

    Router_ShowPage(router, EVENT_NULL, PAGE_MAIN, WITH_ANIM);
    Router_Loop(router);

    PData_WriteToFile(Glo_Records, 
                      GamePage_GetRGrid(router->pages[PAGE_GAME]), 
                      GamePage_GetSGraph(router->pages[PAGE_GAME]), 
                      GamePage_GetCurrentTime(router->pages[PAGE_GAME]), 
                      Glo_SoundData->soundOn);


    Glo_FilePath = Memory_Free(Glo_FilePath);
    router = Router_Free(router);
    Records_FreeDefault();
    Sound_UnloadAll();
    Texture_UnloadAll();
    Font_UnloadDefault();
    Window_Close();

    return 0;
}


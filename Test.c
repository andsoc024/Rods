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


// ============================================================================ TEST

Page* TestPage2_Make(E_PageID id);


// Function for testing and debugging
int Test(UNUSED int argc, UNUSED char** argv){

    Window_Init();

    Texture_LoadAll();
    Font_LoadDefault();

    Router* router = Router_Make();
    Router_AddPage(router, TestPage2_Make(PAGE_GENERIC_2));

    Router_ShowPage(router, PAGE_GENERIC_2, WITH_ANIM);

    Router_Loop(router);

    router = Router_Free(router);
    Texture_UnloadAll();
    Font_UnloadDefault();
    Window_Close();
    
    return 0;
}






void TestPage2_Resize(Page* page);
void TestPage2_ResizeAfterGadgets(Page* page);
void TestPage2_ReactToEvent(Page* page, Event event, EventQueue* queue);


Page* TestPage2_Make(E_PageID id){
    Page* page = Page_Make(id);

    const char* strings[] = {"1", "One", "2", "Two", "3", "Three"};

    Gadget* table = Table_Make(GDG_GENERIC_1, 2, 3, strings);
    Table_SetAlignment(table, RP_MIDDLE_LEFT, TABLE_COLUMN, 1);
    Page_AddGadget(page, table);

    Gadget* toolbar = Toolbar_Make();
    Page_AddGadget(page, toolbar);

    page->Resize = TestPage2_Resize;
    page->ResizeAfterGadgets = TestPage2_ResizeAfterGadgets;
    page->ReactToEvent = TestPage2_ReactToEvent;

    Page_Resize(page);

    return page;
}

void TestPage2_Resize(Page* page){
    page->gadgets[0]->cRect = Geo_AlignRect(TO_RECT(SIZE_SQR(500)), TO_RECT(Glo_WinSize), RP_CENTER);
}

void TestPage2_ResizeAfterGadgets(Page* page){
    Gadget* table = page->gadgets[0];
    Table_SetFontSize(table, Table_GetFontSize(table, 1, 0) * MATH_PHI_INVERSE, TABLE_COLUMN, 1);
    PRINT_LINE3
    Gadget_Print(page->gadgets[0]);
    PRINT_LINE3
}

void TestPage2_ReactToEvent(Page* page, Event event, UNUSED EventQueue* queue){
    switch (event.id){

        case EVENT_BUTTON_PRESSED:{
            if (event.source == GDG_BTN_BACK){
                Queue_AddEvent(queue, Event_SetAsUpdateSwitch(page->id, GDG_SWITCH_SOUND, !Switch_GetValue(page->gadgets[1]->subGadgets[TB_SWITCH_SOUND])));
            }
            break;
        }

        default: {break;}
    }
}




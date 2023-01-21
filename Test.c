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

Page* TestPage1_Make(void);
Page* TestPage2_Make(void);

bool electrifiedStatus = false;

// Function for testing and debugging
int Test(UNUSED int argc, UNUSED char** argv){

    Window_Init();

    Font_LoadDefault();
    Texture_LoadAll();

    Router* router = Router_Make();
    Router_AddPage(router, TestPage1_Make());
    Router_AddPage(router, TestPage2_Make());
    Router_ShowPage(router, PAGE_GENERIC_1, WITH_ANIM);

    Router_Loop(router);

    router = Router_Free(router);
    Texture_UnloadAll();
    Font_UnloadDefault();
    Window_Close();
    
    return 0;
}

void TestPage1_Resize(Page* page);
void TestPage1_ReactToEvent(Page* page, Event event, EventQueue* queue);

void TestPage2_Resize(Page* page);
void TestPage2_ReactToEvent(Page* page, Event event, EventQueue* queue);


Page* TestPage1_Make(void){
    Page* page = Page_Make(PAGE_GENERIC_1);

    Gadget* butt = Button_MakeAsText(GDG_GENERIC_1, "Electrify");
    Button_SetColors(butt, COL_UI_BG_PRIMARY, COL_ROD, Color_ChangeBrightness(COL_ROD, 10));
    Page_AddGadget(page, butt);

    Gadget* sw = Switch_Make(GDG_GENERIC_2, electrifiedStatus);
    Page_AddGadget(page, sw);
    PRINT_LINE3
    Gadget_Print(sw);
    PRINT_LINE3

    Gadget* butt2 = Button_MakeAsText(GDG_GENERIC_3, "To Page 2");
    Page_AddGadget(page, butt2);

    page->Resize       = TestPage1_Resize;
    page->ReactToEvent = TestPage1_ReactToEvent;

    Page_Resize(page);

    return page;
}

void TestPage1_Resize(Page* page){
    VGraph* vg = VGraph_Make(SIZE(1000, 1000), TO_RECT(Glo_WinSize), 10.0f);

    page->gadgets[0]->cRect = VGraph_ProjectRect(RECT(300, 200, 400, 100), vg);
    page->gadgets[1]->cRect = VGraph_ProjectRect(RECT(350, 400, 300, 100), vg);
    page->gadgets[2]->cRect = VGraph_ProjectRect(RECT(300, 700, 400, 100), vg);

    vg = VGraph_Free(vg);
}

void TestPage1_ReactToEvent(Page* page, Event event, EventQueue* queue){
    switch (event.id){
        case EVENT_BUTTON_PRESSED:{
            if (event.source == GDG_GENERIC_1){
                TOGGLE(electrifiedStatus)
                Button_SetColors(page->gadgets[0], COL_UI_BG_PRIMARY, electrifiedStatus ? COL_ELECTRIC_1 : COL_ROD, 
                                                                      electrifiedStatus ? COL_ELECTRIC_2 : Color_ChangeBrightness(COL_ROD, 10));
                Queue_RemoveForTarget(queue, GDG_GENERIC_2);
                Queue_AddEvent(queue, Event_SetAsUpdateSwitch(page->id, GDG_GENERIC_2, electrifiedStatus));
            }

            break;
        }

        case EVENT_BUTTON_RELEASED:{
            if (event.source == GDG_GENERIC_3){
                Queue_AddEvent(queue, Event_SetAsHidePage(PAGE_GENERIC_1, PAGE_GENERIC_1, WITH_ANIM));
                Queue_AddEvent(queue, Event_SetAsShowPage(PAGE_GENERIC_1, PAGE_GENERIC_2, WITH_ANIM));
                Queue_RemoveForTarget(queue, PAGE_GENERIC_2);
                Queue_AddEvent(queue, ((Event) {.id = EVENT_GENERIC, .source = PAGE_GENERIC_1, .target = PAGE_GENERIC_2}));
            }

            break;
        }

        case EVENT_SWITCH_CHANGED:{
            electrifiedStatus = event.data.switchValue;
            Button_SetColors(page->gadgets[0], COL_UI_BG_PRIMARY, electrifiedStatus ? COL_ELECTRIC_1 : COL_ROD, 
                                                                  electrifiedStatus ? COL_ELECTRIC_2 : Color_ChangeBrightness(COL_ROD, 10));
            break;
        }

        case EVENT_GENERIC:{
            if (event.target == PAGE_GENERIC_1){
                Button_SetColors(page->gadgets[0], COL_UI_BG_PRIMARY, electrifiedStatus ? COL_ELECTRIC_1 : COL_ROD, 
                                                                      electrifiedStatus ? COL_ELECTRIC_2 : Color_ChangeBrightness(COL_ROD, 10));
                Switch_SetValue(page->gadgets[1], electrifiedStatus, WITHOUT_ANIM);
            }
            break;
        }

        default: {break;}
    }
}



Page* TestPage2_Make(void){
    Page* page = Page_Make(PAGE_GENERIC_2);

    Gadget* sw = Switch_Make(GDG_GENERIC_4, electrifiedStatus);
    Page_AddGadget(page, sw);

    Gadget* butt = Button_MakeAsText(GDG_GENERIC_5, "To Page 1");
    Page_AddGadget(page, butt);

    page->Resize       = TestPage2_Resize;
    page->ReactToEvent = TestPage2_ReactToEvent;

    Page_Resize(page);

    return page;
}

void TestPage2_Resize(Page* page){
    VGraph* vg = VGraph_Make(SIZE(1000, 1000), TO_RECT(Glo_WinSize), 0.0f);

    page->gadgets[0]->cRect = VGraph_ProjectRect(RECT(200, 300, 600, 200), vg);
    page->gadgets[1]->cRect = VGraph_ProjectRect(RECT(300, 600, 400, 100), vg);

    vg = Memory_Free(vg);
}

void TestPage2_ReactToEvent(Page* page, Event event, EventQueue* queue){
    switch (event.id){
        case EVENT_GENERIC:{
            if (event.target == PAGE_GENERIC_2){
                Switch_SetValue(page->gadgets[0], electrifiedStatus, WITHOUT_ANIM);
            }
            break;
        }

        case EVENT_SWITCH_CHANGED:{
            if (event.source == GDG_GENERIC_4){
                electrifiedStatus = event.data.switchValue;
            }
            break;
        }

        case EVENT_BUTTON_RELEASED:{
            if (event.source == GDG_GENERIC_5){
                Queue_AddEvent(queue, Event_SetAsHidePage(PAGE_GENERIC_2, PAGE_GENERIC_2, WITH_ANIM));
                Queue_AddEvent(queue, Event_SetAsShowPage(PAGE_GENERIC_2, PAGE_GENERIC_1, WITH_ANIM));
                Queue_RemoveForTarget(queue, PAGE_GENERIC_1);
                Queue_AddEvent(queue, ((Event) {.id = EVENT_GENERIC, .source = PAGE_GENERIC_2, .target = PAGE_GENERIC_1}));
            }
            break;
        }

        default: {break;}
    }
}






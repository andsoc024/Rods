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

// Function for testing and debugging
int Test(UNUSED int argc, UNUSED char** argv){

    Window_Init();

    Texture_LoadAll();

    Router* router = Router_Make();
    Router_AddPage(router, TestPage1_Make());
    Router_AddPage(router, TestPage2_Make());
    Router_ShowPage(router, PAGE_GENERIC_1, WITH_ANIM);

    Router_Loop(router);

    router = Router_Free(router);
    Texture_UnloadAll();
    Window_Close();
    
    return 0;
}

void TestPage1_Resize(Page* page);
void TestPage1_ReactToEvent(Page* page, Event event, EventQueue* queue);

void TestPage2_Resize(Page* page);
void TestPage2_ReactToEvent(Page* page, Event event, EventQueue* queue);


Gadget* TButton_Make(E_GadgetID id);
void TButton_ReactToEvent(Gadget* gadget, Event event, EventQueue* queue);
void TButton_Draw(const Gadget* gadget, Vector2 shift);


Page* TestPage1_Make(void){
    Page* page = Page_Make(PAGE_GENERIC_1);

    Gadget* label = IconLabel_Make(GDG_GENERIC_1, ICON_MEDAL, COL_UI_FG_PRIMARY, RP_CENTER);
    Page_AddGadget(page, label);

    PRINT_LINE3
    Gadget_Print(label);
    PRINT_LINE3

    Gadget* butt1 = TButton_Make(GDG_GENERIC_2);
    Page_AddGadget(page, butt1);

    Gadget* butt2 = TButton_Make(GDG_GENERIC_3);
    Page_AddGadget(page, butt2);


    page->Resize       = TestPage1_Resize;
    page->ReactToEvent = TestPage1_ReactToEvent;

    Page_Resize(page);

    return page;
}

void TestPage1_Resize(Page* page){
    const Size vScreen = SIZE(450, 700);
    const Rect labelRect = RECT(75, 50, 300, 300);
    const Rect butt1Rect = RECT(50, 400, 350, 100);
    const Rect butt2Rect = RECT(50, 550, 350, 100);

    VGraph* vg = VGraph_Make(vScreen, TO_RECT(Glo_WinSize), 10.0f);

    page->gadgets[0]->cRect = VGraph_ProjectRect(labelRect, vg);
    page->gadgets[1]->cRect = VGraph_ProjectRect(butt1Rect, vg);
    page->gadgets[2]->cRect = VGraph_ProjectRect(butt2Rect, vg);

    vg = VGraph_Free(vg);
}

void TestPage1_ReactToEvent(Page* page, Event event, EventQueue* queue){
    switch (event.id){
        case EVENT_BUTTON_RELEASED:{
            switch (event.source){
                case GDG_GENERIC_2:{
                    IconLabel_SetIcon(page->gadgets[0], (IconLabel_GetIconID(page->gadgets[0]) + 1) % ICONS_N);
                    if (IconLabel_GetSize(page->gadgets[0]) > 200.0f){
                        IconLabel_SetSize(page->gadgets[0], 200.0f);
                    }
                    IconLabel_SetColor(page->gadgets[0], RED);
                    break;
                }

                case GDG_GENERIC_3:{
                    Queue_AddEvent(queue, Event_SetAsShowPage(page->id, PAGE_GENERIC_2, WITH_ANIM));
                    Queue_AddEvent(queue, Event_SetAsHidePage(page->id, PAGE_GENERIC_1, WITH_ANIM));
                    break;
                }

                default: {break;}
            }
        }

        default: {break;}
    }
}



Page* TestPage2_Make(void){
    Page* page = Page_Make(PAGE_GENERIC_2);

    Gadget* butt = TButton_Make(GDG_GENERIC_4);
    Page_AddGadget(page, butt);

    page->Resize       = TestPage2_Resize;
    page->ReactToEvent = TestPage2_ReactToEvent;

    Page_Resize(page);

    return page;
}

void TestPage2_Resize(Page* page){
    const Size vScreen = SIZE(100, 100);
    const Rect buttRect = RECT(20, 20, 60, 60);

    VGraph* vg = VGraph_Make(vScreen, TO_RECT(Glo_WinSize), 10.0f);

    page->gadgets[0]->cRect = VGraph_ProjectRect(buttRect, vg);

    vg = VGraph_Free(vg);
}

void TestPage2_ReactToEvent(Page* page, Event event, EventQueue* queue){
    if (event.id == EVENT_BUTTON_RELEASED && event.source == GDG_GENERIC_4){
        Queue_AddEvent(queue, Event_SetAsShowPage(page->id, PAGE_GENERIC_1, WITH_ANIM));
        Queue_AddEvent(queue, Event_SetAsHidePage(page->id, PAGE_GENERIC_2, WITH_ANIM));
    }
}





Gadget* TButton_Make(E_GadgetID id){
    Gadget* butt = Gadget_Make(id, GT_GENERIC, IS_SELECTABLE, 0);

    butt->ReactToEvent = TButton_ReactToEvent;
    butt->Draw = TButton_Draw;

    return butt;
}

void TButton_ReactToEvent(Gadget* gadget, Event event, EventQueue* queue){
    switch (event.id){
        case EVENT_MOUSE_MOVE:{
            gadget->isSelected = Geo_PointIsInRect(event.data.mouse.pos, gadget->cRect);
            break;
        }

        case EVENT_MOUSE_DRAG:{
            if (!Geo_PointIsInRect(event.data.mouse.pos, gadget->cRect)){
                gadget->isPressed = false;
            }
            break;
        }

        case EVENT_MOUSE_PRESSED:{
            if (Geo_PointIsInRect(event.data.mouse.pos, gadget->cRect)){
                gadget->isPressed = true;
                Queue_AddEvent(queue, Event_SetAsButtonPressed(gadget->id));
            }
            break;
        }

        case EVENT_MOUSE_RELEASED:{
            if (gadget->isPressed){
                gadget->isPressed = false;
                Queue_AddEvent(queue, Event_SetAsButtonReleased(gadget->id));
            }
            break;
        }

        default: {break;}
    }
}

void TButton_Draw(const Gadget* gadget, Vector2 shift){
    Color color = gadget->isPressed ? COL_UI_BG_PRIMARY : 
                  gadget->isSelected ? COL_UI_FG_EMPH : COL_UI_FG_PRIMARY;
    Shape_DrawOutlinedRect(Geo_TranslateRect(gadget->cRect, shift), 5.0f, COL_UI_BG_PRIMARY, color);
}



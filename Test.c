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


// ============================================================================ TEST

Page* TestPage1_Make(void);
Page* TestPage2_Make(void);

// Function for testing and debugging
int Test(UNUSED int argc, UNUSED char** argv){

    Window_Init();

    Router* router = Router_Make();
    Router_AddPage(router, TestPage1_Make());
    Router_AddPage(router, TestPage2_Make());
    Router_ShowPage(router, PAGE_GENERIC_1, WITH_ANIM);

    PRINT_LINE3
    Router_Print(router);
    PRINT_LINE3

    Router_Loop(router);

    router = Router_Free(router);

    Window_Close();
    
    return 0;
}

void TestPage1_Resize(Page* page);
void TestPage1_ReactToEvent(Page* page, Event event, EventQueue* queue);

void TestPage2_Resize(Page* page);
void TestPage2_ReactToEvent(Page* page, Event event, EventQueue* queue);

typedef struct CircleData{
    Point center;
    float radius;
    Color color;
}CircleData;
Gadget* Circle_Make(E_GadgetID id, Color color);
void Circle_Resize(Gadget* gadget);
void Circle_Draw(Gadget* gadget, Vector2 shift);

Gadget* TButton_Make(E_GadgetID id);
void TButton_ReactToEvent(Gadget* gadget, Event event, EventQueue* queue);
void TButton_Draw(Gadget* gadget, Vector2 shift);


Page* TestPage1_Make(void){
    Page* page = Page_Make(PAGE_GENERIC_1);

    Gadget* circle = Circle_Make(GDG_GENERIC_1, Color_Random(0x33, 0xEE));
    Page_AddGadget(page, circle);

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
    const Rect circleRect = RECT(75, 50, 300, 300);
    const Rect butt1Rect = RECT(50, 400, 350, 100);
    const Rect butt2Rect = RECT(50, 550, 350, 100);

    VGraph* vg = VGraph_Make(vScreen, TO_RECT(Glo_WinSize), 10.0f);

    page->gadgets[0]->cRect = VGraph_ProjectRect(circleRect, vg);
    page->gadgets[1]->cRect = VGraph_ProjectRect(butt1Rect, vg);
    page->gadgets[2]->cRect = VGraph_ProjectRect(butt2Rect, vg);

    vg = VGraph_Free(vg);
}

void TestPage1_ReactToEvent(Page* page, Event event, EventQueue* queue){
    switch (event.id){
        case EVENT_BUTTON_RELEASED:{
            switch (event.source){
                case GDG_GENERIC_2:{
                    ((CircleData*) page->gadgets[0]->data)->color = Color_Random(0x33, 0xEE);
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


Gadget* Circle_Make(E_GadgetID id, Color color){
    Gadget* circle = Gadget_Make(id, GT_GENERIC, IS_NOT_SELECTABLE, 0);

    circle->Resize = Circle_Resize;
    circle->Draw = Circle_Draw;

    CircleData* data = Memory_Allocate(NULL, sizeof(CircleData), ZEROVAL_ALL);
    data->color = color;
    circle->data = data;

    return circle;
}

void Circle_Resize(Gadget* gadget){
    ((CircleData*) gadget->data)->center = Geo_RectPoint(gadget->cRect, RP_CENTER);
    ((CircleData*) gadget->data)->radius = MIN_DIM(gadget->cRect) * 0.5f;
}

void Circle_Draw(Gadget* gadget, Vector2 shift){
    DrawCircleV(Geo_TranslatePoint(((CircleData*) gadget->data)->center, shift), 
                ((CircleData*) gadget->data)->radius, ((CircleData*) gadget->data)->color);
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

void TButton_Draw(Gadget* gadget, Vector2 shift){
    Color color = gadget->isPressed ? COL_UI_BG_PRIMARY : 
                  gadget->isSelected ? COL_UI_FG_EMPH : COL_UI_FG_PRIMARY;
    Shape_DrawOutlinedRect(Geo_TranslateRect(gadget->cRect, shift), 5.0f, COL_UI_BG_PRIMARY, color);
}



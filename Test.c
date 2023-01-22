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

Page* TestPage1_Make(E_PageID id);
Page* TestPage2_Make(E_PageID id);

// Function for testing and debugging
int Test(UNUSED int argc, UNUSED char** argv){

    Window_Init();

    Texture_LoadAll();
    Font_LoadDefault();

    Router* router = Router_Make();
    Router_AddPage(router, TestPage1_Make(PAGE_GENERIC_1));

    Router_ShowPage(router, PAGE_GENERIC_1, WITH_ANIM);

    Router_Loop(router);

    router = Router_Free(router);
    Texture_UnloadAll();
    Font_UnloadDefault();
    Window_Close();
    
    return 0;
}




typedef struct Circle{
    Point center;
    float radius;
    Color color;
}Circle;

typedef struct TestPage1_Data{
    Circle* circles;
    int nCircles;
    SGraph* sg;
    VGraph* vg;
}TestPage1_Data;

#define TP1DATA ((TestPage1_Data*) page->data)

void TestPage1_PrepareToFree(Page* page);
void TestPage1_Resize(Page* page);
void TestPage1_ReactToEvent(Page* page, Event event, EventQueue* queue);
void TestPage1_Draw(const Page* page);

Page* TestPage1_Make(E_PageID id){
    Page* page = Page_Make(id);

    Size vScreen = SIZE(1000, 1000);

    int nCircles = 200;
    Circle* circles = Memory_Allocate(NULL, sizeof(Circle) * nCircles, ZEROVAL_ALL);
    for (int i = 0; i < nCircles; i++){
        circles[i].radius = Math_RandomFloat(7.0f, 25.0f);
        circles[i].center = Geo_RandomPoint(Geo_ApplyRectMargins(TO_RECT(vScreen), circles[i].radius));
        circles[i].color = Color_SetAlpha(Color_Random(0x33, 0xEE), 0x77);
    }

    SGraph* sg = SGraph_Make(vScreen, TO_RECT(Glo_WinSize), 0.0f, Geo_ScaleSize(vScreen, 0.01f), 
                                                                  Geo_ScaleSize(vScreen, 100.0f));
    VGraph* vg = VGraph_Make(vScreen, TO_RECT(SGraph_GetVScreen(sg)), 0.0f);
    

    Gadget* sbarHor = Scrollbar_Make(GDG_GENERIC_1, OR_HORISONTAL);
    sbarHor->cRect = GDG_DEF_RECT;
    Gadget_Resize(sbarHor);
    Scrollbar_SetSizeFromRatio(sbarHor, SGraph_CalcViewportSizeRatio(sg, OR_HORISONTAL));
    Scrollbar_SetPosFromRatio(sbarHor, SGraph_CalcViewportPosRatio(sg, OR_HORISONTAL));
    Page_AddGadget(page, sbarHor);

    Gadget* sbarVer = Scrollbar_Make(GDG_GENERIC_2, OR_VERTICAL);
    sbarVer->cRect = GDG_DEF_RECT;
    Gadget_Resize(sbarVer);
    Scrollbar_SetSizeFromRatio(sbarVer, SGraph_CalcViewportSizeRatio(sg, OR_VERTICAL));
    Scrollbar_SetPosFromRatio(sbarVer, SGraph_CalcViewportPosRatio(sg, OR_VERTICAL));
    Page_AddGadget(page, sbarVer);

    page->PrepareToFree = TestPage1_PrepareToFree;
    page->Resize = TestPage1_Resize;
    page->ReactToEvent = TestPage1_ReactToEvent;
    page->Draw = TestPage1_Draw;

    TestPage1_Data* data = Memory_Allocate(NULL, sizeof(TestPage1_Data), ZEROVAL_ALL);
    data->circles = circles;
    data->nCircles = nCircles;
    data->sg = sg;
    data->vg = vg;
    page->data = data;

    Page_Resize(page);

    return page;
}


void TestPage1_PrepareToFree(Page* page){
    TP1DATA->sg = SGraph_Free(TP1DATA->sg);
    TP1DATA->vg = VGraph_Free(TP1DATA->vg);
    TP1DATA->circles = Memory_Free(TP1DATA->circles);
}

void TestPage1_Resize(Page* page){
    page->gadgets[0]->cRect = RECT(100, Glo_WinSize.height - 30, Glo_WinSize.width - 200, 30);
    page->gadgets[1]->cRect = RECT(Glo_WinSize.width - 30, 100, 30, Glo_WinSize.height - 200);

    SGraph_SetView(TP1DATA->sg, TO_RECT(Glo_WinSize));
    VGraph_Resize(TP1DATA->vg, TO_RECT(SGraph_GetVScreen(TP1DATA->sg)));
    Scrollbar_SetSizeFromRatio(page->gadgets[0], SGraph_CalcViewportSizeRatio(TP1DATA->sg, OR_HORISONTAL));
    Scrollbar_SetPosFromRatio(page->gadgets[0], SGraph_CalcViewportPosRatio(TP1DATA->sg, OR_HORISONTAL));
    Scrollbar_SetSizeFromRatio(page->gadgets[1], SGraph_CalcViewportSizeRatio(TP1DATA->sg, OR_VERTICAL));
    Scrollbar_SetPosFromRatio(page->gadgets[1], SGraph_CalcViewportPosRatio(TP1DATA->sg, OR_VERTICAL));
}

void TestPage1_ReactToEvent(Page* page, Event event, EventQueue* queue){
    switch (event.id){
        case EVENT_KEY_PRESSED:{
            switch (event.data.key){
                case WKEY_RIGHT: case WKEY_DOWN: case WKEY_LEFT: case WKEY_UP:{
                    E_Direction dir = Direction_FromKey(event.data.key);
                    SGraph_MoveViewportToDir(TP1DATA->sg, dir, MIN_DIM(Glo_WinSize) * 0.07f);
                    Queue_AddEvent(queue, Event_SetAsUpdateScrollbar(page->id, GDG_GENERIC_1, 
                                                                     SGraph_CalcViewportPosRatio(TP1DATA->sg, OR_HORISONTAL), 
                                                                     SGraph_CalcViewportSizeRatio(TP1DATA->sg, OR_HORISONTAL)));
                    Queue_AddEvent(queue, Event_SetAsUpdateScrollbar(page->id, GDG_GENERIC_2, 
                                                                     SGraph_CalcViewportPosRatio(TP1DATA->sg, OR_VERTICAL), 
                                                                     SGraph_CalcViewportSizeRatio(TP1DATA->sg, OR_VERTICAL)));   
                    break;                           
                }

                case WKEY_PLUS: case WKEY_MINUS:{
                    float zoom = 1.1f;
                    zoom = (event.data.key == WKEY_PLUS) ? zoom : 1.0f / zoom;
                    SGraph_Zoom(TP1DATA->sg, zoom);
                    VGraph_Resize(TP1DATA->vg, TO_RECT(SGraph_GetVScreen(TP1DATA->sg)));
                    Queue_AddEvent(queue, Event_SetAsUpdateScrollbar(page->id, GDG_GENERIC_1, 
                                                                     SGraph_CalcViewportPosRatio(TP1DATA->sg, OR_HORISONTAL), 
                                                                     SGraph_CalcViewportSizeRatio(TP1DATA->sg, OR_HORISONTAL)));
                    Queue_AddEvent(queue, Event_SetAsUpdateScrollbar(page->id, GDG_GENERIC_2, 
                                                                     SGraph_CalcViewportPosRatio(TP1DATA->sg, OR_VERTICAL), 
                                                                     SGraph_CalcViewportSizeRatio(TP1DATA->sg, OR_VERTICAL))); 
                    break;
                }

                default: {break;}
            }

            break;
        }

        case EVENT_SCROLLBAR_CHANGED:{
            switch (event.source){
                case GDG_GENERIC_1:{
                    SGraph_SetViewportPosFromRatio(TP1DATA->sg, event.data.scrollbar.posRatio, OR_HORISONTAL);
                    break;
                }

                case GDG_GENERIC_2:{
                    SGraph_SetViewportPosFromRatio(TP1DATA->sg, event.data.scrollbar.posRatio, OR_VERTICAL);
                    break;
                }

                default: {break;}
            }
        }

        default: {break;}
    }
}

void TestPage1_Draw(const Page* page){
    for (int i = 0; i < TP1DATA->nCircles; i++){
        DrawCircleV(SGraph_ProjectPoint(VGraph_ProjectPoint(TP1DATA->circles[i].center, TP1DATA->vg), TP1DATA->sg), 
                    VGraph_ProjectLength(TP1DATA->circles[i].radius, TP1DATA->vg), 
                    TP1DATA->circles[i].color);
    }
}






void TestPage2_Resize(Page* page);
void TestPage2_ReactToEvent(Page* page, Event event, EventQueue* queue);


Page* TestPage2_Make(E_PageID id){
    Page* page = Page_Make(id);

    Gadget* timer = Timer_Make(GDG_GENERIC_7, TIME(0, 59, 40), TIME(1,20,30));
    Page_AddGadget(page, timer);

    PRINT_LINE3
    Gadget_Print(timer);
    PRINT_LINE3

    page->Resize = TestPage2_Resize;
    page->ReactToEvent = TestPage2_ReactToEvent;

    Page_Resize(page);

    return page;
}

void TestPage2_Resize(Page* page){
    page->gadgets[0]->cRect = Geo_AlignRect(TO_RECT(SIZE_SQR(500)), TO_RECT(Glo_WinSize), RP_CENTER);
}

void TestPage2_ReactToEvent(Page* page, Event event, UNUSED EventQueue* queue){
    switch (event.id){
        case EVENT_KEY_PRESSED:{
            if (event.data.key == WKEY_W){
                Timer_Toggle(page->gadgets[0]);
            }

            if (event.data.key == WKEY_ENTER){
                if (Time_IsValid(Timer_GetTime(page->gadgets[0]))){
                    Timer_SetTime(page->gadgets[0], TIME_INVALID);
                }else{
                    Timer_SetTime(page->gadgets[0], TIME_NULL);
                }
            }

            if (event.data.key == WKEY_SPACE){
                TOGGLE(page->gadgets[0]->isExpanded);
            }

            break;
        }

        default: {break;}
    }
}




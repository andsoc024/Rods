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

// Function for testing and debugging
int Test(UNUSED int argc, UNUSED char** argv){

    EventQueue* queue = Queue_Make();

    Event event = Event_SetAsUpdateScrollbar(GDG_BOARD, GDG_SCROLLBAR_HOR, 0.1f, 0.3f);
    Queue_AddEvent(queue, event);

    event = Event_SetAsSwitchChanged(GDG_SWITCH_SOUND, true);
    Queue_AddEvent(queue, event);

    event = Event_SetAsMousePressed(POINT(300, 100));
    Queue_SetUserInput(queue, event);

    Queue_Print(queue);

    PRINT_LINE3

    while ((event = Queue_GetNext(queue)).id != EVENT_NONE){
        Event_Print(event, WITH_NEW_LINE);
    }

    PRINT_LINE3

    Queue_Print(queue);

    PRINT_LINE3

    while ((event = Queue_GetNext(queue)).id != EVENT_NONE){
        Event_Print(event, WITH_NEW_LINE);
        if (event.target == GDG_SCROLLBAR_HOR){
            Queue_RemoveForTarget(queue, GDG_SCROLLBAR_HOR);
        }
    }

    PRINT_LINE3

    Queue_Print(queue);

    queue = Queue_Free(queue);

    return 0;
}



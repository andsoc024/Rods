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

    Event event = Event_SetAsUpdateScrollbar(GDG_BOARD, GDG_SCROLLBAR_HOR, 0.1f, 0.3f);
    Event_Print(event, WITH_NEW_LINE);

    return 0;
}



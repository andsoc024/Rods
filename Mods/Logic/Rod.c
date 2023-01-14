// ============================================================================
// RODS
// Rod
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Function for manipulating rods.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"
#include "../Fund/Fund.h"
#include "Logic.h"


// ============================================================================ FUNC DEF

// **************************************************************************** Rod_Set

// Set the rod as unelectrified and not rotating and with the given legs
void Rod_Set(Rod* rod, int legs){
    *rod = ROD_NULL;
    rod->legs = PUT_IN_RANGE(legs, 0x0, 0xF);
}


// **************************************************************************** Rod_HasLegToDir

// Return true if the rod has a leg to the given direction
bool Rod_HasLegToDir(Rod* rod, E_Direction dir){
    int legDir = Direction_ToLegDir(dir);
    return rod->legs & legDir;
}


// **************************************************************************** Rod_IsConnectedToRod

// Return true if the rod is connected to the adjacent rod towards the given 
// direction
bool Rod_IsConnectedToRod(Rod* rod1, Rod* rod2, E_Direction dir){
    if (Rod_IsAnimating(rod1) || Rod_IsAnimating(rod2)){
        return false;
    }

    E_Direction opp = Direction_Opposite(dir);

    return (rod1->legs & Direction_ToLegDir(dir)) && 
           (rod2->legs & Direction_ToLegDir(opp));
}


// **************************************************************************** Rod_IsAnimating

// Return true if the rod is animating
bool Rod_IsAnimating(Rod* rod){
    return rod->frame > 0;
}


// **************************************************************************** Rod_Rotate

// Rotate the rod by 90° clockwise x times, optionally with animation
void Rod_Rotate(Rod* rod, int times, bool withAnim){
    rod->legs = Direction_RotateLegs(rod->legs, times);

    if (withAnim && times > 0){
        rod->frame = ROD_ROT_FRAMES_N - 1;
    }
}


// **************************************************************************** Rod_Update

// Update the rod animation, if any. If an animation is just completed, return 
// true (COMPLETED), otherwise false (NOT_COMPLETED)
bool Rod_Update(Rod* rod){
    if (rod->frame == 0){
        return NOT_COMPLETED;
    }

    rod->frame--;

    return rod->frame == 0;
}


// **************************************************************************** Rod_FinishAnim

// Complete a rotation animation, if any. If an animation is completed return 
// true (COMPLETED), otherwise false (NOT_COMPLETED)
bool Rod_FinishAnim(Rod* rod){
    if (rod->frame == 0){
        return NOT_COMPLETED;
    }

    rod->frame = 0;

    return COMPLETED;
}


#ifdef DEBUG_MODE
// **************************************************************************** Rod_Print

    // Single line print of the rod parameters. Optionally with new line
    void Rod_Print(Rod* rod, bool withNewLine){
        CHECK_NULL(rod, withNewLine)

        Direction_PrintLegs(rod->legs, WITHOUT_NEW_LINE);
        printf(" | El: %s", Bool_ToString(rod->isElectrified, LONG_FORM));
        printf(" | Frame: %d", rod->frame);
        
        if (withNewLine) {printf("\n");}
    }
#endif


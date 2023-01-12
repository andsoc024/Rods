// ============================================================================
// RODS
// Direction
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions for managing directions.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"
#include "Fund.h"


// ============================================================================ FUNC DEF

// **************************************************************************** Direction_Random

// Return a random direction, optionally including DIR_NONE
E_Direction Direction_Random(bool includeNone){
    int startDir = includeNone ? DIR_NONE : DIR_RIGHT;
    return Math_RandomInt(startDir, DIR_UP);
}


// **************************************************************************** Direction_Rotate

// Rotate the direction by 90° x times
E_Direction Direction_Rotate(E_Direction dir, int times){
    if (!Direction_IsValid(dir, INCLUDE_NOT_NONE)){
        return DIR_NONE;
    }

    while (times < 0) {times += 4;}

    return (dir - 1 + times) % 4 + 1;
}


// **************************************************************************** Direction_Opposite

// The opposite direction
E_Direction Direction_Opposite(E_Direction dir){
    return Direction_Rotate(dir, 2);
}


// **************************************************************************** Direction_FromKey

// The direction corresponding to the arrow or WASD key
E_Direction Direction_FromKey(KeyboardKey key){
    switch (key){
        case WKEY_RIGHT: case WKEY_D: {return DIR_RIGHT;}
        case WKEY_DOWN:  case WKEY_S: {return DIR_DOWN;}
        case WKEY_LEFT:  case WKEY_A: {return DIR_LEFT;}
        case WKEY_UP:    case WKEY_W: {return DIR_UP;}
        default: {break;}
    }

    return DIR_NONE;
}


// **************************************************************************** Direction_FromLegDir

// Turn the binary leg direction to direction
E_Direction Direction_FromLegDir(int legDir){
    switch (legDir){
        case LEGDIR_RIGHT: {return DIR_RIGHT;}
        case LEGDIR_DOWN:  {return DIR_DOWN;}
        case LEGDIR_LEFT:  {return DIR_LEFT;}
        case LEGDIR_UP:    {return DIR_UP;}
        default: {break;}
    }

    return DIR_NONE;
}


// **************************************************************************** Direction_ToLegDir

// Turn the direction to binary leg direction
int Direction_ToLegDir(E_Direction dir){
    if (!Direction_IsValid(dir, INCLUDE_NOT_NONE)){
        return LEGDIR_NONE;
    }

    return (1 << (dir - 1));
}


// **************************************************************************** Direction_IsValidLegsValue

// Return true if the legs value is valid (0-15)
bool Direction_IsValidLegsValue(int legs){
    return IS_IN_RANGE(legs, 0x0, 0xF);
}


// **************************************************************************** Direction_RotateLegs

// Rotate the legs by 90° clockwise x times
int Direction_RotateLegs(int legs, int times){
    if (!Direction_IsValidLegsValue(legs)){
        return LEGDIR_NONE;
    }

    while (times < 0) {times += 4;}

    int res = legs;
    for (; times > 0; times--){
        res = ((legs << 1) & 0xF);
        res += ((legs & LEGDIR_UP) ? 1 : 0);
        legs = res;
    }
    
    return res;
}


#ifdef DEBUG_MODE
// **************************************************************************** Direction_PrintLegs

    // Single line print of the legs in the form RDLU. Optionally with new line
    void Direction_PrintLegs(int legs, bool withNewLine){
        bool isValid = Direction_IsValidLegsValue(legs);

        for (int dir = DIR_RIGHT; dir <= DIR_UP; dir++){
            int legDir = Direction_ToLegDir(dir);
            if (isValid && (legs & legDir)){
                printf("%s", Direction_ToString(dir, SHORT_FORM));
            }else{
                printf("%s", STR_INVALID_SHORT);
            }
        }

        if (withNewLine) {printf("\n");}
    }
#endif




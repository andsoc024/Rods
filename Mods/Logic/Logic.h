// ============================================================================
// RODS
// Logic Header
// by Andreas Socratous
// Jan 2023
// ============================================================================


#ifndef LOGIC_GUARD
#define LOGIC_GUARD


// ============================================================================ INFO
/*
    Functions for managing Rods and the Rod Grid as well as the Records, that 
    store record times.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"


// ============================================================================ FUNC DECL

// ---------------------------------------------------------------------------- Rod Functions

void            Rod_Set(Rod* rod, int legs);
bool            Rod_HasLegToDir(Rod* rod, E_Direction dir);
bool            Rod_IsConnectedToRod(Rod* rod1, Rod* rod2, E_Direction dir);
bool            Rod_IsAnimating(Rod* rod);
void            Rod_Rotate(Rod* rod, int times, bool withAnim);
bool            Rod_Update(Rod* rod);
bool            Rod_FinishAnim(Rod* rod);
#ifdef DEBUG_MODE
    void        Rod_Print(Rod* rod, bool withNewLine);
#endif



#endif // LOGIC_GUARD


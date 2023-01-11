// ============================================================================
// RODS
// Logic Header
// by Andreas Socratous
// Dec 2022
// ============================================================================


#ifndef LOGIC_GUARD
#define LOGIC_GUARD


// ============================================================================ INFO
/*
    Functions for managing rods and the rod grid, as well as the Records 
    structure.
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
void            Rod_Rotate(Rod* rod, int times, bool withAnim);
bool            Rod_IsConnectedToRod(Rod* rod1, Rod* rod2, E_Direction dir);
bool            Rod_Update(Rod* rod);
bool            Rod_FinishAnim(Rod* rod);
bool            Rod_IsAnimating(Rod* rod);
#ifdef DEBUG_MODE
    void        Rod_Print(Rod* rod, bool withNewLine);
#endif

// ---------------------------------------------------------------------------- Rod Grid Functions

RGrid*          RGrid_MakeEmpty(int nCols, int nRows);
RGrid*          RGrid_Free(RGrid* rGrid);
void            RGrid_Clear(RGrid* rGrid);
void            RGrid_CreateRandom(RGrid* rGrid);
void            RGrid_Shuffle(RGrid* rGrid);
void            RGrid_Electrify(RGrid* rGrid, GNode start);
void            RGrid_Deelectrify(RGrid* rGrid);
void            RGrid_Reelectrify(RGrid* rGrid);
void            RGrid_Rotate(RGrid* rGrid, GNode node);
void            RGrid_Update(RGrid* rGrid);
void            RGrid_FinishAnim(RGrid* rGrid);
bool            RGrid_IsAnimating(RGrid* rGrid);
void            RGrid_AddToUpdatable(RGrid* rGrid, GNode node);
int             RGrid_UnelectrifiedN(RGrid* rGrid);
bool            RGrid_IsCompleted(RGrid* rGrid);
#ifdef DEBUG_MODE
    void        RGrid_Print(RGrid* rGrid);
#endif

// ---------------------------------------------------------------------------- Records Functions

Records*        Records_Make(void);
Records*        Records_Free(Records* records);
ElTime          Records_Get(const Records* records, int nCols, int nRows);
bool            Records_Set(Records* records, int nCols, int nRows, ElTime t);
bool            Records_TimeIsSmaller(Records* records, int nCols, int nRows, ElTime t);
int             Records_N(const Records* records);
void            Records_MakeDefault(void);
void            Records_FreeDefault(void);
#ifdef DEBUG_MODE
    void        Records_Print(Records* records);
#endif



#endif // LOGIC_GUARD


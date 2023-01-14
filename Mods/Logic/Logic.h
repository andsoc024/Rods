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
bool            Rod_HasLegToDir(const Rod* rod, E_Direction dir);
bool            Rod_IsConnectedToRod(const Rod* rod1, const Rod* rod2, E_Direction dir);
bool            Rod_IsAnimating(const Rod* rod);
void            Rod_Rotate(Rod* rod, int times, bool withAnim);
bool            Rod_Update(Rod* rod);
bool            Rod_FinishAnim(Rod* rod);
#ifdef DEBUG_MODE
    void        Rod_Print(const Rod* rod, bool withNewLine);
#endif

// ---------------------------------------------------------------------------- Rod Grid Functions

RGrid*          RGrid_MakeEmpty(int nCols, int nRows);
RGrid*          RGrid_Copy(RGrid* dst, const RGrid* src);
RGrid*          RGrid_Free(RGrid* rGrid);
void            RGrid_Clear(RGrid* rGrid);
void            RGrid_CreateRandom(RGrid* rGrid);
void            RGrid_SetSize(RGrid* rGrid, int nCols, int nRows);
void            RGrid_Shuffle(RGrid* rGrid);
void            RGrid_Electrify(RGrid* rGrid, GNode start);
void            RGrid_Deelectrify(RGrid* rGrid);
void            RGrid_Reelectrify(RGrid* rGrid);
void            RGrid_RotateRod(RGrid* rGrid, GNode node);
void            RGrid_Update(RGrid* rGrid);
void            RGrid_FinishAnim(RGrid* rGrid);
Grid            RGrid_GetSize(const RGrid* rGrid);
GNode           RGrid_GetSource(const RGrid* rGrid);
Rod*            RGrid_GetRod(const RGrid* rGrid, GNode node);
Rod*            RGrid_GetRod_Fast(const RGrid* rGrid, GNode node);
void            RGrid_SetRod(RGrid* rGrid, GNode node, int legs);
bool            RGrid_IsAnimating(const RGrid* rGrid);
int             RGrid_GetTotal(const RGrid* rGrid);
int             RGrid_GetNumElectrified(const RGrid* rGrid);
int             RGrid_GetNumUnelectrified(const RGrid* rGrid);
#ifdef DEBUG_MODE
    void        RGrid_Print(const RGrid* rGrid);
#endif

// ---------------------------------------------------------------------------- Records Functions

Records*        Records_Make(void);
Records*        Records_Copy(Records* dst, const Records* src);
Records*        Records_Free(Records* records);
void            Records_Clear(Records* records);
Time            Records_Get(const Records* records, int nCols, int nRows);
void            Records_Set(Records* records, Time t, int nCols, int nRows);
int             Records_N(const Records* records);
void            Records_MakeDefault(void);
void            Records_FreeDefault(void);
#ifdef DEBUG_MODE
    void        Records_Print(const Records* records);
#endif


#endif // LOGIC_GUARD


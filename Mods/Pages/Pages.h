// ============================================================================
// RODS
// Pages Header
// by Andreas Socratous
// Jan 2023
// ============================================================================


#ifndef PAGES_GUARD
#define PAGES_GUARD


// ============================================================================ INFO
/*
    Functions for managing the pages.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"


// ============================================================================ FUNC DECL

// ---------------------------------------------------------------------------- Main Page

Page*           MainPage_Make(void);

// ---------------------------------------------------------------------------- Game Page

Page*           GamePage_Make(PData* pData);
RGrid*          GamePage_GetRGrid(const Page* page);
SGraph*         GamePage_GetSGraph(const Page* page);
Time            GamePage_GetCurrentTime(const Page* page);
Grid            GamePage_GetGridSize(const Page* page);

// ---------------------------------------------------------------------------- Setup Page

Page*           SetupPage_Make(void);
void            SetupPage_SetCurrentTime(Page* page, Time current);
void            SetupPage_SetRecordTime(Page* page, Time record);



#endif // PAGES_GUARD


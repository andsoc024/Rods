// ============================================================================
// RODS
// Rod Grid
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions for managing the rod grid.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"
#include "../Fund/Fund.h"
#include "Logic.h"


// ============================================================================ PRIVATE MACROS

// The rod in rGrid, corresponding to the given node
#define RON(gTemp) \
    (rGrid->rods[(gTemp).x][(gTemp).y])


// ============================================================================ PRIVATE CONSTANTS

#define RGRID_UPDATABLE_N                   5


// ============================================================================ OPAQUE STRUCTURES

// A grid of rods of which one is the source. The rods can be rotated until all 
// are connected to the source
struct RGrid{
    Grid size;
    GNode source;

    Rod rods[RGRID_MAX_SIZE][RGRID_MAX_SIZE];

    GNode updatable[RGRID_UPDATABLE_N];
    int updIndex;

    int nElectrified;
    int nTotal;
};


// ============================================================================ PRIVATE FUNC DECL

void            RGrid_AddToUpdatable(RGrid* rGrid, GNode node);






// ============================================================================ FUNC DEF

// **************************************************************************** RGrid_MakeEmpty

// Make an empty rod grid with the given dimensions. Ensure that the dimensions 
// are within limits
RGrid* RGrid_MakeEmpty(int nCols, int nRows){
    RGrid* rGrid = Memory_Allocate(NULL, sizeof(RGrid), ZEROVAL_ALL);

    RGrid_SetSize(rGrid, nCols, nRows);

    return rGrid;
}


// **************************************************************************** RGrid_Copy

// Copy the rod grid from dst to src
RGrid* RGrid_Copy(RGrid* dst, const RGrid* src){
    return Memory_Copy(dst, src, sizeof(RGrid));
}


// **************************************************************************** RGrid_Free

// Free the memory of the rod grid
RGrid* RGrid_Free(RGrid* rGrid){
    return Memory_Free(rGrid);
}


// **************************************************************************** RGrid_Clear

// Clear all the rod legs and set the rod grid structure to an empty state
void RGrid_Clear(RGrid* rGrid){
    Grid size = GRID0(rGrid->size.nCols, rGrid->size.nRows);
    
    Memory_Set(rGrid, sizeof(RGrid), 0);

    rGrid->size = size;

    rGrid->source = GNODE_INVALID;

    for (int i = 0; i < RGRID_UPDATABLE_N; i++){
        rGrid->updatable[i] = GNODE_INVALID;
    }

    rGrid->nTotal = Grid_N(rGrid->size);
}


// **************************************************************************** RGrid_CreateRandom

// Create a random rod grid
void RGrid_CreateRandom(RGrid* rGrid){
    /*
        DEPTH-FIRST SEARCH ALGORITHM:
        1) Choose a random rod in the grid, electrify it, and make it the 
           source.
        2) Choose a random adjacent rod, connect to it and electrify it, 
           but only if it is not already electrified. This becomes the new 
           current rod.
        3) If there is not an unelectrified rod, backtrack to the last rod that 
           has unelectrified neighbours.
        4) The algorithm terminates when the process has backtracked from the 
           source.
    */

    // Ensure that the grid is clear
    RGrid_Clear(rGrid);

    // Make an array to hold the track
    GNode* track = Memory_Allocate(NULL, sizeof(GNode) * rGrid->nTotal, ZEROVAL_ALL);
    int n = 0;

    // Choose a random rod and make it the source
    rGrid->source = Grid_RandomNode(rGrid->size);
    RON(rGrid->source).isElectrified = true;
    rGrid->nElectrified++;

    // Add the source to the track
    track[0] = rGrid->source;
    n = 1;

    // Repeat until the process has backtracked from the source
    while (n > 0){
        // Make the last rod in the track the current rod
        GNode current = track[n - 1];

        // Assume no adjacent unelectrified rod exists
        GNode next = GNODE_INVALID;

        // Check all 4 directions for an unelectrified adjacent rod
        E_Direction dir = Direction_Random(INCLUDE_NOT_NONE);
        for (int i = 0; i < 4; i++){
            GNode temp = Grid_MoveNodeToDir(current, dir, 1);
            if (Grid_NodeIsInGrid(temp, rGrid->size) && !RON(temp).isElectrified){
                next = temp;
                break;
            }

            dir = Direction_Rotate(dir, 1);
        }

        // If an unelectrified adjacent rod was found, connect to it, electrify 
        // it and add it to the track
        if (next.x != INVALID){
            E_Direction opp = Direction_Opposite(dir);
            RON(current).legs |= Direction_ToLegDir(dir);
            RON(next).legs    |= Direction_ToLegDir(opp);
            RON(next).isElectrified = true;
            rGrid->nElectrified++;
            track[n] = next;
            n++;

        // If no unelectrified adjacent rod was found, then backtrack
        }else{
            n--;
        }
    }

    // Clean up
    track = Memory_Free(track);

    // Validate
    Err_Assert(rGrid->nElectrified == rGrid->nTotal, "Failed to create a valid rod grid");
}


// **************************************************************************** RGrid_SetSize

// Set the rod grid size and clear it. Ensure that the size is within the valid 
// limits
void RGrid_SetSize(RGrid* rGrid, int nCols, int nRows){
    nCols = PUT_IN_RANGE(nCols, RGRID_MIN_SIZE, RGRID_MAX_SIZE);
    nRows = PUT_IN_RANGE(nRows, RGRID_MIN_SIZE, RGRID_MAX_SIZE);

    rGrid->size = GRID0(nCols, nRows);

    RGrid_Clear(rGrid);
}
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
void            RGrid_SetRod(RGrid* rGrid, GNode node, int legs);
bool            RGrid_IsAnimating(const RGrid* rGrid);
int             RGrid_GetTotal(const RGrid* rGrid);
int             RGrid_GetNumElectrified(const Grid* rGrid);
int             RGrid_GetNumUnelectrified(const RGrid* rGrid);


#ifdef DEBUG_MODE
// **************************************************************************** RGrid_Print

    // Print the rod grid in the terminal as a map
    void RGrid_Print(const RGrid* rGrid){
        CHECK_NULL(rGrid, WITH_NEW_LINE)

        int nCols = MIN(rGrid->size.nCols, RGRID_MAX_SIZE);
        int nRows = MIN(rGrid->size.nRows, RGRID_MAX_SIZE);

        for (int y = 0; y < nRows; y++){
            for (int n = 0; n < 3; n++){
                for (int x = 0; x < nCols; x++){
                    GNode temp = GNODE(x, y);

                    switch (n){
                        case 0: case 2:{
                            int legDir = (n == 0) ? LEGDIR_UP : LEGDIR_DOWN;
                            printf("  %c  ", (RON(temp).legs & legDir) ? '|' : ' ');
                            break;
                        }

                        case 1:{
                            printf("%s", (RON(temp).legs & LEGDIR_LEFT) ? "--" : "  ");

                            char chr = Rod_IsAnimating(&RON(temp))             ? '@' :
                                       Grid_NodesAreEqual(temp, rGrid->source) ? 'S' :
                                       RON(temp).isElectrified                 ? 'X' : '*';
                            printf("%c", chr);

                            printf("%s", (RON(temp).legs & LEGDIR_RIGHT) ? "--" : "  ");
                            break;
                        }

                        default: {break;}
                    }
                }
                printf("\n");
            }
        }
        printf("\n");

        PRINT_LINE
        printf("Size:          "); Grid_Print(rGrid->size, WITH_NEW_LINE);
        printf("Source:        "); Grid_PrintNode(rGrid->source, WITH_NEW_LINE);
        printf("Updatable (%d):\n", RGRID_UPDATABLE_N);
        for (int i = 0; i < RGRID_UPDATABLE_N; i++){
            printf("   %d) ", i);
            if (rGrid->updatable[i].x == INVALID){
                printf("%s\n", STR_INVALID);
            }else{
                Grid_PrintNode(rGrid->updatable[i], WITH_NEW_LINE);
            }
        }
        printf("Upd Index:     %d\n", rGrid->updIndex);
        printf("Total:         %d\n", rGrid->nTotal);
        printf("N Electrified: %d\n", rGrid->nElectrified);
        PRINT_LINE
    }
#endif






// ============================================================================ PRIVATE FUNC DEF

// ****************************************************************************
void            RGrid_AddToUpdatable(RGrid* rGrid, GNode node);


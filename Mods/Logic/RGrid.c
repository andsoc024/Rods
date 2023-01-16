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
bool            RGrid_RodCanBeElectrified(const RGrid* rGrid, GNode node);






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


// **************************************************************************** RGrid_Shuffle

// Rotate all the rods in the grid randomly 0-3 times
void RGrid_Shuffle(RGrid* rGrid){
    GNode temp = GNODE_NULL;
    do{
        Rod_Rotate(&RON(temp), Math_RandomInt(0, 3), WITHOUT_ANIM);
        temp = Grid_NextNode(temp, rGrid->size);
    }while (!Grid_NodesAreEqual(temp, GNODE_NULL));

    RGrid_Reelectrify(rGrid);
}


// **************************************************************************** RGrid_Electrify

// Electrify the rod, starting from the rod at start. If start is 
// GNODE_INVALID, start at the source
void RGrid_Electrify(RGrid* rGrid, GNode start){
    // If start is invalid, start from the source
    start = Grid_NodesAreEqual(start, GNODE_INVALID) ? rGrid->source : start;

    // If start can not be electrified, do nothing
    if (!RGrid_RodCanBeElectrified(rGrid, start)){
        return;
    }

    // Make an array to hold the electrified rods
    GNode* list = Memory_Allocate(NULL, sizeof(GNode) * rGrid->nTotal, ZEROVAL_NONE);
    int n = 0;

    // Electrify the start and add it to the list
    if (!RON(start).isElectrified){
        RON(start).isElectrified = true;
        rGrid->nElectrified++;
    }
    list[0] = start;
    n++;

    // Repeat for all nodes in the list
    for (int i = 0; i < n; i++){
        GNode current = list[i];

        // Check all 4 directions for rods that are connected to the current 
        // rod
        for (int dir = DIR_RIGHT; dir <= DIR_UP; dir++){
            GNode next = Grid_MoveNodeToDir(current, dir, 1);
            if (Grid_NodeIsInGrid(next, rGrid->size) && !RON(next).isElectrified){
                if (Rod_IsConnectedToRod(&RON(current), &RON(next), dir)){
                    RON(next).isElectrified = true;
                    rGrid->nElectrified++;
                    list[n] = next;
                    n++;
                }
            }
        }
    }

    // Clean up
    list = Memory_Free(list);
}


// **************************************************************************** RGrid_Deelectrify

// Set all the rods as not electrified
void RGrid_Deelectrify(RGrid* rGrid){
    GNode temp = GNODE_NULL;
    do{
        RON(temp).isElectrified = false;
        temp = Grid_NextNode(temp, rGrid->size);
    }while (!Grid_NodesAreEqual(temp, GNODE_NULL));

    rGrid->nElectrified = 0;
}


// **************************************************************************** RGrid_Reelectrify

// Deelectrify and reelectrify the rod grid
void RGrid_Reelectrify(RGrid* rGrid){
    RGrid_Deelectrify(rGrid);
    RGrid_Electrify(rGrid, GNODE_INVALID);
}


// **************************************************************************** RGrid_RotateRod

// Rotate the rod at the given node by 90°, clockwise, with animation
void RGrid_RotateRod(RGrid* rGrid, GNode node){
    if (!Grid_NodeIsInGrid(node, rGrid->size)){
        return;
    }

    Rod_Rotate(&RON(node), 1, WITH_ANIM);

    if (RON(node).isElectrified){
        RGrid_Reelectrify(rGrid);
    }

    RGrid_AddToUpdatable(rGrid, node);
}


// **************************************************************************** RGrid_Update

// Update all the rods in the updatable list
void RGrid_Update(RGrid* rGrid){
    for (int i = 0; i < RGRID_UPDATABLE_N; i++){
        if (rGrid->updatable[i].x == INVALID) {continue;}
        if (Rod_Update(&RON(rGrid->updatable[i])) == COMPLETED){
            RGrid_Electrify(rGrid, rGrid->updatable[i]);
            rGrid->updatable[i] = GNODE_INVALID;
        }
    }
}


// **************************************************************************** RGrid_FinishAnim

// Finish all eventual animations at the updatable list
void RGrid_FinishAnim(RGrid* rGrid){
    bool reelectrificationNeeded = false;
    for (int i = 0; i < RGRID_UPDATABLE_N; i++){
        if (rGrid->updatable[i].x != INVALID){
            Rod_FinishAnim(&RON(rGrid->updatable[i]));
            reelectrificationNeeded = true;
            rGrid->updatable[i] = GNODE_INVALID;
        }
    }

    if (reelectrificationNeeded){
        RGrid_Reelectrify(rGrid);
    }
}


// **************************************************************************** RGrid_GetSize

// Return the rod grid size
Grid RGrid_GetSize(const RGrid* rGrid){
    return rGrid->size;
}


// **************************************************************************** RGrid_GetSource

// Return the source of the rod grid
GNode RGrid_GetSource(const RGrid* rGrid){
    return rGrid->source;
}


// **************************************************************************** RGrid_GetRod

// Return the rode at the given node. If the node is invalid, return NULL
Rod* RGrid_GetRod(const RGrid* rGrid, GNode node){
    if (!Grid_NodeIsInGrid(node, rGrid->size)){
        return NULL;
    }

    return (Rod*) &RON(node);
}


// **************************************************************************** RGrid_GetRod_Fast

// Return the rod at the given node, without validating the node
Rod* RGrid_GetRod_Fast(const RGrid* rGrid, GNode node){
    return (Rod*) &RON(node);
}


// **************************************************************************** RGrid_SetRod

// Set the rod at the given node, with the given legs and as unelectrified and 
// not rotating
void RGrid_SetRod(RGrid* rGrid, GNode node, int legs){
    Rod_Set(&RON(node), legs);
}


// **************************************************************************** RGrid_IsAnimating

// Return true if any rod in the grid is animating
bool RGrid_IsAnimating(const RGrid* rGrid){
    for (int i = 0; i < RGRID_UPDATABLE_N; i++){
        if (rGrid->updatable[i].x != INVALID){
            return true;
        }
    }

    return false;
}


// **************************************************************************** RGrid_GetTotal

// Return the total number of rods in the grid
int RGrid_GetTotal(const RGrid* rGrid){
    return rGrid->nTotal;
}


// **************************************************************************** RGrid_GetNumElectrified

// Return the number of electrified rods in the grid
int RGrid_GetNumElectrified(const RGrid* rGrid){
    return rGrid->nElectrified;
}


// **************************************************************************** RGrid_GetNumUnelectrified

// Return the number of unelectrified rods in the grid
int RGrid_GetNumUnelectrified(const RGrid* rGrid){
    return rGrid->nTotal - rGrid->nElectrified;
}


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

// **************************************************************************** RGrid_AddToUpdatable

// Add the node to the updatable list. If the position is already occupied, 
// finish the current animation first
void RGrid_AddToUpdatable(RGrid* rGrid, GNode node){
    if (!Grid_NodeIsInGrid(node, rGrid->size)){
        return;
    }

    for (int i = 0; i < RGRID_UPDATABLE_N; i++){
        if (Grid_NodesAreEqual(node, rGrid->updatable[i])){
            return;
        }
    }

    int ind = rGrid->updIndex;

    if (rGrid->updatable[ind].x != INVALID){
        if (Rod_FinishAnim(&RON(rGrid->updatable[ind]))){
            RGrid_Electrify(rGrid, rGrid->updatable[ind]);
        }
    }

    rGrid->updatable[ind] = node;

    rGrid->updIndex = (ind + 1) % RGRID_UPDATABLE_N;
}


// **************************************************************************** RGrid_RodCanBeElectrified

// Return true if the rod can be electrified
bool RGrid_RodCanBeElectrified(const RGrid* rGrid, GNode node){
    if (!Grid_NodeIsInGrid(node, rGrid->size)){
        return false;
    }

    if (Grid_NodesAreEqual(node, rGrid->source) && 
        !Rod_IsAnimating(&RON(node))){
        return true;
    }

    for (int dir = DIR_RIGHT; dir <= DIR_UP; dir++){
        GNode next = Grid_MoveNodeToDir(node, dir, 1);
        if (Grid_NodeIsInGrid(next, rGrid->size) && 
            RON(next).isElectrified &&
            Rod_IsConnectedToRod(&RON(node), &RON(next), dir)){
            return true;
        }
    }

    return false;
}


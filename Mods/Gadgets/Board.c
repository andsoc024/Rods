// ============================================================================
// RODS
// Board
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions as well as structure and constants definitions for the Board 
    gadget.

    The board gadget manages the rod grid. It emits the victory event when 
    completed.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"
#include "../Fund/Fund.h"
#include "../Logic/Logic.h"
#include "../Graph/Graph.h"
#include "../GUI/GUI.h"
#include "Gadgets.h"


// ============================================================================ PRIVATE MACROS

// **************************************************************************** BDATA

// Pointer to the data object of the board
#define BDATA \
    ((BoardData*) board->data)


// **************************************************************************** RGRID

// Pointer to the rod grid
#define RGRID \
    (BDATA->rGrid)


// **************************************************************************** SGRAPH

// Pointer to the scroll graphics object
#define SGRAPH \
    (BDATA->sg)


// **************************************************************************** RODMODEL

// Pointer to the rod model
#define RODMODEL \
    (BDATA->rodModel)


// ============================================================================ PRIVATE CONSTANTS

#define BOARD_MOVE_DIST_RATIO               0.07f
#define BOARD_ZOOM                          1.1f


// ============================================================================ PRIVATE STRUCTURES

// **************************************************************************** BoardData

// The data object of the board gadget
typedef struct BoardData{
    RGrid* rGrid;
    SGraph* sg;
    RodModel* rodModel;
    GNode selBox;
    bool isReactive;
    GNode pressedRod;
    bool victory;
}BoardData;


// ============================================================================ PRIVATE FUNC DECL

void            Board_PrepareToFree(Gadget* board);
void            Board_Resize(Gadget* board);
void            Board_ReactToEvent(Gadget* board, Event event, EventQueue* queue);
void            Board_Update(Gadget* board, EventQueue* queue);
void            Board_Draw(const Gadget* board, Vector2 shift);
Grid            Board_CalcVisiblePart(const Gadget* board);
GNode           Board_FirstVisibleRod(const Gadget* board);
float           Board_CalcTileSize(const Gadget* board);
void            Board_MoveSelBox(Gadget* board, E_Direction dir);
void            Board_FocusOnSource(Gadget* board);
void            Board_Zoom(Gadget* board, float zoom);
void            Board_MoveViewportToDir(Gadget* board, E_Direction dir);
void            Board_TranslateViewport(Gadget* board, Vector2 vector);
GNode           Board_RodAtMousePos(const Gadget* board, Point mousePos);
#ifdef DEBUG_MODE
    void        Board_PrintData(const Gadget* board);
#endif






// ============================================================================ FUNC DEF

// **************************************************************************** Board_Make

// Make a board gadget. Load the road grid from the given data. If NULL create 
// a new grid of default size
Gadget* Board_Make(E_GadgetID id, const PData* pData){
    Gadget* board = Gadget_Make(id, GT_BOARD, IS_NOT_SELECTABLE, 0);

    board->PrepareToFree = Board_PrepareToFree;
    board->Resize        = Board_Resize;
    board->ReactToEvent  = Board_ReactToEvent;
    board->Update        = Board_Update;
    board->Draw          = Board_Draw;
    #ifdef DEBUG_MODE
        board->PrintData = Board_PrintData;
    #endif

    board->cRect = TO_RECT(Glo_WinSize);

    BoardData* data = Memory_Allocate(NULL, sizeof(BoardData), ZEROVAL_ALL);
    board->data = data;

    if (pData != NULL && pData->rGrid != NULL){
        data->rGrid = RGrid_Copy(NULL, pData->rGrid);
    }else{
        data->rGrid = RGrid_MakeEmpty(RGRID_DEF_SIZE, RGRID_DEF_SIZE);
        RGrid_CreateRandom(data->rGrid);
        RGrid_Shuffle(data->rGrid);
    }

    if (pData != NULL && pData->sg != NULL){
        data->sg = SGraph_Copy(NULL, pData->sg);
        SGraph_SetView(data->sg, board->cRect);
    }else{
        data->sg = SGraph_MakeFromGrid(RGrid_GetSize(data->rGrid), ROD_DEF_TEXTURE_SIZE, board->cRect, 
                                       SGRAPH_DEF_MARGIN, ROD_MIN_TEXTURE_SIZE, ROD_MAX_TEXTURE_SIZE);
    }

    float tileSize = Board_CalcTileSize(board);

    data->rodModel = RGraph_MakeRodModel(tileSize);

    data->isReactive = true;
    data->pressedRod = GNODE_INVALID;
    data->selBox = GNODE_INVALID;
    data->victory = false;
    
    return board;
}


// **************************************************************************** Board_CreateNewRodGrid

// Create a new rod grid of the given size
void Board_CreateNewRodGrid(Gadget* board, int nCols, int nRows){
    RGrid_SetSize(RGRID, nCols, nRows);
    RGrid_CreateRandom(RGRID);
    RGrid_Shuffle(RGRID);

    SGRAPH = SGraph_Free(SGRAPH);
    SGRAPH = SGraph_MakeFromGrid(RGrid_GetSize(RGRID), ROD_DEF_TEXTURE_SIZE, board->cRect, 
                                 SGRAPH_DEF_MARGIN, ROD_MIN_TEXTURE_SIZE, ROD_MAX_TEXTURE_SIZE);

    float tileSize = Board_CalcTileSize(board);
    RGraph_ResizeRodModel(RODMODEL, tileSize);

    BDATA->selBox = GNODE_INVALID;
    BDATA->pressedRod = GNODE_INVALID;
    BDATA->victory = false;
}


// **************************************************************************** Board_SetAsReactive

// Set the board as reactive to mouse events
void Board_SetAsReactive(Gadget* board){
    BDATA->isReactive = true;
}


// **************************************************************************** Board_SetAsNotReactive

// Set the board as not reactive to mouse events
void Board_SetAsNotReactive(Gadget* board){
    BDATA->isReactive = false;
}


// **************************************************************************** Board_GetNumElectrifiedRods

// Return the number of electrified rods in the grid
int Board_GetNumElectrifiedRods(const Gadget* board){
    return RGrid_GetNumElectrified(RGRID);
}


// **************************************************************************** Board_GetNumUnelectrifiedRodsLeft

// Return the number of unelectrified rods left in the grid
int Board_GetNumUnelectrifiedRodsLeft(const Gadget* board){
    return RGrid_GetNumUnelectrified(RGRID);
}


// **************************************************************************** Board_GetTotalNumRods

// Return the total number of rods in the grid
int Board_GetTotalNumRods(const Gadget* board){
    return RGrid_GetTotal(RGRID);
}


// **************************************************************************** Board_GetGridSize

// Return the rod grid size as a grid
Grid Board_GetGridSize(const Gadget* board){
    return RGrid_GetSize(RGRID);
}


// **************************************************************************** Board_GetSelBox

// Return the grid node corresponding to the position of the selection box
GNode Board_GetSelBox(const Gadget* board){
    return BDATA->selBox;
}


// **************************************************************************** Board_GetRGrid

// Return a pointer to the rod grid
RGrid* Board_GetRGrid(const Gadget* board){
    return RGRID;
}


// **************************************************************************** Board_GetSGraph

// Return a pointer to the scroll graphics object
SGraph* Board_GetSGraph(const Gadget* board){
    return SGRAPH;
}


// **************************************************************************** Board_GetRodModel

// Return a pointer to the rod model
RodModel* Board_GetRodModel(const Gadget* board){
    return RODMODEL;
}


// **************************************************************************** Board_IsReactive

// Return true if the board can react to mouse events
bool Board_IsReactive(const Gadget* board){
    return BDATA->isReactive;
}






// ============================================================================ PRIVATE FUNC DEF

// **************************************************************************** Board_PrepareToFree

// Free the rod grid, scroll graphics and rod model
void Board_PrepareToFree(Gadget* board){
    RGRID    = RGrid_Free(RGRID);
    SGRAPH   = SGraph_Free(SGRAPH);
    RODMODEL = RGraph_FreeRodModel(RODMODEL);
}


// **************************************************************************** Board_Resize

// Resize the board
void Board_Resize(Gadget* board){
    BDATA->selBox = GNODE_INVALID;
    BDATA->pressedRod = GNODE_INVALID;

    SGraph_SetView(SGRAPH, board->cRect);

    float tileSize = Board_CalcTileSize(board);

    RGraph_ResizeRodModel(RODMODEL, tileSize);
}


// **************************************************************************** Board_ReactToEvent

// The board reacts to mouse, keyboard and other GUI events
void Board_ReactToEvent(Gadget* board, Event event, UNUSED EventQueue* queue){
    switch (event.id){
        case EVENT_MOUSE_PRESSED:{
            if (!BDATA->isReactive) {break;}
            if (!Geo_PointIsInRect(event.data.mouse.pos, board->cRect)) {break;}
            BDATA->pressedRod = Board_RodAtMousePos(board, event.data.mouse.pos);
            break;
        }

        case EVENT_MOUSE_RELEASED:{
            if (BDATA->isReactive && !BDATA->victory && Geo_PointIsInRect(event.data.mouse.pos, board->cRect)){
                if (!Geo_PointIsInRect(event.data.mouse.pos, board->cRect)) {break;}
                GNode rod = Board_RodAtMousePos(board, event.data.mouse.pos);
                if (Grid_NodesAreEqual(rod, BDATA->pressedRod) && Grid_NodeIsInGrid(rod, RGrid_GetSize(RGRID))){
                    RGrid_RotateRod(RGRID, rod);
                }
            }
            BDATA->pressedRod = GNODE_INVALID;
            break;
        }

        case EVENT_MOUSE_MOVE:{
            BDATA->selBox = GNODE_INVALID;
            break;
        }

        case EVENT_MOUSE_DRAG:{
            if (!BDATA->isReactive) {break;}
            if (!Geo_PointIsInRect(event.data.mouse.pos, board->cRect)) {break;}
            BDATA->pressedRod = GNODE_INVALID;
            Board_TranslateViewport(board, Geo_OppositePoint(event.data.mouse.delta));
            break;
        }

        case EVENT_MOUSE_WHEEL:{
            if (!BDATA->isReactive) {break;}
            if (!Geo_PointIsInRect(event.data.wheel.pos, board->cRect)) {break;}
            float zoom = 1.0f + event.data.wheel.move * MOUSE_WHEEL_SENITIVITY;

            Board_Zoom(board, zoom);
            break;
        }

        case EVENT_KEY_PRESSED:{
            switch (event.data.key){
                case WKEY_RIGHT: case WKEY_DOWN: case WKEY_LEFT: case WKEY_UP:{
                    E_Direction dir = Direction_FromKey(event.data.key);
                    Board_MoveViewportToDir(board, dir);
                    break;
                }

                case WKEY_D: case WKEY_S: case WKEY_A: case WKEY_W:{
                    E_Direction dir = Direction_FromKey(event.data.key);
                    Board_MoveSelBox(board, dir);
                    break;
                }

                case WKEY_SPACE:{
                    if (BDATA->victory) {break;}
                    if (Grid_NodeIsInGrid(BDATA->selBox, RGrid_GetSize(RGRID))){
                        RGrid_RotateRod(RGRID, BDATA->selBox);
                    }
                    break;
                }

                case WKEY_PLUS: case WKEY_MINUS: case WKEY_Z: case WKEY_X:{
                    float zoom = BOARD_ZOOM;
                    if (event.data.key == WKEY_MINUS || event.data.key == WKEY_X){
                        zoom = 1.0f / zoom;
                    }
                    Board_Zoom(board, zoom);
                    break;
                }

                case WKEY_R:{
                    Board_FocusOnSource(board);
                    break;
                }

                default: {break;}
            }

            break;
        }

        case EVENT_MAKE_NEW_GRID:{
            Board_CreateNewRodGrid(board, event.data.newGrid.nCols, event.data.newGrid.nRows);
            break;
        }

        default: {break;}
    }
}


// **************************************************************************** Board_Update

// Update the rod grid animations and emit the victory event when the grid is 
// completed
void Board_Update(Gadget* board, EventQueue* queue){
    RGrid_Update(RGRID);

    if (!BDATA->victory && RGrid_IsCompleted(RGRID)){
        Queue_AddEvent(queue, Event_SetAsVictory(board->id));
        BDATA->victory = true;
    }
}


// **************************************************************************** Board_Draw

// Draw the rod grid and the selction box
void Board_Draw(const Gadget* board, Vector2 shift){
    float tileSize = RGraph_GetTileSize(RODMODEL);
    Grid visible = Board_CalcVisiblePart(board);
    Point origin = SGraph_ProjectPoint(Grid_NodeToPoint(visible.origin, tileSize), SGRAPH);
    origin = Geo_TranslatePoint(origin, shift);

    RGraph_DrawRGrid(RGRID, visible, origin, RODMODEL);

    if (BDATA->selBox.x != INVALID){
        Point selboxPos = SGraph_ProjectPoint(Grid_NodeToPoint(BDATA->selBox, tileSize), SGRAPH);
        selboxPos = Geo_TranslatePoint(selboxPos, shift);
        RGraph_DrawSelBox(selboxPos, RODMODEL);
    }
}


// **************************************************************************** Board_CalcVisiblePart

// The visible part of the rod grid
Grid Board_CalcVisiblePart(const Gadget* board){
    Rect viewport = SGraph_GetViewport(SGRAPH);
    Grid total = RGrid_GetSize(RGRID);
    float tileSize = RGraph_GetTileSize(RODMODEL);

    return Grid_SectionFromRect(viewport, tileSize, total);
}


// **************************************************************************** Board_FirstVisibleRod

// The top left rod in the visiblke part of the grid
GNode Board_FirstVisibleRod(const Gadget* board){
    return Board_CalcVisiblePart(board).origin;
}


// **************************************************************************** Board_CalcTileSize

// Calculate the tile size in the virtual screen of the scroll graphics
float Board_CalcTileSize(const Gadget* board){
    Grid grid = RGrid_GetSize(RGRID);
    Size vScreen = SGraph_GetVScreen(SGRAPH);
    return Grid_CalcSqrCellSize(grid, vScreen);
}


// **************************************************************************** Board_MoveSelBox

// Move the selection box to the given direction
void Board_MoveSelBox(Gadget* board, E_Direction dir){
    Grid grid = RGrid_GetSize(RGRID);

    if (!Grid_NodeIsInGrid(BDATA->selBox, grid)){
        BDATA->selBox = Board_FirstVisibleRod(board);
        return;
    }

    GNode newPos = Grid_MoveNodeToDir(BDATA->selBox, dir, 1);

    if (Grid_NodeIsInGrid(newPos, grid)){
        BDATA->selBox = newPos;
    }
}


// **************************************************************************** Board_FocusOnSource

// Set the scroll graphics in default position and center the viewport over the 
// source
void Board_FocusOnSource(Gadget* board){
    SGraph_SetDefault(SGRAPH);
    float tileSize = Board_CalcTileSize(board);
    RGraph_ResizeRodModel(RODMODEL, tileSize);

    Point sourcePos = Grid_NodeToPoint(RGrid_GetSource(RGRID), tileSize);
    sourcePos = SGraph_ProjectPoint(sourcePos, SGRAPH);
    sourcePos = Geo_MovePoint(sourcePos, tileSize * 0.5f, tileSize * 0.5f);
    SGraph_PlaceViewport(SGRAPH, sourcePos, RP_CENTER);
}


// **************************************************************************** Board_Zoom

// Zoom the scroll graphics in or out
void Board_Zoom(Gadget* board, float zoom){
    SGraph_Zoom(SGRAPH, zoom);
    float tileSize = Board_CalcTileSize(board);
    RGraph_ResizeRodModel(RODMODEL, tileSize);
}


// **************************************************************************** Board_MoveViewportToDir

// Move the viewport of the scroll graphics object towards the given direction
void Board_MoveViewportToDir(Gadget* board, E_Direction dir){
    float moveDistance = MIN_DIM(board->cRect) * BOARD_MOVE_DIST_RATIO;
    SGraph_MoveViewportToDir(SGRAPH, dir, moveDistance);
}


// **************************************************************************** Board_TranslateViewport

// Move the viewport by the given vector
void Board_TranslateViewport(Gadget* board, Vector2 vector){
    SGraph_TranslateViewport(SGRAPH, vector);
}


// **************************************************************************** Board_RodAtMousePos

// The node corresponding to the rod under the mouse cursor
GNode Board_RodAtMousePos(const Gadget* board, Point mousePos){
    Point p = SGraph_UnprojectPoint(mousePos, SGRAPH);
    float tileSize = RGraph_GetTileSize(RODMODEL);

    GNode res = Grid_PointToNode(p, tileSize);

    if (!Grid_NodeIsInGrid(res, RGrid_GetSize(RGRID))){
        return GNODE_INVALID;
    }

    return res;
}


#ifdef DEBUG_MODE

// **************************************************************************** Board_PrintData

    // Multiline print of the parameters of the data object of the board
    void Board_PrintData(const Gadget* board){
        CHECK_NULL(board, WITH_NEW_LINE)
        CHECK_NULL(board->data, WITH_NEW_LINE);

        printf("RGrid:         %p\n", (void*) (BDATA->rGrid));
        printf("SGraph;        %p\n", (void*) (BDATA->sg));
        printf("Rod Model:     %p\n", (void*) (BDATA->rodModel));
        printf("Selection Box: "); Grid_PrintNode(BDATA->selBox, WITH_NEW_LINE);
        printf("Is Reactive:   %s\n", Bool_ToString(BDATA->isReactive, LONG_FORM));
        printf("Pressed Rod:   "); Grid_PrintNode(BDATA->pressedRod, WITH_NEW_LINE);
        printf("Victory:       %s\n", Bool_ToString(BDATA->victory, LONG_FORM));
    }
#endif




// ============================================================================
// RODS
// Grid
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions for working with grid structures: GNode and Grid.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"
#include "Fund.h"


// ============================================================================ FUNC DEF

// **************************************************************************** Grid_SetBetweenNodes

// Set a grid between the two nodes
Grid Grid_SetBetweenNodes(GNode node1, GNode node2){
    Grid res;

    res.origin.x = MIN(node1.x, node2.x);
    res.origin.y = MIN(node1.y, node2.y);
    
    res.nCols = node2.x - node1.x;
    res.nCols = ABS(res.nCols) + 1;
    res.nRows = node2.y - node1.y;
    res.nRows = ABS(res.nRows) + 1;

    return res;
}


// **************************************************************************** Grid_IsValid

// Return true if the grid is valid (nCols and nRows are positive)
bool Grid_IsValid(Grid grid){
    return (grid.nCols > 0 && 
            grid.nRows > 0);
}


// **************************************************************************** Grid_N

// The number of nodes in the grid
int Grid_N(Grid grid){
    return grid.nCols * grid.nRows;
}


// **************************************************************************** Grid_RandomNode

// A random node in the grid
GNode Grid_RandomNode(Grid grid){
    GNode res;
    res.x = grid.origin.x + Math_RandomInt(0, grid.nCols - 1);
    res.y = grid.origin.y + Math_RandomInt(0, grid.nRows - 1);

    return res;
}


// **************************************************************************** Grid_LastNode

// The bottom right node of the grid
GNode Grid_LastNode(Grid grid){
    return GNODE(grid.origin.x + grid.nCols - 1, 
                 grid.origin.y + grid.nRows - 1);
}


// **************************************************************************** Grid_NextNode

// Move one node to the right. When the right edge is reached, move to the 
// first node in the next row. When the bottom right is reached, jump to the 
// top left
GNode Grid_NextNode(GNode node, Grid grid){
    node.x++;
    if (node.x >= grid.origin.x + grid.nCols){
        node.x = grid.origin.x;
        node.y++;
        if (node.y >= grid.origin.y + grid.nRows){
            node.y = grid.origin.y;
        }
    }

    return node;
}


// **************************************************************************** Grid_Limit

// The limit of the grid on the X or Y axis towards the given direction
int Grid_Limit(Grid grid, E_Direction dir){
    switch (dir){
        case DIR_RIGHT: {return grid.origin.x + grid.nCols - 1;}
        case DIR_DOWN:  {return grid.origin.y + grid.nRows - 1;}
        case DIR_LEFT:  {return grid.origin.x;}
        case DIR_UP:    {return grid.origin.y;}
        default:        {break;}
    }

    return INVALID;
}


// **************************************************************************** Grid_OppositeNode

// Return the reflection of the node in (0, 0)
GNode Grid_OppositeNode(GNode node){
    return GNODE(-node.x, -node.y);
}


// **************************************************************************** Grid_NodesAreEqual

// Return true if the two nodes are the same
bool Grid_NodesAreEqual(GNode node1, GNode node2){
    return (node1.x == node2.x) && 
           (node1.y == node2.y);
}


// **************************************************************************** Grid_NodeIsInRange

// Return true if the node is beteween and including the limits
bool Grid_NodeIsInRange(GNode node, int minX, int maxX, int minY, int maxY){
    return IS_IN_RANGE(node.x, minX, maxX) &&
           IS_IN_RANGE(node.y, minY, maxY);
}


// **************************************************************************** Grid_NodeIsInGrid

// Return true if the node is part of the grid
bool Grid_NodeIsInGrid(GNode node, Grid grid){
    return Grid_NodeIsInRange(node, grid.origin.x, grid.origin.x + grid.nCols - 1, 
                                    grid.origin.y, grid.origin.y + grid.nRows - 1);
}


// **************************************************************************** Grid_PutInRange

// Ensure that the node is between and including the limits
GNode Grid_PutNodeInRange(GNode node, int minX, int maxX, int minY, int maxY){
    node.x = PUT_IN_RANGE(node.x, minX, maxX);
    node.y = PUT_IN_RANGE(node.y, minY, maxY);

    return node;
}


// **************************************************************************** Grid_PutNodeInGrid

// Ensure that the node is inide the grid
GNode Grid_PutNodeInGrid(GNode node, Grid grid){
    return Grid_PutNodeInRange(node, grid.origin.x, grid.origin.x + grid.nCols - 1, 
                                     grid.origin.y, grid.origin.y + grid.nRows - 1);
}


// **************************************************************************** Grid_Overlap

// Return true if the two grids have one or more common nodes
bool Grid_Overlap(Grid grid1, Grid grid2){
    return  Grid_NodeIsInRange(grid1.origin, grid2.origin.x - (grid1.nCols - 1), 
                                             grid2.origin.x + (grid2.nCols - 1), 
                                             grid2.origin.y - (grid1.nRows - 1), 
                                             grid2.origin.y + (grid2.nRows - 1));
}


// **************************************************************************** Grid_Section

// Return the overlapping part of the two grids. If they do not overlap, return 
// GRID_NULL
Grid Grid_Section(Grid grid1, Grid grid2){
    int x = MAX(grid1.origin.x, grid2.origin.x);
    int y = MAX(grid1.origin.y, grid2.origin.y);

    int x1_1 = grid1.origin.x + grid1.nCols - 1;
    int x1_2 = grid2.origin.x + grid2.nCols - 1;
    int x1 = MIN(x1_1, x1_2);

    int y1_1 = grid1.origin.y + grid1.nRows - 1;
    int y1_2 = grid2.origin.y + grid2.nRows - 1;
    int y1 = MIN(y1_1, y1_2);

    int nCols = x1 - x + 1;
    int nRows = y1 - y + 1;

    if (nCols <= 0 || nRows <= 0){
        return GRID_NULL;
    } 

    return GRID(x, y, nCols, nRows);
}


// **************************************************************************** Grid_MoveNode

// Move the node by dx and dy
GNode Grid_MoveNode(GNode node, int dx, int dy){
    node.x += dx;
    node.y += dy;

    return node;
}


// **************************************************************************** Grid_MoveToDir

// Move the node towards the given direction, by the given distance
GNode Grid_MoveNodeToDir(GNode node, E_Direction dir, int distance){
    switch (dir){
        case DIR_RIGHT: {node.x += distance; break;}
        case DIR_DOWN:  {node.y += distance; break;}
        case DIR_LEFT:  {node.x -= distance; break;}
        case DIR_UP:    {node.y -= distance; break;}
        default:        {break;}
    }

    return node;
}


// **************************************************************************** Grid_TranslateNode

// Move the node by the given vector
GNode Grid_TranslateNode(GNode node, GNode vector){
    return Grid_MoveNode(node, vector.x, vector.y);
}


// **************************************************************************** Grid_CalcCellSize

// The rectangular cell size of the grid, given its total size
Size Grid_CalcCellSize(Grid grid, Size gridSize){
    Size res;
    res.width  = gridSize.width  / (float) grid.nCols;
    res.height = gridSize.height / (float) grid.nRows;

    return res;
}


// **************************************************************************** Grid_CalcSqrCellSize

// The minimum square cell size of the grid, for which it fills the entire size
float Grid_CalcSqrCellSize(Grid grid, Size gridSize){
    Size rectCellSize = Grid_CalcCellSize(grid, gridSize);
    return MAX(rectCellSize.width, rectCellSize.height);
}


// **************************************************************************** Grid_NodeToPoint

// The top left corner of the grid cell, corresponding to the node, given the 
// square cell size
Point Grid_NodeToPoint(GNode node, float cellSize){
    Point res;
    res.x = (float) node.x * cellSize;
    res.y = (float) node.y * cellSize;

    return res;
}


// **************************************************************************** Grid_PointToNode

// The node corresponding to the grid cell the point is inside of
GNode Grid_PointToNode(Point p, float cellSize){
    GNode res;
    res.x = (int) (p.x / cellSize);
    res.y = (int) (p.y / cellSize);

    res.x = (p.x < 0) ? res.x - 1 : res.x;
    res.y = (p.y < 0) ? res.y - 1 : res.y;

    return res;
}


// **************************************************************************** Grid_ToSize

// The size of the grid, given the square cell size
Size Grid_ToSize(Grid grid, float cellSize){
    Size res;
    res.width  = (float) grid.nCols * cellSize;
    res.height = (float) grid.nRows * cellSize;

    return res;
}


// **************************************************************************** Grid_FromSize

// The minimum grid, with the given square cell size, that completely covers 
// the size
Grid Grid_FromSize(Size size, float cellSize){
    return Grid_FromRect(TO_RECT(size), cellSize);
}


// **************************************************************************** Grid_ToRect

// The rectangle containing the grid, given its square cell size
Rect Grid_ToRect(Grid grid, float cellSize){
    Point origin = Grid_NodeToPoint(grid.origin, cellSize);
    Size size = Grid_ToSize(grid, cellSize);

    return RECT(origin.x, origin.y, size.width, size.height);
}


// **************************************************************************** Grid_FromRect

// The minimum grid, with the given square cell size, that completely covers 
// the rectangle
Grid Grid_FromRect(Rect rect, float cellSize){
    Point p0 = RORIGIN(rect);
    Point p1 = Geo_RectPoint(rect, RP_BOTTOM_RIGHT);

    GNode node0 = Grid_PointToNode(p0, cellSize);
    GNode node1 = Grid_PointToNode(p1, cellSize);

    return Grid_SetBetweenNodes(node0, node1);
}


// **************************************************************************** Grid_SectionFromRect

// The section between the minimum grid, with the given square cell size, that 
// completely covers the rectangle, and the total grid
Grid Grid_SectionFromRect(Rect rect, float cellSize, Grid totalGrid){
    Grid res = Grid_FromRect(rect, cellSize);
    res = Grid_Section(res, totalGrid);
    return res;
}


// **************************************************************************** Grid_FitSizeToGrid

// Adjust the size, so that it has the same aspect ratio as the grid
Size Grid_FitSizeToGrid(Size size, Grid grid){
    Size rectCellSize = Grid_CalcCellSize(grid, size);
    float cellSize = AVG(rectCellSize.width, rectCellSize.height);
    return Grid_ToSize(grid, cellSize);
}


#ifdef DEBUG_MODE
// **************************************************************************** Grid_PrintNode

    // Print the node in the form (x, y). Optionally with new line
    void Grid_PrintNode(GNode node, bool withNewLine){
        printf("(%d, %d)", node.x, node.y);
        if (withNewLine) {printf("\n");}
    }


// **************************************************************************** Grid_Print

    // Print the grid in the form (x, y, nCols, nRows). Optionally with new 
    // line
    void Grid_Print(Grid grid, bool withNewLine){
        printf("(%d, %d, ", grid.origin.x, grid.origin.y);
        printf("%d, %d)", grid.nCols, grid.nRows);
        if (withNewLine) {printf("\n");}
    }
#endif





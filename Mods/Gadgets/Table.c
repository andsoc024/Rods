// ============================================================================
// RODS
// Table
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions and definitions of constants and structures for the table gadget.

    The table gadget consists of cells organised in columns and rows. Each cell 
    contains a label.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include <stdarg.h>

#include "../Public/Public.h"
#include "../Fund/Fund.h"
#include "../Graph/Graph.h"
#include "../GUI/GUI.h"
#include "Gadgets.h"


// ============================================================================ PRIVATE MACROS

// **************************************************************************** TDATA

// Pointer to the data object of the table gadget
#define TDATA \
    ((TableData*) table->data)


// **************************************************************************** TCELL

// Pointer to the table cell
#define TCELL(gX, gY) \
    (TDATA->cells[gX][gY])


// **************************************************************************** GET_CELL_RANGE

// Get the cell range in variadic table functions with indicator
#define GET_CELL_RANGE \
    if (indicator == TABLE_ALL){ \
        start = GNODE(0, 0); \
        end = Grid_LastNode(TDATA->size); \
    }else{ \
        va_list argPtr; \
        va_start(argPtr, indicator); \
        if (indicator == TABLE_CELL){ \
            start.x = end.x = va_arg(argPtr, int); \
            start.y = end.y = va_arg(argPtr, int); \
        }else if (indicator == TABLE_COLUMN){ \
            start.x = end.x = va_arg(argPtr, int); \
            start.y = 0; \
            end.y = TDATA->size.nRows - 1; \
        }else if (indicator == TABLE_ROW){ \
            start.y = end.y = va_arg(argPtr, int); \
            start.x = 0; \
            start.x = TDATA->size.nCols - 1;\
        }else{ \
            va_end(argPtr); \
            return; \
        }; \
        va_end(argPtr); \
        if (!Grid_NodeIsInGrid(start, TDATA->size) || !Grid_NodeIsInGrid(end, TDATA->size)){ \
            return; \
        } \
    }


// **************************************************************************** FOR_CELL_RANGE

// Do for all cells from node start to node end
#define FOR_CELL_RANGE(gCommand) \
    for (int x = start.x; x <= end.x; x++){ \
        for (int y = start.y; y <= end.y; y++){ \
            gCommand; \
        } \
    }


// ============================================================================ PRIVATE CONSTANTS

#define TABLE_MARGIN_RATIO                  0.1f
#define TABLE_MIN_MARGIN                    5.0f
#define TABLE_MAX_MARGIN                    15.0f

#define COL_TABLE_FG                        COL_UI_FG_SECONDARY


// ============================================================================ PRIVATE STRUCTURES

// **************************************************************************** TableData

// The data object of the table gadget
typedef struct TableData{
    Grid size;
    Gadget*** cells;
}TableData;


// ============================================================================ PRIVATE FUNC DECL

void            Table_PrepareToFree(Gadget* table);
void            Table_Resize(Gadget* table);
void            Table_ResizeAfterSubgadgets(Gadget* table);
float           Table_GetMinFontSize(const Gadget* table);
void            Table_EqualizeFontSizes(Gadget* table);
void            Table_Draw(const Gadget* table, Vector2 shift);
#ifdef DEBUG_MODE
    void        Table_PrintData(const Gadget* table);
#endif






// ============================================================================ FUNC DEF

// **************************************************************************** Table_Make

// Make a table gadget. It needs to be resized before its first use
Gadget* Table_Make(E_GadgetID id, int nCols, int nRows, const char** strings){
    Gadget* table = Gadget_Make(id, GT_TABLE, IS_NOT_SELECTABLE, 0);


    table->Resize                = Table_Resize;
    table->ResizeAfterSubgadgets = Table_ResizeAfterSubgadgets;
    table->Draw                  = Table_Draw;
    #ifdef DEBUG_MODE
        table->PrintData         = Table_PrintData;
    #endif

    TableData* data = Memory_Allocate(NULL, sizeof(TableData), ZEROVAL_ALL);
    table->data = data;

    nCols = MAX(nCols, 1);
    nRows = MAX(nRows, 1);
    data->size = GRID0(nCols, nRows);

    MAKE_2D_ARR(data->cells, nCols, nRows, Gadget*, ZEROVAL_ALL);

    int count = 0;
    for (int y = 0; y < nRows; y++){
        for (int x = 0; x < nCols; x++){
            data->cells[x][y] = Label_Make(id, strings[count], COL_TABLE_FG, RP_CENTER);
            count++;
        }
    }

    return table;
}


// **************************************************************************** Table_SetText

// Set the text at the given cell
void Table_SetText(Gadget* table, int nCols, int nRows, const char* txt){
    if (!Grid_NodeIsInGrid(GNODE(nCols, nRows), TDATA->size)){
        return;
    }

    Label_SetText(TDATA->cells[nCols][nRows], txt);

    Table_EqualizeFontSizes(table);
}


// **************************************************************************** Table_SetFontSize

// Set the font size at the given cell(s)
void Table_SetFontSize(Gadget* table, float fontSize, int indicator, ...){
    GNode start, end;
    GET_CELL_RANGE

    FOR_CELL_RANGE(Label_SetFontSize(TDATA->cells[x][y], fontSize))
}


// **************************************************************************** Table_SetColor

// Set the color of the labels at the given cell(s)
void Table_SetColor(Gadget* table, Color color, int indicator, ...){
    GNode start, end;
    GET_CELL_RANGE

    FOR_CELL_RANGE(Label_SetColor(TDATA->cells[x][y], color))
}


// **************************************************************************** Table_SetAlignment

// Set the alignment of the labels at the given cell(s)
void Table_SetAlignment(Gadget* table, E_RectPointType alignment, int indicator, ...){
    GNode start, end;
    GET_CELL_RANGE

    FOR_CELL_RANGE(Label_SetAlignment(TDATA->cells[x][y], alignment))
}


// **************************************************************************** Table_GetText

// Return the text of the label at the given cell
const char* Table_GetText(const Gadget* table, int nCols, int nRows){
    if (!Grid_NodeIsInGrid(GNODE(nCols, nRows), TDATA->size)){
        return NULL;
    }

    return Label_GetText(TDATA->cells[nCols][nRows]);
}


// **************************************************************************** Table_GetFontSize

// Return the font size of the label at the given cell
float Table_GetFontSize(const Gadget* table, int nCols, int nRows){
    if (!Grid_NodeIsInGrid(GNODE(nCols, nRows), TDATA->size)){
        return INVALID;
    }

    return Label_GetFontSize(TDATA->cells[nCols][nRows]);
}


// **************************************************************************** Table_GetColor

// Return the color of the label at the given cell
Color Table_GetColor(const Gadget* table, int nCols, int nRows){
    if (!Grid_NodeIsInGrid(GNODE(nCols, nRows), TDATA->size)){
        return COL_NULL;
    }

    return Label_GetColor(TDATA->cells[nCols][nRows]);
}


// **************************************************************************** Table_GetAlignment

// Return the alignment of the label at the given cell
int Table_GetAlignment(const Gadget* table, int nCols, int nRows){
    if (!Grid_NodeIsInGrid(GNODE(nCols, nRows), TDATA->size)){
        return INVALID;
    }

    return Label_GetAlignment(TDATA->cells[nCols][nRows]);
}






// ============================================================================ PRIVATE FUNC DEF

// **************************************************************************** Table_PrepareToFree

// Free all the cell labels in the table
void Table_PrepareToFree(Gadget* table){
    for (int y = 0; y < TDATA->size.nRows; y++){
        for (int x = 0; x < TDATA->size.nCols; x++){
            TDATA->cells[x][y] = Gadget_Free(TDATA->cells[x][y]);
        }
    }

    FREE_2D_ARR(TDATA->cells, TDATA->size.nCols);
}


// **************************************************************************** Table_Resize

// Resize the table. All cells have the same size and internal margin
void Table_Resize(Gadget* table){
    float cellWidth  = table->cRect.width  / (float) TDATA->size.nCols;
    float cellHeight = table->cRect.height / (float) TDATA->size.nRows;
    Size cellSize = SIZE(cellWidth, cellHeight);

    Point origin = RORIGIN(table->cRect);
    Point cursor = origin;

    for (int y = 0; y < TDATA->size.nRows; y++){
        for (int x = 0; x < TDATA->size.nCols; x++){
            Rect outerRect = Geo_SetRectPS(cursor, cellSize, RP_TOP_LEFT);
            float margin = MIN_DIM(outerRect) * TABLE_MARGIN_RATIO;
            margin = PUT_IN_RANGE(margin, TABLE_MIN_MARGIN, TABLE_MAX_MARGIN);
            Rect innerRect = Geo_ApplyRectMargins(outerRect, margin);
            TDATA->cells[x][y]->cRect = innerRect;
            Gadget_Resize(TDATA->cells[x][y]);

            cursor.x += cellWidth;
        }
        cursor.x = origin.x;
        cursor.y += cellHeight;
    }
}


// **************************************************************************** Table_ResizeAfterSubgadgets

// Equalize the font size of all the labels
void Table_ResizeAfterSubgadgets(Gadget* table){
    Table_EqualizeFontSizes(table);
}


// **************************************************************************** Table_GetMinFontSize

// Get the minimum label font size in the table
float Table_GetMinFontSize(const Gadget* table){
    float minFontSize = INVALID;

    for (int y = 0; y < TDATA->size.nRows; y++){
        for (int x = 0; x < TDATA->size.nCols; x++){
            float fontSize = Label_GetFontSize(TDATA->cells[x][y]);
            if (minFontSize <= 0.0f || fontSize < minFontSize){
                minFontSize = fontSize;
            }
        }
    }

    return minFontSize;
}


// **************************************************************************** Table_EqualizeFontSizes

// Set the same font size in all labels in the table
void Table_EqualizeFontSizes(Gadget* table){
    float fontSize = Table_GetMinFontSize(table);

    Table_SetFontSize(table, fontSize, TABLE_ALL);
}


// **************************************************************************** Table_Draw

// Draw all the cell labels in the table
void Table_Draw(const Gadget* table, Vector2 shift){
    for (int y = 0; y < TDATA->size.nRows; y++){
        for (int x = 0; x < TDATA->size.nCols; x++){
            Gadget_Draw(TDATA->cells[x][y], shift);
        }
    }
}


#ifdef DEBUG_MODE
// **************************************************************************** Table_PrintData

    // Multiline print of the parameters of the table's data object
    void Table_PrintData(const Gadget* table){
        CHECK_NULL(table, WITH_NEW_LINE)
        CHECK_NULL(table->data, WITH_NEW_LINE)

        printf("Size: "); Grid_Print(TDATA->size, WITH_NEW_LINE);
        printf("\n");

        printf("CELLS:\n");
        CHECK_NULL(TDATA->cells, WITH_NEW_LINE)
        for (int y = 0; y < TDATA->size.nRows; y++){
            for (int x = 0; x < TDATA->size.nCols; x++){
                printf("   %2d, %2d: ", x, y);
                printf("\"%s\" | ", Label_GetText(TDATA->cells[x][y]));
                Geo_PrintRect(TDATA->cells[x][y]->cRect, WITHOUT_NEW_LINE);
                printf(" | %s\n", RectPointType_ToString(Label_GetAlignment(TDATA->cells[x][y])));
            }
        }
    }
#endif




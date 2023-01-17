// ============================================================================
// RODS
// Persistent Data
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions for reading and writing data to the data file.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"
#include "../Fund/Fund.h"
#include "../Logic/Logic.h"
#include "../Graph/Graph.h"
#include "Store.h"
#include "File_Internal.h"


// ============================================================================ FILE STRUCTURE
/*
    1) HEADER:
        File Identifier         1 x String                FILE_IDENTIFIER
        Version                 1 x String                APP_VERSION
        Seperator               1 x Byte                  FILE_SEP

    2) RECORDS:
        Num of Records (NR)     1 x Int32   
        Records                 NR x Record (4 Bytes)
        Seperator               1 x Byte                  FILE_SEP
            Record:
                Column   1 x Byte
                Row      1 x Byte
                Time     1 x Time (2 bytes)

    3) ROD GRID:
        Exists                  1 x Byte                  0:false, 1:false
        Num of Columns          1 x Byte
        Num of Rows             1 x Byte
        Source Column           1 x Byte
        Source Row              1 x Byte
        Leg Data:               Num of Rods / 2           Two rods per byte: RDLU
        Seperator               1 x Byte                  FILE_SEP

    4) SCROLL GRAPHICS:
        Virtual Screen Width    1 x Float (4 Bytes)
        Virtual Screen Height   1 x Float (4 Bytes)
        Viewport X              1 x Float (4 Bytes)
        Viewport Y              1 x Float (4 Bytes)
        Viewport Width          1 x Float (4 Bytes)
        Viewport Height         1 x Float (4 Bytes)
        Seperator               1 x Byte                  FILE_SEP

    5) TIME:
        Time                    1 x Time (2 Bytes)
        Seperator               1 x Byte                  FILE_SEP

    6) SOUND
        Sound On/Off            1 x Byte                  0:false, 1:true
        Seperator               1 x Byte                  FILE_SEP

    7) TERMINATOR
        Seperator               1 x Byte                  FILE_SEP

*/


// ============================================================================ PRIVATE MACROS

// **************************************************************************** OPEN_FILE

// Open the file for reading ("r") or writing ("w")
#define OPEN_FILE(gMode, gReturn) \
    if (Glo_FilePath == NULL){ \
        Glo_FilePath = Path_GetDataFile(); \
        if (Glo_FilePath == NULL) {return gReturn;} \
    } \
    file = fopen(Glo_FilePath, gMode); \
    if (file == NULL){ \
        return gReturn; \
    }


// ============================================================================ PRIVATE CONSTANTS

#define FILE_IDENTIFIER                     "RODS"


// ============================================================================ PRIVATE FUNC DECL

bool            PData_WriteHeader(FILE* file);
bool            PData_ReadHeader(char** version, FILE* file);
bool            PData_WriteRecords(const Records* records, FILE* file);
bool            PData_ReadRecords(Records** records, FILE* file);
bool            PData_WriteRGrid(const RGrid* rGrid, FILE* file);
bool            PData_ReadRGrid(RGrid** rGrid, FILE* file);
bool            PData_WriteSGraph(const SGraph* sg, FILE* file);
bool            PData_ReadSGraph(SGraph** sg, Grid grid, FILE* file);
bool            PData_WriteTime(Time time, FILE* file);
bool            PData_ReadTime(Time* time, FILE* file);
bool            PData_WriteSound(bool sound, FILE* file);
bool            PData_ReadSound(bool* sound, FILE* file);






// ============================================================================ FUNC DEF

// **************************************************************************** PData_MakeEmpty

// Make an empty persistent data structure
PData* PData_MakeEmpty(void){
    PData* pData = Memory_Allocate(NULL, sizeof(PData), ZEROVAL_ALL);

    pData->time = TIME_INVALID;

    return pData;
}


// **************************************************************************** PData_Free

// Free the persistent data structure. Return NULL
PData* PData_Free(PData* pData){
    if (pData == NULL) {return NULL;}

    pData->version = Memory_Free(pData->version);
    pData->records = Records_Free(pData->records);
    pData->rGrid = RGrid_Free(pData->rGrid);
    pData->sg = SGraph_Free(pData->sg);

    return Memory_Free(pData);
}


// **************************************************************************** PData_WriteToFile

// Write the peristent data to the data file. Return true if successful
bool PData_WriteToFile(const Records* records, const RGrid* rGrid, const SGraph* sg, Time time, bool sound){

    #define TRY(gFunc) if(!gFunc) {res = false; goto LAB_PDATA_WRITE_EXIT;}
    
    FILE* file = NULL;
    OPEN_FILE("w", false)

    bool res = true;
    
    TRY(PData_WriteHeader(file))
    
    TRY(PData_WriteRecords(records, file))
    
    if (rGrid == NULL || sg == NULL || !Time_IsValid(time)){
        File_WriteByte(0, file);
        goto LAB_PDATA_WRITE_EXIT;
    }else{
        TRY(File_WriteByte(1, file))
    }
    
    TRY(PData_WriteRGrid(rGrid, file))
    
    TRY(PData_WriteSGraph(sg, file))
    
    TRY(PData_WriteTime(time, file))
    
    TRY(PData_WriteSound(sound, file))
    
    TRY(File_WriteSep(file))

    LAB_PDATA_WRITE_EXIT:

    fclose(file);
    return res;

    #undef TRY
}


// **************************************************************************** PData_ReadFromFile

// Read the persistent data from the file. Return NULL if it fails
PData* PData_ReadFromFile(void){

    #define TRY(gFunc) if(!gFunc) {success = false; goto LAB_PDATA_READ_EXIT;}

    FILE* file = NULL;
    OPEN_FILE("r", NULL)

    PData* pData = PData_MakeEmpty();
    
    bool success = true;

    TRY(PData_ReadHeader(&(pData->version), file))
    
    TRY(PData_ReadRecords(&(pData->records), file))
    
    unsigned char exists = 0;
    TRY(File_ReadByte(&exists, file))
    if (exists == 0){
        goto LAB_PDATA_READ_EXIT;
    }
    
    TRY(PData_ReadRGrid(&(pData->rGrid), file))
    
    Grid grid = RGrid_GetSize(pData->rGrid);
    if (!IS_IN_RANGE(grid.nCols, RGRID_MIN_SIZE, RGRID_MAX_SIZE) || 
        !IS_IN_RANGE(grid.nRows, RGRID_MIN_SIZE, RGRID_MAX_SIZE)){
        success = false;
        goto LAB_PDATA_READ_EXIT;
    }
    
    TRY(PData_ReadSGraph(&(pData->sg), grid, file))
    
    TRY(PData_ReadTime(&(pData->time), file))
    
    TRY(PData_ReadSound(&(pData->sound), file));
    
    TRY(File_ReadSep(file))
    

    LAB_PDATA_READ_EXIT:

    fclose(file);
    if (!success){
        pData = PData_Free(pData);
    }
    return pData;

    #undef TRY
}


#ifdef DEBUG_MODE
// **************************************************************************** PData_Print

    // Multiline print of the persistent data
    void PData_Print(PData* pData){
        CHECK_NULL(pData, WITH_NEW_LINE);
        
        PRINT_LINE

        printf("Version: %s\n", pData->version);
        PRINT_LINE

        Records_Print(pData->records);
        PRINT_LINE

        RGrid_Print(pData->rGrid);
        PRINT_LINE

        SGraph_Print(pData->sg);
        PRINT_LINE

        printf("Time: "); Time_Print(pData->time, WITH_NEW_LINE);
        PRINT_LINE

        printf("Sound: %s\n", Bool_ToString(pData->sound, LONG_FORM));
        PRINT_LINE
    }   
#endif






// ============================================================================ PRIVATE FUNC DEF

// **************************************************************************** PData_WriteHeader

// Write the header to the file. Return true if successful
bool PData_WriteHeader(FILE* file){
    if (!File_WriteString(FILE_IDENTIFIER, file)) {return false;}
    if (!File_WriteString(APP_VERSION, file)) {return false;}
    return File_WriteSep(file);
}


// **************************************************************************** PData_ReadHeader

// Read the header and save the version at the given pointer, if not NULL. 
// Return true if successful
bool PData_ReadHeader(char** version, FILE* file){
    #define TRY(gFunc) if (!gFunc) {str = Memory_Free(str); return false;}

    char* str = NULL;

    TRY(File_ReadString(&str, file))
    TRY(String_IsEqual(str, FILE_IDENTIFIER))

    TRY(File_ReadString(&str, file))

    if (version != NULL){
        *version = String_Copy(*version, str);
    }
    str = Memory_Free(str);

    TRY(File_ReadSep(file))

    return true;

    #undef TRY
}


// **************************************************************************** PData_WriteRecords

// Write the records to the file. Return true if successful
bool PData_WriteRecords(const Records* records, FILE* file){
    if (records == NULL) {return false;}

    int n = Records_N(records);
    if (!File_WriteInt(n, 4, file)) {return false;}

    int count = 0;
    for (int y = RGRID_MIN_SIZE; y <= RGRID_MAX_SIZE; y++){
        for (int x = RGRID_MIN_SIZE; x <= RGRID_MAX_SIZE; x++){
            Time t = Records_Get(records, x, y);
            if (Time_IsValid(t)){
                count++;
                if (count > n) {return false;}
                if (!File_WriteByte(x, file)) {return false;}
                if (!File_WriteByte(y, file)) {return false;}
                if (!File_WriteTime(t, file)) {return false;}
            }
        }
    }

    return true;
}


// **************************************************************************** PData_ReadRecords

// Read the records from the file and save them at the given pointer. Return 
// true if successful
bool PData_ReadRecords(Records** records, FILE* file){
    #define TRY(gFunc) if (!gFunc) {*records = Memory_Free(*records); return false;}

    if (records == NULL) {return false;}
    if (*records != NULL) {*records = Records_Free(*records);}

    *records = Records_Make();
    
    int n = 0;
    TRY(File_ReadInt(&n, 4, file))

    for (int i = 0; i < n; i++){
        unsigned char x, y;
        Time t;
        TRY(File_ReadByte(&x, file))
        TRY(File_ReadByte(&y, file))
        TRY(File_ReadTime(&t, file))
        Records_Set(*records, t, x, y);
    }

    return true;

    #undef TRY
}


// **************************************************************************** PData_WriteRGrid

// Write the rod grid to the file. Return true if successful
bool PData_WriteRGrid(const RGrid* rGrid, FILE* file){
    if (rGrid == NULL) {return false;}

    Grid size = RGrid_GetSize(rGrid);
    if (!File_WriteByte(size.nCols, file)) {return false;}
    if (!File_WriteByte(size.nRows, file)) {return false;}

    GNode source = RGrid_GetSource(rGrid);
    if (!File_WriteByte(source.x, file)) {return false;}
    if (!File_WriteByte(source.y, file)) {return false;}

    GNode node = GNODE_NULL;
    do{
        unsigned char byte = 0;

        byte = (RGrid_GetRod(rGrid, node)->legs << 4);
        node = Grid_NextNode(node, size);

        if (!Grid_NodesAreEqual(node, GNODE_NULL)){
            byte += (RGrid_GetRod(rGrid, node)->legs);
            node = Grid_NextNode(node, size);
        }

        if (!File_WriteByte(byte, file)) {return false;}
    }while (!Grid_NodesAreEqual(node, GNODE_NULL));

    if (!File_WriteSep(file)) {return false;}

    return true;
}


// **************************************************************************** PData_ReadRGrid

// Read the rod grid from the file. Save it in rGrid. Return true if successful
bool PData_ReadRGrid(RGrid** rGrid, FILE* file){
    #define TRY(gFunc) if (!gFunc) {*rGrid = RGrid_Free(*rGrid); return false;}

    if (rGrid == NULL) {return false;}
    if (*rGrid != NULL) {*rGrid = RGrid_Free(*rGrid);}

    unsigned char nCols, nRows;
    TRY(File_ReadByte(&nCols, file))
    TRY(File_ReadByte(&nRows, file))
    
    Grid size = GRID0(nCols, nRows);
    if (!IS_IN_RANGE(nCols, RGRID_MIN_SIZE, RGRID_MAX_SIZE) || 
        !IS_IN_RANGE(nRows, RGRID_MIN_SIZE, RGRID_MAX_SIZE)){
        return false;
    }

    unsigned char sourceX, sourceY;
    TRY(File_ReadByte(&(sourceX), file))
    TRY(File_ReadByte(&(sourceY), file))

    GNode source = GNODE(sourceX, sourceY);
    if (!Grid_NodeIsInGrid(source, size)){
        return false;
    }

    *rGrid = RGrid_MakeEmpty(nCols, nRows);
    RGrid_SetSource(*rGrid, source);

    GNode node = GNODE_NULL;
    do{
        unsigned char byte;
        TRY(File_ReadByte(&byte, file));

        int legs = ((byte >> 4) & 0xF);
        RGrid_SetRod(*rGrid, node, legs);
        node = Grid_NextNode(node, size);

        if (!Grid_NodesAreEqual(node, GNODE_NULL)){
            legs = (byte & 0xF);
            RGrid_SetRod(*rGrid, node, legs);
            node = Grid_NextNode(node, size);
        }
    }while (!Grid_NodesAreEqual(node, GNODE_NULL));

    TRY(File_ReadSep(file))

    RGrid_Electrify(*rGrid, GNODE_INVALID);

    return true;

    #undef TRY
}


// **************************************************************************** PData_WriteSGraph

// Write the scroll graphics object to the file. Return true, if successful
bool PData_WriteSGraph(const SGraph* sg, FILE* file){
    if (sg == NULL) {return false;}

    Size vScreen = SGraph_GetVScreen(sg);
    if (!File_WriteFloat(vScreen.width, file)) {return false;}
    if (!File_WriteFloat(vScreen.height, file)) {return false;}

    Rect viewport = SGraph_GetViewport(sg);
    if (!File_WriteFloat(viewport.x, file)) {return false;}
    if (!File_WriteFloat(viewport.y, file)) {return false;}
    if (!File_WriteFloat(viewport.width, file)) {return false;}
    if (!File_WriteFloat(viewport.height, file)) {return false;}

    if (!File_WriteSep(file)) {return false;}

    return true;
}


// **************************************************************************** PData_ReadSGraph

// Read a scroll graphics object from the file and save it at the given 
// pointer. Return true if successful
bool PData_ReadSGraph(SGraph** sg, Grid grid, FILE* file){
    #define TRY(gFunc) if (!gFunc) {*sg = SGraph_Free(*sg); return false;}

    if (sg == NULL) {return false;}
    if (*sg != NULL) {return *sg = SGraph_Free(*sg);}

    float x, y, w, h;

    TRY(File_ReadFloat(&w, file))
    TRY(File_ReadFloat(&h, file))
    
    Size vScreen = SIZE(w, h);
    vScreen = Grid_FitSizeToGrid(vScreen, grid);
    float tileSize = Grid_CalcSqrCellSize(grid, vScreen);
    if (!IS_IN_RANGE(tileSize, ROD_MIN_TEXTURE_SIZE, ROD_MAX_TEXTURE_SIZE)){
        return false;
    }

    TRY(File_ReadFloat(&x, file))
    TRY(File_ReadFloat(&y, file))
    TRY(File_ReadFloat(&w, file))
    TRY(File_ReadFloat(&h, file))

    TRY(File_ReadSep(file))

    Rect viewport = RECT(x, y, w, h);

    *sg = SGraph_MakeFromGrid(grid, tileSize, TO_RECT(RSIZE(viewport)), SGRAPH_DEF_MARGIN, 
                              ROD_MIN_TEXTURE_SIZE, ROD_MAX_TEXTURE_SIZE);

    SGraph_SetVScreen(*sg, vScreen);
    SGraph_PlaceViewport(*sg, RORIGIN(viewport), RP_TOP_LEFT);

    return true;

    #undef TRY
}


// **************************************************************************** PData_WriteTime

// Write the time to the file. Return true if successful
bool PData_WriteTime(Time time, FILE* file){
    if (!Time_IsValid(time)) {return false;}
    if (!File_WriteTime(time, file)) {return false;}
    return File_WriteSep(file);
}


// **************************************************************************** PData_ReadTime

// Read the time from the file and save it at the given pointer. Return true if 
// successful
bool PData_ReadTime(Time* time, FILE* file){
    if (!File_ReadTime(time, file)) {return false;}
    return File_ReadSep(file);
}


// **************************************************************************** PData_WriteSound

// Write the sound boolean (0 or 1) to the file. Return true if successful
bool PData_WriteSound(bool sound, FILE* file){
    if (!File_WriteByte(sound ? 1 : 0, file)) {return false;}
    return File_WriteSep(file);
}


// **************************************************************************** PData_ReadSound

// Read the sound indicator from the file and save it at the given pointer. 
// Return true if successful
bool PData_ReadSound(bool* sound, FILE* file){
    unsigned char byte;
    File_ReadByte(&byte, file);
    if (byte == 0) {*sound = false; return true;}
    if (byte == 1) {*sound = true;  return true;}
    return false;
}









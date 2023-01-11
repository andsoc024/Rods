// ============================================================================
// RODS
// Store Header
// by Andreas Socratous
// Dec 2022
// ============================================================================


#ifndef STORE_GUARD
#define STORE_GUARD


// ============================================================================ INFO
/*
    Functions for determining the file and folder paths as well as loading data 
    from and saving data to the data file.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"


// ============================================================================ FUNC DECL

// ---------------------------------------------------------------------------- Path Functions

#ifdef PLATFORM_MACOS
    void        Path_GetHomeDir_MacOS(char* dst);
#endif
Paths*          Path_Make(void);
Paths*          Path_Free(Paths* paths);
void            Path_MakeDefault(void);
void            Path_FreeDefault(void);
char*           Path_GetHomeDir(char* dst);
char*           Path_GetSystemAppDir(char* dst);
char*           Path_GetAppDir(char* dst);
char*           Path_GetDataFile(char* dst);
char*           Path_AddPathComponent(char* dst, char* root, char* addOn);
bool            Path_FileExists(char* path);
bool            Path_DirExists(char* path);
bool            Path_CreateDir(char* path);
#ifdef DEBUG_MODE
    void        Path_Print(Paths* paths);
#endif

// ---------------------------------------------------------------------------- File Functions

bool            File_WriteBytes(Bytes bytes, FILE* file);
bool            File_ReadBytes(Bytes* bytes, int nBytes, FILE* file);
bool            File_WriteChar(char chr, FILE* file);
bool            File_ReadChar(char* chr, FILE* file);
bool            File_WriteString(const char* str, FILE* file);
bool            File_ReadString(char** str, FILE* file);
bool            File_WriteInt(unsigned int num, int nBytes, FILE* file);
bool            File_ReadInt(unsigned int* num, int nBytes, FILE* file);
bool            File_WriteFloat(float num, FILE* file);
bool            File_ReadFloat(float* num, FILE* file);
bool            File_WriteSep(FILE* file);
bool            File_ReadSep(FILE* file);
bool            File_WriteFileIdentifier(FILE* file);
bool            File_ReadFileIdentifier(FILE* file);
bool            File_WriteVersion(FILE* file);
bool            File_ReadVersion(char** dst, FILE* file);
bool            File_WriteHeader(FILE* file);
bool            File_ReadHeader(char** versionStr, FILE* file);
bool            File_WriteRecords(const Records* records, FILE* file);
Records*        File_ReadRecords(FILE* file);
bool            File_WriteRGrid(const RGrid* rGrid, FILE* file);
RGrid*          File_ReadRGrid(FILE* file);
bool            File_WriteSGraph(const SGraph* sg, FILE* file);
SGraph*         File_ReadSGraph(int nCols, int nRows, FILE* file);
bool            File_WriteData(const Records* records, const RGrid* rGrid, const SGraph* sg);
FileData*       File_ReadData(void);
FileData*       FileData_Make(void);
FileData*       FileData_Free(FileData* fData);
#ifdef DEBUG_MODE
    void        FileData_Print(FileData* fData);
#endif



#endif // STORE_GUARD


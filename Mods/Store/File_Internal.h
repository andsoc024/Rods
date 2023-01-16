// ============================================================================
// RODS
// File Internal Header
// by Andreas Socratous
// Jan 2023
// ============================================================================


#ifndef FILE_GUARD
#define FILE_GUARD


// ============================================================================ INFO
/*
    Functions for reading and writing elementary types in the file.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"


// ============================================================================ FUNC DECL

bool            File_WriteByte(unsigned char b, FILE* file);
bool            File_ReadByte(unsigned char* b, FILE* file);
bool            File_WriteChar(char c, FILE* file);
bool            File_ReadChar(char* c, FILE* file);
bool            File_WriteString(const char* str, FILE* file);
bool            File_ReadString(char** str, FILE* file);
bool            File_WriteBytes(Bytes bytes, FILE* file);
bool            File_ReadBytes(Bytes* bytes, int nBytes, FILE* file);
bool            File_WriteInt(int num, int nBytes, FILE* file);
bool            File_ReadInt(int* num, int nBytes, FILE* file);
bool            File_WriteFloat(float num, FILE* file);
bool            File_ReadFloat(float* num, FILE* file);
bool            File_WriteTime(Time t, FILE* file);
bool            File_ReadTime(Time* t, FILE* file);
bool            File_WriteSep(FILE* file);
bool            File_ReadSep(FILE* file);



#endif // FILE_GUARD


// ============================================================================
// RODS
// File
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions for reading and writing elementary data types to a file.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"
#include "../Fund/Fund.h"
#include "Store.h"
#include "File_Internal.h"


// ============================================================================ PRIVATE CONSTANTS

#define FILE_SEP                            0x10






// ============================================================================ FUNC DEF

// **************************************************************************** File_WriteByte

// Write a byte to the file. Return true if successful
bool File_WriteByte(unsigned char b, FILE* file){
    int res = fputc(b, file);
    return (res != EOF);
}


// **************************************************************************** File_ReadByte

// Read a byte from the file and sve it at b. Return true if successful
bool File_ReadByte(unsigned char* b, FILE* file){
    int res = fgetc(file);
    if (res == EOF) {
        *b = 0;
        return false;
    }
    *b = (unsigned int) res;
    return true;
}


// **************************************************************************** File_WriteChar

// Write the character to the file. Return true if successful
bool File_WriteChar(char c, FILE* file){
    int v = (int) c + 128;
    return File_WriteByte(v, file);
}


// **************************************************************************** File_ReadChar

// Read a character from the file and store it in c. Return true if successful
bool File_ReadChar(char* c, FILE* file){
    unsigned char b;
    if (!File_ReadByte(&b, file)){
        *c = 0;
        return false;
    }

    int v = (int) b - 128;
    *c = (char) v;

    return true;
}


// **************************************************************************** File_WriteString

// Write the string to the file. Return true if successful
bool File_WriteString(const char* str, FILE* file){
    if (str == NULL) {return true;}

    for (; *str != '\0'; str++){
        if (!File_WriteChar(*str, file)){
            return false;
        }
    }

    if (!File_WriteChar('\0', file)){
        return false;
    }

    return true;
}


// **************************************************************************** File_ReadString

// Read a string from the file ans store it in str. Return true if successful
bool File_ReadString(char** str, FILE* file){
    if (str == NULL) {return false;}

    char* temp = Memory_Allocate(NULL, STR_DEF_LENGTH, ZEROVAL_ALL);
    char* ptr = temp;

    for (int i = 0; i < STR_DEF_LENGTH; i++){
        if (!File_ReadChar(ptr, file)){
            temp = Memory_Free(temp);
            return false;
        }

        if (*ptr == '\0'){
            *str = String_Copy(*str, temp);
            temp = Memory_Free(temp);
            return true;
        }

        ptr++;
    }

    temp = Memory_Free(temp);
    *str = Memory_Free(*str);
    return false;
}


// **************************************************************************** File_WriteBytes

// Write 1-4 bytes to the file. Return true if successful
bool File_WriteBytes(Bytes bytes, FILE* file){
    if (!Bytes_IsValid(bytes)){
        return false;
    }

    for (int i = 0; i < bytes.n; i++){
        if (!File_WriteByte(bytes.values[i], file)){
            return false;
        }
    }

    return true;
}


// **************************************************************************** File_ReadBytes

// Read 1-4 bytes from the file and store them at the given pointer. Return 
// true if successful
bool File_ReadBytes(Bytes* bytes, int nBytes, FILE* file){
    nBytes = PUT_IN_RANGE(nBytes, 1, 4);

    bytes->n = nBytes;

    for (int i = 0; i < nBytes; i++){
        if (!File_ReadByte(&(bytes->values[i]), file)){
            *bytes = BYTES_NULL;
            return false;
        }
    }

    return true;
}


// **************************************************************************** File_WriteInt

// Write the number to the file, with the given number of bytes. return true if 
// successful
bool File_WriteInt(int num, int nBytes, FILE* file){
    Bytes bytes = Bytes_FromInt(num, nBytes);
    return File_WriteBytes(bytes, file);
}


// **************************************************************************** File_ReadInt

// Read an integer, with the given number of bytes, from the file. Return true 
// if successful
bool File_ReadInt(int* num, int nBytes, FILE* file){
    Bytes bytes;
    if (!File_ReadBytes(&bytes, nBytes, file)){
        *num = 0;
        return false;
    }

    *num = Bytes_ToInt(bytes);
    return true;
}


// **************************************************************************** File_WriteFloat

// Write the float as 4 bytes to the file. Return true if successful
bool File_WriteFloat(float num, FILE* file){
    Bytes bytes = Bytes_FromFloat(num);
    return File_WriteBytes(bytes, file);
}


// **************************************************************************** File_ReadFloat

// Read a float, as 4 bytes, from the file and store it at num. Return true if 
// successful
bool File_ReadFloat(float* num, FILE* file){
    Bytes bytes;
    if (!File_ReadBytes(&bytes, 4, file)){
        *num = 0.0f;
        return false;
    }

    *num = Bytes_ToFloat(bytes);
    return true;
}


// **************************************************************************** File_WriteTime

// Write the time as 2 bytes to the file. Return true if successful
bool File_WriteTime(Time t, FILE* file){
    if (!Time_IsValid(t)) {return false;}
    int num = Time_ToInt(t);
    return File_WriteInt(num, 2, file);
}


// **************************************************************************** File_ReadTime

// Read a time, as 2 bytes, from the file and store it at t. Return true, if 
// successful
bool File_ReadTime(Time* t, FILE* file){
    int num;
    if (!File_ReadInt(&num, 2, file)){
        *t = TIME_INVALID;
        return false;
    }

    *t = Time_FromInt(num);
    return true;
}


// **************************************************************************** File_WriteSep

// Write the FILE_SEP byte to the file. Return true if successful
bool File_WriteSep(FILE* file){
    return File_WriteByte(FILE_SEP, file);
}


// **************************************************************************** File_ReadSep

// Return true if the next byte in the file is FILE_SEP
bool File_ReadSep(FILE* file){
    unsigned char b;
    if (!File_ReadByte(&b, file)){
        return false;
    }

    return (b == FILE_SEP);
}



// ============================================================================
// RODS
// MacOS Specific Functions
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions specific for the MacOS platform.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>

#include <stdbool.h>
#include <sys/stat.h>

#include "../../../Mods/Public/Public.h"
#include "../../../Mods/Fund/Fund.h"


// ============================================================================ PRIVATE FUNC DECL

void            Path_GetHomeDirForPlatform(char* dst);






// ============================================================================ FUNC DEF

// **************************************************************************** Path_GetHomeDir

// Get the home folder path using the objective C function in HomeDir.m
char* Path_GetHomeDir(char* dst){
    char* temp = Memory_Allocate(NULL, STR_DEF_LENGTH, ZEROVAL_ALL);
    Path_GetHomeDirForPlatform(temp);

    dst = String_Copy(dst, temp);

    temp = Memory_Free(temp);
    
    return dst;
}


// **************************************************************************** Path_MakeDirForPlatform

// Make the folder at the given path
bool Path_MakeDirForPlatform(const char* path){
    int res = mkdir(path, S_IRWXU);

    return (res == 0);
}



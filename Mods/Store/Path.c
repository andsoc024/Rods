// ============================================================================
// RODS
// Path
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions for determining the folder and file paths, as well as making the 
    app's folder.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include <dirent.h>

#include "../Public/Public.h"
#include "../Fund/Fund.h"


// ============================================================================ PRIVATE CONSTANTS

#define PATH_SUPPORT_DIR_NAME               ARCH_DEF(PATH_SUPPORT_DIR_NAME)
#define PATH_APP_DIR_NAME                   ARCH_DEF(PATH_APP_DIR_NAME)
#define PATH_DATA_FILE_NAME                 "RodsData"
#define PATH_SEP                            ARCH_DEF(PATH_SEP)


// ============================================================================ PRIVATE FUNC DECL

char*           Path_GetHomeDir(char* dst);
char*           Path_AddComponent(char* dst, const char* component);
bool            Path_MakeDir(const char* path);
bool            Path_MakeDirForPlatform(const char* path);






// ============================================================================ FUNC DEF

// **************************************************************************** Path_GetDataFile

// Determine the data file path, in the app folder in the application support 
// folder of the operating system. Create the app folder if it does not exist
char* Path_GetDataFile(void){
    char* path = Path_GetHomeDir(NULL);

    path = Path_AddComponent(path, PATH_SUPPORT_DIR_NAME);

    path = Path_AddComponent(path, PATH_APP_DIR_NAME);
    bool res = Path_MakeDir(path);
    if (res == false){
        return Memory_Free(path);
    }

    path = Path_AddComponent(path, PATH_DATA_FILE_NAME);

    return path;
}


// **************************************************************************** Path_FileExists

// Return true if the file exists
bool Path_FileExists(const char* path){
    FILE* file = fopen(path, "r");
    if (file != NULL){
        fclose(file);
        return true;
    }

    return false;
}


// **************************************************************************** Path_DirExists

// Return true if the folder exists
bool Path_DirExists(const char* path){
    DIR* dir = opendir(path);
    if (dir != NULL){
        closedir(dir);
        return true;
    }

    return false;
}






// ============================================================================ PRIVATE FUNC DECL

// **************************************************************************** Path_AddComponent

// Add the path component to dst. If the component is NULL or empty, do nothing
char* Path_AddComponent(char* dst, const char* component){
    if (component == NULL || *component == '\0'){
        return dst;
    }

    if (dst == NULL){
        return String_Copy(NULL, component);
    }

    int dstLength = String_Length(dst);
    const char* sep = (dstLength > 0 && *(component + dstLength - 1) == PATH_SEP[0]) ? NULL : PATH_SEP;

    return String_Concat(dst, 3, dst, sep, component);
}


// **************************************************************************** Path_MakeDir

// Make the folder, if it does not already exist. Return true if the folder 
// already exists or created successfully
bool Path_MakeDir(const char* path){
    if (Path_DirExists(path)){
        return true;
    }

    return Path_MakeDirForPlatform(path);
}



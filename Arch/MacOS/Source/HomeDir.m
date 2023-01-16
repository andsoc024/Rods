// ============================================================================
// RODS
// Home Directory
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Objective C function for getting the path of the home folder in MacOS.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <Cocoa/Cocoa.h>


// ============================================================================ PRIVATE CONSTANTS

#define STR_DEF_LENGTH                      200




// ============================================================================ FUNC DEF

// **************************************************************************** Path_GetHomeDirForPlatform

// Objective C function for getting the path of the home folder in MacOS
void Path_GetHomeDirForPlatform(char* dst){
    NSString* temp = NSHomeDirectory();
    const char* cStr = [temp UTF8String];

    int count = 0;
    char* ptr = dst;
    for (; *cStr != '\0'; cStr++, ptr++){
        count++;
        if (count >= STR_DEF_LENGTH){
            [temp release];
            *dst = '\0';
        }
        *ptr = *cStr;
    }
    *ptr = '\0';

    [temp release];
}




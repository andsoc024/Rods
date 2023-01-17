// ============================================================================
// RODS
// System
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions for error and memory management.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include <stdarg.h>

#include "../Public/Public.h"
#include "Fund.h"


// ============================================================================ FUNC DEF

// **************************************************************************** Err_FatalError

// Print the error message and exit the program with exit code 1
void Err_FatalError(const char* errorMsg){
    printf("ERROR! %s\n", errorMsg);
    exit(1);
}


// **************************************************************************** Err_Assert

// If the condition is false, throw a fatal error
void Err_Assert(int condition, const char* errorMsg){
    if (!condition){
        Err_FatalError(errorMsg);
    }
}






// ---------------------------------------------------------------------------- Memory Functions

// **************************************************************************** Memory_Allocate

// Allocate a memory block at ptr (new pointer if NULL). Eventually set the 
// values to 0 according to the zero values mode
void* Memory_Allocate(void* ptr, int size, E_ZeroValMode zeroValMode){
    Err_Assert(size > 0, "Invalid memory size");

    if (ptr == NULL){
        #ifdef MEMORY_TRACK
            Glo_AllocCount++;
        #endif
        ptr = malloc(size);
    }else{
        ptr = realloc(ptr, size);
    }

    Err_Assert(ptr != NULL, "Failed to allocate memory");

    switch(zeroValMode){
        case ZEROVAL_ALL:{
            Memory_Set(ptr, size, 0);
            break;
        }

        case ZEROVAL_LAST:{
            *((unsigned char*) ptr + size - 1) = 0;
            break;
        }

        default: {break;}
    }

    return ptr;
}


// **************************************************************************** Memory_Copy

// Copy the memory block from src to dst
void* Memory_Copy(void* dst, const void* src, int size){
    if (dst != src){
        dst = Memory_Allocate(dst, size, ZEROVAL_NONE);
        Memory_Write(dst, src, size);
    }

    return dst;
}


// **************************************************************************** Memory_Write

// Copy the memory block from src to dst, blindly, without allocating memory
void Memory_Write(void* dst, const void* src, int size){
    for (int i = 0; i < size; i++){
        *((unsigned char*) dst + i) = *((unsigned char*) src + i);
    }
}


// **************************************************************************** Memory_Set

// Set the values of the memory block to the given value
void Memory_Set(void* ptr, int size, unsigned char value){
    for (int i = 0; i < size; i++){
        *((unsigned char*) ptr + i) = value;
    }
}


// **************************************************************************** Memory_Free

// Free the memory at the given pointer. Return NULL
void* Memory_Free(void* ptr){
    if (ptr != NULL){
        #ifdef MEMORY_TRACK
            Glo_FreeCount++;
        #endif
        free(ptr);
    }

    return NULL;
}


// **************************************************************************** Memory_FreeAll

// Free the memory at all the given pointers. The arguments must be in the form 
// of pointers to pointers (void**), so that they can be set to NULL
void Memory_FreeAll(int nObjects, ...){
    if (nObjects <= 0) {return;}

    va_list argPtr;

    va_start(argPtr, nObjects);
    for (int i = 0; i < nObjects; i++){
        void** temp = va_arg(argPtr, void**);
        if (temp == NULL) {continue;}
        *temp = Memory_Free(*temp);
    }
    va_end(argPtr);
}


#ifdef MEMORY_TRACK
// **************************************************************************** Memory_Print

    // Print the total number of memory allocations and deallocations
    void Memory_Print(void){
        PRINT_LINE
        printf("MEMORY REPORT:\n");
        PRINT_LINE
        printf("Allocations:   %d\n", Glo_AllocCount);
        printf("Deallocations: %d\n", Glo_FreeCount);
        PRINT_LINE
    }
#endif



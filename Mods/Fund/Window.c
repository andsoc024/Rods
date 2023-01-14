// ============================================================================
// RODS
// Window
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions for managing the program's window.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"
#include "Fund.h"


// ============================================================================ PRIVATE CONSTANTS

#define INITIAL_SIZE                        SIZE(800, 600)
#define MIN_SIZE                            SIZE(500, 500)
#define WIN_TITLE                           APP_TITLE " v" APP_VERSION


// ============================================================================ FUNC DEF

// **************************************************************************** Window_Init

// Initialize the program's window
void Window_Init(void){
    Glo_WinSize = INITIAL_SIZE;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(Glo_WinSize.width, Glo_WinSize.height, WIN_TITLE);
    SetTargetFPS(FPS);
    SetWindowMinSize(MIN_SIZE.width, MIN_SIZE.height);
}


// **************************************************************************** Window_GetSize

// Return the current size of the window
Size Window_GetSize(void){
    return SIZE(GetScreenWidth(), GetScreenHeight());
}


// **************************************************************************** Window_UpdateWinSize

// Update the global Glo_WinSize, with the current window size
void Window_UpdateWinSize(void){
    Glo_WinSize = Window_GetSize();
}


// **************************************************************************** Window_Close

// Close the program's window
void Window_Close(void){
    CloseWindow();
    Glo_WinSize = SIZE_NULL;
}


#ifdef DEBUG_MODE
// **************************************************************************** Window_PrintSize

    // Print the window size, optionally with a new line
    void Window_PrintSize(bool withNewLine){
        Geo_PrintSize(Window_GetSize(), withNewLine);
    }


// **************************************************************************** Window_PrintAppInfo

    // Multiline print of the app info
    void Window_PrintAppInfo(void){
        printf("App Title:    %s\n", APP_TITLE);
        printf("App Version:  %s\n", APP_VERSION);
        printf("App Author:   %s\n", APP_AUTHOR);
        printf("App Company:  %s\n", APP_COMPANY);
        printf("App Year:     %s\n", APP_YEAR);
        printf("App Platform: %s\n", APP_PLATFORM);
    }
#endif


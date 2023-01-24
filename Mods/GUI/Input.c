// ============================================================================
// RODS
// Input
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Function for getting mouse or keyboard input.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"
#include "../Fund/Fund.h"
#include "GUI.h"


// ============================================================================ PRIVATE CONSTANTS

#define TURBO_KEY_COUNT                     TURBO_COUNT
#define MOUSE_DRAG_THRESHOLD                7.0f






// ============================================================================ FUNC DEF

// **************************************************************************** Event_GetUserInput

// Get mouse or keyboard input from the user
Event Event_GetUserInput(void){
    // Assume no keyboard or mouse event has occured
    Event res = EVENT_NULL;

    // Remeber the previous mouse position
    static Point previousMousePos = {0};
    // Remember if mouse dragging has started
    static bool draggingHasStarted = false;
    // Remember the last key pressed
    static KeyboardKey lastKeyPressed = KEY_NULL;
    // Count the frames a key is down
    static int keyDownCount = 0;
    // Remember the position the left mouse button was pressed
    static Point mousePressedPos = {0};

    // Get the current mouse position
    Point mousePos = GetMousePosition();

    // Check if a valid key is pressed or released
    for (int i = 0; i < WATCHED_KEYS_N; i++){
        KeyboardKey tempKey = WATCHED_KEYS[i];
        
        if (IsKeyPressed(tempKey)){
            lastKeyPressed = tempKey;
            keyDownCount = 0;
            res = Event_SetAsKeyPressed(tempKey);
            goto LAB_INPUT_EXIT;
        }

        if (IsKeyReleased(tempKey)){
            keyDownCount = 0;
            lastKeyPressed = KEY_NULL;
            res = Event_SetAsKeyReleased(tempKey);
            goto LAB_INPUT_EXIT;
        }
    }

    // Check if a key is down for more time than TURBO_KEY_COUNT
    if (lastKeyPressed != KEY_NULL && IsKeyDown(lastKeyPressed)){
        keyDownCount++;
        if (keyDownCount > TURBO_KEY_COUNT){
            res = Event_SetAsKeyPressed(lastKeyPressed);
            goto LAB_INPUT_EXIT;
        }
    }

    // Check if the left mouse button was pressed
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        mousePressedPos = mousePos;
        draggingHasStarted = false;
        res = Event_SetAsMousePressed(mousePos);
        goto LAB_INPUT_EXIT;
    }

    // Check if the left mouse button was released
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
        draggingHasStarted = false;
        res = Event_SetAsMouseReleased(mousePos);
        goto LAB_INPUT_EXIT;
    }

    // Check if the mouse wheel has moved
    float wheelMove = GetMouseWheelMove();
    if (wheelMove != 0){
        res = Event_SetAsMouseWheel(wheelMove, mousePos);
        goto LAB_INPUT_EXIT;
    }

    // Check if the mouse was moved
    Vector2 mouseDelta = Geo_PointsDelta(previousMousePos, mousePos);
    if (mouseDelta.x != 0.0f || mouseDelta.y != 0.0f){
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
            if (draggingHasStarted){
                res = Event_SetAsMouseDrag(mousePos, mouseDelta);
                goto LAB_INPUT_EXIT;

            }else{ // Mouse dragging has not started yet
                mouseDelta = Geo_PointsDelta(mousePressedPos, mousePos);
                if (ABS(mouseDelta.x) > MOUSE_DRAG_THRESHOLD || 
                    ABS(mouseDelta.y) > MOUSE_DRAG_THRESHOLD){
                    draggingHasStarted = true;
                    res = Event_SetAsMouseDrag(mousePos, mouseDelta);
                    goto LAB_INPUT_EXIT;
                }
            }

        }else{ // Left mouse button is not down
            res = Event_SetAsMouseMove(mousePos, mouseDelta);
            goto LAB_INPUT_EXIT;
        }
    }


    LAB_INPUT_EXIT:

    // Update the previous mouse position
    previousMousePos = mousePos;

    // Return the event
    return res;
}



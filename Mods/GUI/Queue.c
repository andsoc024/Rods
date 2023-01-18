// ============================================================================
// RODS
// Event Queue
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions for managing the event queue.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"
#include "../Fund/Fund.h"
#include "GUI.h"


// ============================================================================ PRIVATE CONSTANTS

#define QUEUE_EVENTS_N                      10

#define CURRENT_LIST                        0
#define NEXT_LIST                           1


// ============================================================================ OPAQUE STRUCTURES

// Structure that stores events. Events are stored in two lists: a current and 
// a next list. In each cycle the events in the current list are processed. New 
// events are added in the next list. Events from mouse and keyboard are stored 
// seperately for immediate processing
struct EventQueue{
    Event userInput;

    Event lists[2][QUEUE_EVENTS_N];
    int n[2];
    int current;
    int next;
};


// ============================================================================ PRIVATE FUNC DECL

int             Queue_FindEventIndexForTarget(EventQueue* queue, E_GuiID target, int listKind);
void            Queue_RemoveEventAtIndex(EventQueue* queue, int index, int listKind);
void            Queue_SwapLists(EventQueue* queue);






// ============================================================================ FUNC DEF

// **************************************************************************** Queue_Make

// Make an empty event queue
EventQueue* Queue_Make(void){
    EventQueue* queue = Memory_Allocate(NULL, sizeof(EventQueue), ZEROVAL_ALL);

    Queue_Clear(queue);

    return queue;
}


// **************************************************************************** Queue_Free

// Free the memory of the event queue
EventQueue* Queue_Free(EventQueue* queue){
    return Memory_Free(queue);
}


// **************************************************************************** Queue_Clear

// Clear all the events in the event queue
void Queue_Clear(EventQueue* queue){
    queue->userInput = EVENT_NULL;

    queue->n[0] = 0;
    queue->n[1] = 0;

    queue->current = 0;
    queue->next    = 1;
}


// **************************************************************************** Queue_GetNext

// Get the next event in the queue. If there are no more current events, return 
// EVENT_NULL
Event Queue_GetNext(EventQueue* queue){
    Event res;

    if (queue->userInput.id != EVENT_NONE){
        res = queue->userInput;
        queue->userInput = EVENT_NULL;
        return res;
    }

    int ind = queue->current;

    if (queue->n[ind] > 0){
        res = queue->lists[ind][queue->n[ind] - 1];
        if (res.target != GDG_NONE){
            Queue_AddEvent(queue, res);
        }

        queue->n[ind]--;

    }else{ // The number of events in the current list is 0
        Queue_SwapLists(queue);
        res = EVENT_NULL;
    }

    return res;
}


// **************************************************************************** Queue_AddEvent

// Add an event to the next list of the queue
void Queue_AddEvent(EventQueue* queue, Event event){
    if (event.id == EVENT_NONE) {return;}

    int ind = queue->next;

    queue->n[ind]++;
    queue->n[ind] = MIN(queue->n[ind], QUEUE_EVENTS_N);

    queue->lists[ind][queue->n[ind] - 1] = event;
}


// **************************************************************************** Queue_RemoveForTarget

// Remove the first event with the given target from the next list of the queue
void Queue_RemoveForTarget(EventQueue* queue, E_GuiID target){
    int index = Queue_FindEventIndexForTarget(queue, target, NEXT_LIST);
    if (index < 0) {return;}
    Queue_RemoveEventAtIndex(queue, index, NEXT_LIST);
}


// **************************************************************************** Queue_SetUserInput

// Set the user input of the event queue
void Queue_SetUserInput(EventQueue* queue, Event event){
    queue->userInput = event;
}


#ifdef DEBUG_MODE
// **************************************************************************** Queue_Print

    // Multiline print of the parameters of the event queue
    void Queue_Print(EventQueue* queue){
        CHECK_NULL(queue, WITH_NEW_LINE)

        printf("User Input: "); Event_Print(queue->userInput, WITH_NEW_LINE);
        printf("\n");

        for (int listInd = 0; listInd < 2; listInd++){
            printf("%s LIST", (listInd == queue->current) ? "CURRENT" : "NEXT");
            printf(" (%d):\n", queue->n[listInd]);
            int n = PUT_IN_RANGE(queue->n[listInd], 0, QUEUE_EVENTS_N);
            for (int i = 0; i < n; i++){
                printf("   %2d) ", i);
                Event_Print(queue->lists[listInd][i], WITH_NEW_LINE);
            }
            if (n == 0){
                printf("%s\n", STR_EMPTY);
            }
            printf("\n");
        }
        printf("Current: %d\n", queue->current);
        printf("Next:    %d\n", queue->next);
    }
#endif






// ============================================================================ PRIVATE FUNC DEF

// **************************************************************************** Queue_FindEventIndexForTarget

// The index of the first event with the given target in the given list. If no 
// event is found, return INVALID
int Queue_FindEventIndexForTarget(EventQueue* queue, E_GuiID target, int listKind){
    int listInd = (listKind == CURRENT_LIST) ? queue->current : queue->next;

    for (int i = 0; i < queue->n[listInd]; i++){
        if (queue->lists[listInd][i].target == target){
            return i;
        }
    }

    return INVALID;
}


// **************************************************************************** Queue_RemoveEventAtIndex

// Remove the event at the given index in the given list
void Queue_RemoveEventAtIndex(EventQueue* queue, int index, int listKind){
    int listInd = (listKind == CURRENT_LIST) ? queue->current : queue->next;

    for (int i = index; i < queue->n[listInd] - 1; i++){
        queue->lists[listInd][i] = queue->lists[listInd][i + 1];
    }

    if (IS_IN_RANGE(index, 0, queue->n[listInd] - 1)){
        queue->n[listInd]--;
    }
}


// **************************************************************************** Queue_SwapLists

// Swap the current and the next list
void Queue_SwapLists(EventQueue* queue){
    queue->n[queue->current] = 0;
    queue->current = (queue->current == 0) ? 1 : 0;
    queue->next    = (queue->next    == 0) ? 1 : 0; 
}




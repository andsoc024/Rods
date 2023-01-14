// ============================================================================
// RODS
// Time
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions for managing time.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"
#include "Fund.h"


// ============================================================================ FUNC DEF

// **************************************************************************** Time_Set

// Set an elapsed time. If the values are invalid, return TIME_INVALID
Time Time_Set(int hours, int minutes, int seconds){
    Time res = TIME(hours, minutes, seconds);

    if (!Time_IsValid(res)){
        return TIME_INVALID;
    }

    return res;
}


// **************************************************************************** Time_PutInValidRange

// Ensure that the time is valid
Time Time_PutInValidRange(Time t){
    t.hours   = PUT_IN_RANGE(t.hours, 0, TIME_MAX_HOURS);
    t.minutes = PUT_IN_RANGE(t.minutes, 0, 59);
    t.seconds = PUT_IN_RANGE(t.seconds, 0, 59);

    return t;
}


// **************************************************************************** Time_IsValid

// Return true if the time is within the valid range
bool Time_IsValid(Time t){
    return IS_IN_RANGE(t.hours,   0, TIME_MAX_HOURS) &&
           IS_IN_RANGE(t.minutes, 0, 59)             &&
           IS_IN_RANGE(t.seconds, 0, 59);
}


// **************************************************************************** Time_IsEqual

// Return true if the two times are the same
bool Time_IsEqual(Time t1, Time t2){
    return (t1.hours   == t2.hours)   &&
           (t1.minutes == t2.minutes) &&
           (t1.seconds == t2.seconds);
}


// **************************************************************************** Time_IsSmaller

// Return true if t1 is smaller than t2. If t1 is invalid, return false. If t2 
// is invalid and t1 valid, return true
bool Time_IsSmaller(Time t1, Time t2){
    if (!Time_IsValid(t1)){
        return false;
    }

    if (!Time_IsValid(t2)){
        return true;
    }

    return Time_ToInt(t1) < Time_ToInt(t2);
}


// **************************************************************************** Time_Increment

// Increment the time by 1 second. Keep it within the valid range
Time Time_Increment(Time t){
    if (!Time_IsValid(t)) {return TIME_INVALID;}

    if (Time_IsEqual(t, TIME_MAX)) {return TIME_MAX;}

    t.seconds++;
    if (t.seconds >= 60){
        t.seconds = 0;
        t.minutes++;
        if (t.minutes >= 60){
            t.minutes = 0;
            t.hours++;
        }
    }

    return t;
}


// **************************************************************************** Time_Shift

// Shift the time by dt. Keep it within the valid range
Time Time_Shift(Time t, int dt){
    if (!Time_IsValid(t)) {return TIME_INVALID;}

    int secs = Time_ToInt(t);
    secs += dt;
    secs = PUT_IN_RANGE(secs, 0, TIME_MAX_SECS);

    return Time_FromInt(secs);
}


// **************************************************************************** Time_ToInt

// The total number of seconds. If the time is invalid, return INVALID
int Time_ToInt(Time t){
    if (!Time_IsValid(t)) {return INVALID;}

    return t.hours   * 60 * 60 + 
           t.minutes * 60      + 
           t.seconds;
}


// **************************************************************************** Time_FromInt

// The time from the total number of seconds
Time Time_FromInt(int secs){
    if (!IS_IN_RANGE(secs, 0, TIME_MAX_SECS)){
        return TIME_INVALID;
    }

    Time res;
    res.seconds = secs % 60;
    secs /= 60;
    res.minutes = secs % 60;
    secs /= 60;
    res.hours = secs;

    return res;
}


// **************************************************************************** Time_ToString

// Turn the time to a string in the form [HH:]MM:SS
char* Time_ToString(char* dst, Time t){
    if (!Time_IsValid(t)){
        return String_Copy(dst, TIME_INVALID_STR);
    }

    bool form = (t.hours > 0) ? LONG_FORM : SHORT_FORM;

    dst = Memory_Allocate(dst, (form == SHORT_FORM) ? 6 : 9, ZEROVAL_LAST);
    char* ptr = dst;

    for (int unit = HOURS; unit <= SECONDS; unit++){
        if (unit == HOURS && form == SHORT_FORM){
            continue;
        }

        *ptr = t.values[unit] / 10 + '0';
        ptr++;
        *ptr = t.values[unit] % 10 + '0';
        ptr++;

        if (unit != SECONDS){
            *ptr = TIME_SEP_CHAR;
            ptr++;
        }
    }

    return dst;
}


#ifdef DEBUG_MODE
// **************************************************************************** Time_Print

    // Print the time in the form [HH:]MM:SS. Optionally with new line
    void Time_Print(Time t, bool withNewLine){
        char* str = Time_ToString(NULL, t);
        printf("%s", str);
        str = Memory_Free(str);

        if (withNewLine) {printf("\n");}
    }
#endif


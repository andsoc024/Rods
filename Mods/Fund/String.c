// ============================================================================
// RODS
// String
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions for manipulating strings.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include <stdarg.h>

#include "../Public/Public.h"
#include "Fund.h"


// ============================================================================ FUNC DEF

// **************************************************************************** String_Length

// The number of characters in the string, excluding the terminator
int String_Length(const char* str){
    int count = 0;

    for (; *str != '\0'; str++){
        count++;
    }

    return count;
}


// **************************************************************************** String_IsEqual

// Return true if the two strings are the same
bool String_IsEqual(const char* str1, const char* str2){
    if (str1 == NULL || str2 == NULL){
        return false;
    } 

    for (; *str1 != '\0' && *str2 != '\0'; str1++, str2++){
        if (*str1 != *str2){
            return false;
        }
    }

    return (*str1 == *str2);
}


// **************************************************************************** String_Copy

// Copy the string from src to dst
char* String_Copy(char* dst, const char* src){
    return Memory_Copy(dst, src, String_Length(src) + 1);
}


// **************************************************************************** String_Concat

// Concatenate the strings and save the results at dst
char* String_Concat(char* dst, int nStrings, ...){
    // If the number of strings is invalid, return an empty string
    if (nStrings <= 0){
        return String_Copy(dst, "");
    }

    // Pointer to the string list in the arguments
    va_list argPtr;

    // Determine the length of the concatenated string
    int length = 0;
    va_start(argPtr, nStrings);
    for (int i = 0; i < nStrings; i++){
        char* temp = va_arg(argPtr, char*);
        if (temp == NULL) {continue;}
        length += String_Length(temp);
    }
    va_end(argPtr);

    // Allocate memory on a new pointer in case dst is also in the string list 
    // in the arguments
    char* res = Memory_Allocate(NULL, length + 1, ZEROVAL_LAST);
    char* ptr = res;

    // Concatenate the strings
    va_start(argPtr, nStrings);
    for (int i = 0; i < nStrings; i++){
        char* temp = va_arg(argPtr, char*);
        if (temp == NULL) {continue;}
        for (; *temp != '\0'; temp++, ptr++){
            *ptr = *temp;
        }
    }
    va_end(argPtr);

    // Free the memory at dst
    dst = Memory_Free(dst);

    // Return the concatenated string
    return res;
}


// **************************************************************************** String_FromInt

// Turn the integer to string. Save the result at dst
char* String_FromInt(char* dst, int num){
    // Mark if the number is negative
    bool isNegative = (num < 0);
    num = ABS(num);

    // Determine the number of digits
    int nDigits = 1;
    int num2 = num;
    while ((num2 /= 10) > 0){
        nDigits++;
    }

    // Allocate memory for the string
    dst = Memory_Allocate(dst, nDigits + 1, ZEROVAL_LAST);
    char* ptr = dst + nDigits - 1;

    // Write each digit, starting from the last
    do{
        *ptr = num % 10 + '0';
        ptr--;
    }while ((num /= 10) > 0);

    // If negative, add the minus sign
    if (isNegative){
        dst = String_Concat(dst, 2, "-", dst);
    }

    // Return the string
    return dst;
}


// **************************************************************************** String_Pad

// Pad the string on the left with the padding character, if needed, so that it 
// has the given minimum length
char* String_Pad(char* str, int minLength, char padChar){
    int oldLength = String_Length(str);
    int newLength = MAX(minLength, oldLength);
    int padLength = newLength - oldLength;

    if (padLength <= 0){
        return str;
    }

    char* padStr = Memory_Allocate(NULL, padLength + 1, ZEROVAL_LAST);
    Memory_Set(padStr, padLength, padChar);

    str = String_Concat(str, 2, padStr, str);

    return str;
}


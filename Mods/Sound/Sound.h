// ============================================================================
// RODS
// Sound
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions for managing music and sound.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"


// ============================================================================ FUNC DECL

// ---------------------------------------------------------------------------- Sound Functions

void            Sound_LoadAll(void);
void            Sound_UnloadAll(void);
void            Sound_TurnOn(void);
void            Sound_TurnOff(void);
void            Sound_Toggle(void);
bool            Sound_IsOn(void);
void            Sound_PlaySoundFX(E_SoundFX sfx);
#ifdef DEBUG_MODE
    void        Sound_Print(void);
#endif



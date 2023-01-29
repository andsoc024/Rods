// ============================================================================
// RODS
// Sound FX
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions for loading and unloading the sound assets and playing sound 
    effects.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Assets/Assets.h"

#include "../Public/Public.h"
#include "../Fund/Fund.h"
#include "Sound.h"


// ============================================================================ FUNC DEF

// **************************************************************************** Sound_LoadAll

// Initialize the sound and load all the sound data in the global Glo_SoundData
void Sound_LoadAll(void){
    InitAudioDevice();

    Sound_UnloadAll();

    Glo_SoundData = Memory_Allocate(NULL, sizeof(SoundData), ZEROVAL_ALL);

    Glo_SoundData->soundOn = true;

    Glo_SoundData->musicVolume = 1.0f;
    Glo_SoundData->musicDVol = 0.0f;

    const unsigned char* ptrs[] = {
        Asset_Sound_Press, 
        Asset_Sound_Electricity, 
        Asset_Sound_Victory
    };
    const int dataSizes[] = {
        Asset_Sound_Press_Size,
        Asset_Sound_Electricity_Size,
        Asset_Sound_Victory_Size
    };

    for (int sfx = 0; sfx < SOUND_FX_N; sfx++){
        Wave temp = LoadWaveFromMemory(".wav", ptrs[sfx], dataSizes[sfx]);
        Glo_SoundData->soundFX[sfx] = LoadSoundFromWave(temp);
        UnloadWave(temp);
    }

    Glo_SoundData->music = LoadMusicStreamFromMemory(".mp3", Asset_Music_Barroom, 
                                                     Asset_Music_Barroom_Size);
}


// **************************************************************************** Sound_UnloadAll

// Unload all sound assets and deiinitialize the sound
void Sound_UnloadAll(void){
    if (Glo_SoundData == NULL) {return;}

    for (int sfx = 0; sfx < SOUND_FX_N; sfx++){
        UnloadSound(Glo_SoundData->soundFX[sfx]);
        Glo_SoundData->soundFX[sfx] = (Sound) {0};
    }

    UnloadMusicStream(Glo_SoundData->music);
    Glo_SoundData->music = (Music) {0};

    Glo_SoundData = Memory_Free(Glo_SoundData);

    CloseAudioDevice();
}


// **************************************************************************** Sound_TurnOn

// Turn the sound index on
void Sound_TurnOn(void){
    Glo_SoundData->soundOn = true;
    Music_SetVolume(1.0f);
}


// **************************************************************************** Sound_TurnOff

// Turn the sound index off
void Sound_TurnOff(void){
    Glo_SoundData->soundOn = false;
    Music_SetVolume(0.0f);
}


// **************************************************************************** Sound_Toggle

// Invert the sound index
void Sound_Toggle(void){
    if (Sound_IsOn()){
        Sound_TurnOff();
    }else{
        Sound_TurnOn();
    }
}


// **************************************************************************** Sound_IsOn

// Return true if the sound index is on
bool Sound_IsOn(void){
    return Glo_SoundData->soundOn;
}


// **************************************************************************** Sound_PlaySoundFX

// Play the given sound fx, if the sound index is on
void Sound_PlaySoundFX(E_SoundFX sfx){
    if (Glo_SoundData->soundOn){
        PlaySound(Glo_SoundData->soundFX[sfx]);
    }
}


#ifdef DEBUG_MODE
// **************************************************************************** Sound_Print

    // Print the parameters of the Glo_SoundData object
    void Sound_Print(void){
        CHECK_NULL(Glo_SoundData, WITH_NEW_LINE)

        printf("Sound On: %s\n", Bool_ToString(Glo_SoundData->soundOn, LONG_FORM));
        printf("Music Volume: %.3f\n", Glo_SoundData->musicVolume);
        printf("Music dVol: %.3f\n", Glo_SoundData->musicDVol);
        
        printf("SOUND EFFECTS (%d):\n", SOUND_FX_N);
        for (int sfx = 0; sfx < SOUND_FX_N; sfx++){
            printf("   %d %s) ", sfx, SoundFX_ToString(sfx));
            printf("%d\n", Glo_SoundData->soundFX[sfx].frameCount);
        }

        printf("MUSIC:\n");
        printf("%d\n", Glo_SoundData->music.frameCount);
    }
#endif


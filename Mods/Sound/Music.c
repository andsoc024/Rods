// ============================================================================
// RODS
// Music
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions for managing the music of the game.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"
#include "../Fund/Fund.h"
#include "Sound.h"


// ============================================================================ PRIVATE CONSTANTS

#define MUSIC_FADEOUT_FRAMES_N              ((FPS * 3) / 1)


// ============================================================================ FUNC DEF

// **************************************************************************** Music_Start

// Start playing the music
void Music_Start(void){
    PlayMusicStream(Glo_SoundData->music);
    Music_SetVolume(Glo_SoundData->soundOn ? 1.0f : 0.0f);
    Glo_SoundData->musicDVol = 0.0f;
}


// **************************************************************************** Music_Stop

// Stop playing the music
void Music_Stop(void){
    StopMusicStream(Glo_SoundData->music);
}


// **************************************************************************** Music_Restart

// Stop and restart the music
void Music_Restart(void){
    Music_Stop();
    Music_Start();
}


// **************************************************************************** Music_FadeOut

// Slowly decrease the music volume until it fades out
void Music_FadeOut(void){
    Glo_SoundData->musicDVol = - (1.0f / (float) MUSIC_FADEOUT_FRAMES_N);
}


// **************************************************************************** Music_SetVolume

// Set the volume of the music
void Music_SetVolume(float volume){
    volume = PUT_IN_RANGE(volume, 0.0f, 1.0f);
    SetMusicVolume(Glo_SoundData->music, volume);
    Glo_SoundData->musicVolume = volume;
}


// **************************************************************************** Music_Update

// Update the music stream
void Music_Update(void){
    if (!IsMusicStreamPlaying(Glo_SoundData->music)) {return;}

    UpdateMusicStream(Glo_SoundData->music);

    if (Glo_SoundData->musicDVol != 0.0f){
        float newVolume = Glo_SoundData->musicVolume + Glo_SoundData->musicDVol;
        Music_SetVolume(newVolume);

        if (Glo_SoundData->musicVolume <= 0.0f){
            Music_Stop();
        }
    }
}


// **************************************************************************** Music_IsPlaying

// Return true if the music is currently playing
bool Music_IsPlaying(void){
    return IsMusicStreamPlaying(Glo_SoundData->music);
}



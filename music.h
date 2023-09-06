// music.h

#ifndef MUSIC_H
#define MUSIC_H

#include <ArduboyTones.h>
#include <ArduboyTonesPitches.h>

// Title Song
extern const uint16_t titleSong[] PROGMEM = {
    NOTE_REST,500,
    NOTE_FS2,250, NOTE_G2,150,
    NOTE_C3,75, NOTE_D3,75, NOTE_E3,75, NOTE_FS3,75,
    NOTE_GS3,75, NOTE_AS3,75, NOTE_C4,75, NOTE_D4,75,
    NOTE_E4,75, NOTE_FS4,75, NOTE_GS4,75, NOTE_AS4,75,
    NOTE_C5,75, NOTE_D5,75, NOTE_E5,75, NOTE_FS5,150,
    NOTE_REST,200,
    NOTE_G4,200, NOTE_A3,300, NOTE_D3,300,
    TONES_END };

#endif
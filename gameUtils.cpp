#include "GameUtils.h"

// Let the user pick numbers, scrolling around
// Used in ::GameSetup and ::Playing
void pickNumber(int& pickedNumb, int& randomLimit, bool silent, Arduboy2& a, ArduboyTones& sound) {
    int framesDelay = 6; // Set the speed of held-button changing

    // Press or hold UP
    if (a.pressed(UP_BUTTON)) {
        if (a.everyXFrames(framesDelay)) {
            if ((pickedNumb + 1) < randomLimit) {
                // If pickedNumb+1 still in limits
                pickedNumb++;
                if (!silent) {
                    sound.tone((71+(pickedNumb*2)),80);
                }
            } else {
                // If pickedNumb+1 too large
                sound.tone(NOTE_E4,60, NOTE_REST,60, NOTE_E4,80);
            }
        }
    }
    // Press or hold DOWN
    if (a.pressed(DOWN_BUTTON)) {
        if (a.everyXFrames(framesDelay)) {
            if ((pickedNumb - 1) > 0) {
                // If pickedNumb+1 still in limits
                pickedNumb--;
                if (!silent) {
                    sound.tone((71+(pickedNumb*2)),80);
                }
            } else {
                // If pickedNumb-1 too small
                sound.tone(NOTE_A1,60, NOTE_REST,60, NOTE_A1,80);
            }
        }
    }
}


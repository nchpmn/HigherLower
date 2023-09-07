#ifndef PLAYING_STATE_H
#define PLAYING_STATE_H

// Include Arduboy Libraries
#include "Arduboy2.h"
#include "ArduboyTones.h"
#include "gameState.h"

// Declare global variables being used inside of this class
extern bool playerWin;
extern int targetNumb;
extern int randomLimit;
extern GameState gameState;

class PlayingState {
public:
    // Constructor
    PlayingState(Arduboy2& arduboy, ArduboyTones& tones);

    void guess();

private:
    // Declare an Arduboy2 object as "a" here
    Arduboy2& a;
    // And ArduboyTones object as "sound"
    ArduboyTones& sound;
    
    // Other member variables and functions specific to PlayingState
    int attempts = 0;
    int guessedNumb = random(1, randomLimit);
    int lastGuess = 0;
};

#endif

#ifndef PLAYING_STATE_H
#define PLAYING_STATE_H

// Include Arduboy Libraries
#include "Arduboy2.h"
#include "ArduboyTones.h"

class PlayingState {
public:
    PlayingState(Arduboy2& arduboy);  // Constructor

    void guess();

private:
    // You can declare an Arduboy2 object as "a" here
    Arduboy2& a;
    
    // Other member variables and functions specific to PlayingState
};

#endif

#include "PlayingState.h"
#include "GameUtils.h"  // Include the GameUtils header
#include "graphics.h"

// Constructor
PlayingState::PlayingState(Arduboy2& arduboy) : a(arduboy) {
    int attempts = 0;
}

void PlayingState::guess() {
    Sprites::drawOverwrite(0, 0, playHeader, 0);
}
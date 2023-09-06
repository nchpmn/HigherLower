#include "PlayingState.h"
#include "GameUtils.h"  // Include the GameUtils header

PlayingState::PlayingState(Arduboy2 &arduboy) : arduboy(arduboy) {
    // Constructor implementation (if needed)
}

void PlayingState::init() {
    // Initialization code for PlayingState
}

void PlayingState::update() {
    // Update code for PlayingState
}

void PlayingState::draw() {
    // Drawing code for PlayingState
    arduboy.clear();
    // Call pickNumber function in PlayingState.cpp
    int targetNumber = 0;
    bool isTwoPlayer = true;  // Example, change as needed
    // pickNumber(targetNumber, isTwoPlayer, arduboy, arduboy.tunes());

    // Draw game elements here
    arduboy.display();
}
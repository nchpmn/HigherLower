// HIGHERLOWER
// A game of logic, to guess the random number using only two clues: HIGHER or LOWER!
//
// This started life as the tutorial by @Crait
// https://community.arduboy.com/t/7928
//
// Code & Graphics licenced under GNU GPL 3.0
//
// 15 August 2023 (Original)
// 17 August 2023 (Major Refactor)
// 20 August 2023 (Initial Release - 1.0.0)
// 06 September 2023 (Public Accountment & Release - 1.3.0)
// 07 September 2023 (Begin v2.0.0 Rewrite)
// Nathan Chapman -- @nchpmn

#define VERSION "v2.0.0-prealpha"

// Arduboy Library Setup
#include <Arduboy2.h>
Arduboy2 a;
#include <ArduboyTones.h>
#include <ArduboyTonesPitches.h>
ArduboyTones sound(a.audio.enabled);

// Include Custom Modules
#include "graphics.h" // honestly right now this is just here for testing

// State Machine Setup
enum class GameState {
    Title,
    Credits,
    ModeSelect,
    GameSetup,
    Playing,
    EndScreen
};
GameState currentState = GameState::Title;

// Setup - Run once at the beginning of the program
void setup() {
    a.begin();
    a.setFrameRate(60);
    a.initRandomSeed();
}

// Main Loop - Run continuously forever and ever and ever and ever
void loop() {
    // If it's not time to draw the next frame
    if (!a.nextFrame())
        // Exit the loop() early - i.e. wait for next frame time
        return;
    
    a.clear();
    a.pollButtons();

    // Choose what to do based on the 'gameState' variable
    switch(currentState) {
        // Title Screen
        case GameState::Title: {
            Sprites::drawOverwrite(0, 0, title, 0);
        }
        break;

        // Credits
        case GameState::Credits: {
            // Credits here
        }
        break;

        // Mode Select - 1P or 2P
        case GameState::ModeSelect: {
            // Mode Select
        }
        break;
        
        // Game Setup
        case GameState::GameSetup: {
            // Game Setup - ask for target number or randomly generate
            // Also reset all the variables
            // The game returns to here after each round
        }
        break;

        // Main Game
        case GameState::Playing: {
            // Main Game... Lots Here...
        }
        break;

        // End Screen
        case GameState::EndScreen: {
            // End screen - win or lose message
        }
        break;
    }

    // Update the screen
    a.display();
}
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

#define VERSION "v2-prealpha-230907"

// Arduboy Library Setup
#include <Arduboy2.h>
Arduboy2 a;
#include <ArduboyTones.h>
#include <ArduboyTonesPitches.h>
ArduboyTones sound(a.audio.enabled);

// Include Custom Modules
#include "graphics.h"
#include "music.h"

// State Machine Setup
enum class GameState {
    Title,
    Credits,
    ModeSelect,
    GameSetup,
    Playing,
    EndScreen
};
GameState gameState = GameState::Title;

// Global Variable Setup
bool modeSingle = true;

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
    switch(gameState) {
        // Title Screen
        case GameState::Title: {
            Sprites::drawOverwrite(0, 0, title, 0);

            static bool titleSongFlag = false;
            if (!titleSongFlag) {
                sound.tones(titleSong);
                titleSongFlag = true;
            }
            
            if (a.justPressed(A_BUTTON)) {
                gameState = GameState::ModeSelect;
            }
            if (a.justPressed(B_BUTTON)) {
                gameState = GameState::Credits;
            }
        }
        break;

        // Credits
        case GameState::Credits: {
            Sprites::drawOverwrite(0, 0, credits, 0);

            // Return to title screen
            if (a.justPressed(B_BUTTON|A_BUTTON)) {
                gameState = GameState::Title;
            }

            // Toggle showing version number
            static bool showVersion = false;
            if (showVersion) {
                a.setCursor(0,50);
                a.print(VERSION);
            }
            if (a.justPressed(DOWN_BUTTON)) {
                showVersion = !showVersion;
            }
        }
        break;

        // Mode Select - 1P or 2P
        case GameState::ModeSelect: {
            Sprites::drawOverwrite(0, 0, modeselect, 0);
            if (modeSingle) {
                // Single-player currently selected
                a.fillRect(10, 55, 41, 2, BLACK);
            } else {
                // 2-Player selected
                a.fillRect(71, 55, 47, 2, WHITE);
            }

            if (a.justPressed(LEFT_BUTTON)) {
                modeSingle = true;
                sound.tone(NOTE_A1,60);
            }
            if (a.justPressed(RIGHT_BUTTON)) {
                modeSingle = false;
                sound.tone(NOTE_B1,60);
            }
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
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

// Global State Machine Setup
enum class GameState {
    Title,
    Credits,
    ModeSelect,
    GameSetup,
    Playing,
    EndScreen
};
GameState gameState = GameState::Title;

// GameSetup State Machine
enum class SetupState {
    Reset,
    PickNumber
};
SetupState setupState = SetupState::Reset;

// Global Variable Setup
bool modeSingle = true;
int attempts = 0;
bool playerWin = false;
int targetNumb = 0;
int gussedNumb = 0;
int randomLimit = 101;


// Setup - Run once at the beginning of the program
void setup() {
    a.begin();
    a.setFrameRate(60);
    a.initRandomSeed();
}

// Let the user pick numbers, scrolling around
void pickNumber(int& pickedNumb, bool silent, Arduboy2& a, ArduboyTones& sound) {
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
            
            if (a.justPressed(A_BUTTON)) {
                gameState = GameState::GameSetup;
            }
        }
        break;
        
        // Game Setup
        case GameState::GameSetup: {
            // Game Setup - ask for target number or randomly generate
            // Also reset all the variables
            // The game returns to here after each round

            switch(setupState) {
                // Game Setup code to run once only
                case SetupState::Reset: {
                    // Reset Game Variables
                    attempts = 0;
                    playerWin = false;
                    a.digitalWriteRGB(RGB_OFF,RGB_OFF,RGB_OFF);
                    targetNumb = random(1,randomLimit); // random() generates high-1
                    gussedNumb = random(1,randomLimit);

                    // Move on to PickNumber state immediately
                    setupState = SetupState::PickNumber;
                }
                break;

                // Additional 2-Player Setup
                case SetupState::PickNumber: {
                    // Additional 2-Player Setup
                    if (!modeSingle) {
                        Sprites::drawOverwrite(0, 0, targetHeader, 0);
                        a.setTextSize(2);
                        a.setCursor(10,45);
                        a.print(targetNumb);

                        pickNumber(targetNumb, true, a, sound);
                        if (a.justPressed(A_BUTTON)) {
                            gameState = GameState::Playing;
                        }
                    } else {
                        // Single-Player Game
                        gameState = GameState::Playing;
                    }
                }
                break;
            }
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
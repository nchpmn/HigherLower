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

#define VERSION "v2-dev-230918"

// Arduboy Library Setup
#include <Arduboy2.h>
Arduboy2 a;
#include <ArduboyTones.h>
#include <ArduboyTonesPitches.h>
ArduboyTones sound(a.audio.enabled);

// Include Custom Modules
#include "gameUtils.h"  // Custom functions available everywhere
#include "graphics.h" // Bitmap arrays
#include "music.h" // Tones and beeps
#include "gameState.h" // Global State Machine Class
GameState gameState = GameState::Title;
#include "playingState.h" // GameState::Playing split into its own file
PlayingState playingState(a, sound);

// Global Variable Setup
bool modeSingle = true;
bool playerWin = false;
int targetNumb = 0;
int guessedNumb = 0;
int randomLimit = 101;
int levelIndex = 0;
const char* levelNames[] = {"BEGINNER", "NOVICE", "AVERAGE", "HARD", "VERY HARD", "GOOD LUCK"};
int levelAttempts[] = {10, 8, 7, 6, 5, 1};


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
            Sprites::drawOverwrite(0, 0, modeSelect, 0);
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
                playingState.reset(); // Reset Game variables for new game
                gameState = GameState::GameSetup;
            }
        }
        break;
        
        // Game Setup
        case GameState::GameSetup: {
            // Game Setup - ask for target number or randomly generate
            // Additional 2-Player Setup
            if (!modeSingle) {
                a.invert(true);
                Sprites::drawOverwrite(0, 0, targetHeader, 0);

                // This is all to center the targetNumb
                static int y = 32;
                if (targetNumb < 10) {
                    a.setCursor(61,y);
                } else if (targetNumb < 100) {
                    a.setCursor(53,y);
                } else {
                    a.setCursor(45,y);
                }
                a.setTextSize(2);
                a.print(targetNumb);

                pickNumber(targetNumb, randomLimit, true, a, sound);
                if (a.justPressed(A_BUTTON)) {
                    a.invert(false);
                    levelIndex = 0;
                    gameState = GameState::Playing;
                }
            } else {
                // Single-Player Game
                gameState = GameState::LevelScreen;
            }
        }
        break;

        // Level Screen
        case GameState::LevelScreen: {
            a.fillRect(0, 0, 128, 17, WHITE);
            Sprites::drawOverwrite(42, 0, levelTitle, 0);
            Sprites::drawOverwrite(54, 20, levelNumbs, levelIndex);

            int xOffset = (128 - (strlen(levelNames[levelIndex]))*6-1) / 2;
            a.setCursor(xOffset,52);
            a.print(levelNames[levelIndex]);

            if (a.justPressed(A_BUTTON)) {
                gameState = GameState::Playing;
            }
        }
        break;

        // Main Game
        case GameState::Playing: {
            playingState.guess();
        }
        break;

        // End Screen
        case GameState::EndScreen: {
            if (playerWin) {
                // Write our win grapic to screen at position x0 y0
                Sprites::drawOverwrite(0, 0, win, 0);
                a.digitalWriteRGB(RGB_OFF,RGB_ON,RGB_OFF);
            } else {
                // Write our win grapic (array) to screen at position x0 y0
                Sprites::drawOverwrite(0, 0, lose, 0);
                a.digitalWriteRGB(RGB_ON,RGB_OFF,RGB_OFF);
            }

            if (a.justPressed(A_BUTTON)) {
                gameState = GameState::ModeSelect;
            }
        }
        break;
    }

    // Update the screen
    a.display();
}
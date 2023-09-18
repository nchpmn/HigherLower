#include "playingState.h"
#include "gameUtils.h"  // Include the GameUtils header
#include "graphics.h"
#include "music.h"
#include "gameState.h"

// Constructor
PlayingState::PlayingState(Arduboy2& arduboy, ArduboyTones& tones) : a(arduboy), sound(tones) {

}

void PlayingState::reset() {
    // Turn off LED
    a.digitalWriteRGB(RGB_OFF,RGB_OFF,RGB_OFF);

    // Reset variables to starting state
    attempts = 0; // Different level attempts set by levelAttempts[levelIndex]
    lastGuess = 0;
    targetNumb = random(1,randomLimit); // random() generates high-1
    guessedNumb = random(1,randomLimit);
    playerWin = false;
}

void PlayingState::guess() {
    if (modeSingle) {
        Sprites::drawOverwrite(0, 0, playHeader, 0);
    } else {
        Sprites::drawOverwrite(0, 0, playP2Header, 0);
    }
    
    
    // Draw guessedNumb to screen
    static int y = 26;
    if (guessedNumb < 10) {
        a.setCursor(61,y);
    } else if (guessedNumb < 100) {
        a.setCursor(53,y);
    } else {
        a.setCursor(45,y);
    }
    a.setTextSize(2);
    a.print(guessedNumb);
    a.setTextSize(1);

    // Draw attempts remaining
    a.setCursor(29,43);
    a.print("Attempts: ");
    a.print(levelAttempts[levelIndex] - attempts);

    // And a hint
    if(attempts > 0) {
        if (lastGuess > targetNumb) {
            a.setCursor(20,53);
            a.print(lastGuess);
            a.print(" was too high");
        } else {
            a.setCursor(22,53);
            a.print(lastGuess);
            a.print(" was too low");
        }
    }

    // Select Number
    pickNumber(guessedNumb, randomLimit, false, a, sound);

    // Submit Guess
    if (a.justPressed(A_BUTTON)) {
        if (guessedNumb == targetNumb) {
            // Play the 'you win' tune
            sound.tones(winSong);

            // Transition the game into the 'player win' state
            playerWin = true;
            levelIndex++;
            gameState = GameState::EndScreen;
            
        } else {
            attempts++;
            if (attempts == levelAttempts[levelIndex]) {
                // Play the "you lose" tune
                sound.tones(loseSong);

                // Transition to 'player lose' state
                levelIndex = 0;
                gameState = GameState::EndScreen;
            } else {
                lastGuess = guessedNumb;
                a.digitalWriteRGB(RGB_ON,RGB_OFF,RGB_OFF);
                sound.tone(NOTE_D1,60, NOTE_REST,20, NOTE_D1,60);
            }
        }
    }
}
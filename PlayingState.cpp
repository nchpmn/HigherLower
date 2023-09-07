#include "playingState.h"
#include "gameUtils.h"  // Include the GameUtils header
#include "graphics.h"
#include "gameState.h"

// Constructor
PlayingState::PlayingState(Arduboy2& arduboy, ArduboyTones& tones) : a(arduboy), sound(tones) {
    
}

void PlayingState::guess() {
    Sprites::drawOverwrite(0, 0, playHeader, 0);
    
    a.setCursor(0,30);
    a.setTextSize(1);
    a.print("     Attempts: ");
    a.print(7 - attempts);
    a.print("\n Number to Guess: ");
    a.print(guessedNumb);
    if(attempts > 0) {
        // After the first guess, give a hint
        a.print("\n\n    ");
        a.print(lastGuess);
        if (lastGuess > targetNumb) {
            a.print(" was too high");
        } else {
            a.print(" was too low");
        }
    }

    // Select Number
    pickNumber(guessedNumb, randomLimit, false, a, sound);

    // Submit Guess
    if (a.justPressed(A_BUTTON)) {
        if (guessedNumb == targetNumb) {
            // Play the 'you win' tune
            // sound.tones(winSong); Come back to this - requires to create
            // music.cpp and fix declare/define in music.h

            // Transition the game into the 'player win' state
            playerWin = true;
            gameState = GameState::EndScreen;
            
        } else {
            attempts++;
            if (attempts == 7) {
                // Play the "you lose" tune
                // sound.tones(loseSong);

                // Transition to 'player lose' state
                gameState = GameState::EndScreen;
            } else {
                lastGuess = guessedNumb;
                a.digitalWriteRGB(RGB_ON,RGB_OFF,RGB_OFF);
                sound.tone(NOTE_D1,60, NOTE_REST,20, NOTE_D1,60);
            }
        }
    }
}